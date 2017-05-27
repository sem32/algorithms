#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG

#define FILE_IN "gamsrv.in"
#define FILE_OUT "gamsrv.out"

struct went;

typedef enum NODE_TYPE {
    ROUTE,
    CLIENT
} e_node_t;

typedef struct edge {
    unsigned long long latency;
    struct node *startnode;
    struct node *endtnode;
} edge_t;

typedef struct list_edges {
    edge_t *data;
    struct list_edges *next;
} list_edges_t;

typedef struct node {
    int number;
    list_edges_t *head;
    list_edges_t *tail;
    unsigned long long dist;
    int visited;

    e_node_t type;
} node_t;

void set_clients(char *data, node_t **nodes)
{
    int length_str, i;
    int number = 0, counter = 0;

    if (!data || !nodes) {
        return;
    }

    length_str = strlen(data);

    for (i = 0; i < length_str; i++) {
        if (data[i] == ' ' || data[i] == '\n') {
            counter++;
        }
    }

    for (i = 0; i < length_str; i++) {
        if (data[i] == ' ' || data[i] == '\n') {
            if (counter > 0) {

                if (nodes[number] == NULL) {
                    nodes[number] = calloc(1, sizeof(node_t));
                    nodes[number]->number = number;
                }

                nodes[number]->type = CLIENT;
                number = 0;
                counter = 0;
            }
        } else {
            counter++;
            number = number * 10 + (data[i] - '0');
        }
    }
    if (counter > 0) {
        if (nodes[number] == NULL) {
            nodes[number] = calloc(1, sizeof(node_t));
            nodes[number]->number = number;
        }
        nodes[number]->type = CLIENT;
    }
    return;
}

void get_data(FILE *file, node_t ***_nodes, int *length_nodes)
{
    char *line = NULL;
    size_t n = 0;
    int startnode = 0, endnode = 0;
    unsigned long long latency = 0;
    int length_edges;

    fscanf(file, "%d %d\n", length_nodes, &length_edges);

    *length_nodes = *length_nodes + 1;

    node_t **nodes = *_nodes = calloc(*length_nodes, sizeof(node_t *));

    if (getline(&line, &n, file) > 0) {
        set_clients(line, nodes);
        if (line) {
            free(line);
            line = NULL;
        }
    }

    while ((fscanf(file, "%d %d %lld", &startnode, &endnode, &latency) != EOF)) {
        if (nodes[startnode] == NULL) {
            nodes[startnode] = calloc(1, sizeof(node_t));
            nodes[startnode]->number = startnode;
        }

        if (nodes[endnode] == NULL) {
            nodes[endnode] = calloc(1, sizeof(node_t));
            nodes[endnode]->number = endnode;
        }

        edge_t *edge = calloc(1, sizeof(edge_t));
        edge->startnode = nodes[startnode];
        edge->endtnode = nodes[endnode];
        edge->latency = latency;

        list_edges_t *list_edges_startnode = calloc(1, sizeof(list_edges_t));
        list_edges_startnode->data = edge;
        list_edges_startnode->next = NULL;

        if (nodes[startnode]->head == NULL) {
            nodes[startnode]->head = list_edges_startnode;
        } else {
            nodes[startnode]->tail->next = list_edges_startnode;
        }
        nodes[startnode]->tail = list_edges_startnode;

        list_edges_t *list_edges_endnode = calloc(1, sizeof(list_edges_t));
        list_edges_endnode->data = edge;
        list_edges_endnode->next = NULL;

        if (nodes[endnode]->head == NULL) {
            nodes[endnode]->head = list_edges_endnode;
        } else {
            nodes[endnode]->tail->next = list_edges_endnode;
        }
        nodes[endnode]->tail = list_edges_endnode;

        startnode = endnode = latency = 0;
    }

    return;
}

void init(node_t **nodes, int length_nodes)
{
    for (int i = 1; i < length_nodes ; ++i) {
        nodes[i]->dist = LONG_LONG_MAX;
        nodes[i]->visited = 0;
    }
}

typedef struct queue {
    struct queue *head;
    struct queue *next;
    struct queue *prev;
    struct queue *tail;

    int processed;

    node_t *data;
} queue_t;

void queue_push(queue_t *queue, node_t *item)
{
    if (!queue) {
        return;
    }

    if (item->visited) {
        return;
    }

    queue_t *current = (queue_t *) calloc(sizeof(queue_t), 1);
    current->next = NULL;
    current->prev = queue->tail;
    current->data = item;

    if (queue->head == NULL) {
        queue->head = current;
    } else {
        queue->tail->next = current;
    }
    queue->tail = current;
}

node_t *queue_take_min(queue_t *queue)
{
    if (!queue || queue->head == NULL) {
        return NULL;
    }

    queue_t *current = queue->head;
    queue_t *item = current;
    node_t *min = NULL;
    while (current) {
        if (current->processed) {
            current = current->next;
            continue;
        }
        if (current->data->visited == 0) {
            if (min == NULL) {
                min = current->data;
                item = current;
            } else {
                if (current->data->dist < min->dist) {
                    min = current->data;
                    item = current;
                }
            }
        }
        current = current->next;
    }

    if (item) {
        item->processed = 1;
        if (item == queue->head) {
            queue->head = item->next;
        }

        if (item == queue->tail) {
            queue->tail = item->prev;
        }

        if (item->prev) {
            item->prev->next = item->next;
        }

        if (item->next) {
            item->next->prev = item->prev;
        }
        free(item);
    }

    if (min) {
        min->visited = 1;
    }

    return min;
}

unsigned long long deikstra(node_t **nodes, int index, int length_nodes)
{
    nodes[index]->dist = 0;
    queue_t queue = {NULL, NULL, NULL, NULL, 0, NULL};

    for (int i = 1; i < length_nodes; ++i) {
        queue_push(&queue, nodes[i]);
    }

    node_t *min;
    node_t *node;
    list_edges_t *current_list_edges;
    while ((min = queue_take_min(&queue))) {
        current_list_edges = min->head;
        int count = 0;
        while (current_list_edges) {
            if (min != current_list_edges->data->endtnode) {
                node = current_list_edges->data->endtnode;
            } else {
                node = current_list_edges->data->startnode;
            }
            if ((min->dist + current_list_edges->data->latency) < node->dist) {
                node->dist = min->dist + current_list_edges->data->latency;
            }
            count++;
            current_list_edges = current_list_edges->next;
        }
    }

    unsigned long long res = 0;
    for (int i = 1; i < length_nodes; ++i) {
        if (nodes[i]->type == CLIENT && res < nodes[i]->dist) {
            res = nodes[i]->dist;
        }
    }

    return res;
};

unsigned long long implementation(node_t **nodes, int length_nodes)
{
    unsigned long long res = 0;
    unsigned long long min = 0;

    for (int i = 1; i < length_nodes ; ++i) {
        if (nodes[i]->type != ROUTE) {
            continue;
        }

        init(nodes, length_nodes);

        res = deikstra(nodes, i, length_nodes);
        if (min == 0 || min > res) {
            min = res;
        }
    }

    return min;
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;

    if (argc > 2) {
        if (argv[1] && strcasestr(argv[1], ".in")) {
            file_name_in = argv[1];
            file_name_out = argv[2];
        } else if (argv[1] && strcasestr(argv[1], ".out")) {
            file_name_in = argv[2];
            file_name_out = argv[1];
        }
    } else if (argc > 1) {
        if (argv[1] && strcasestr(argv[1], ".in")) {
            file_name_in = argv[1];
        } else if (argv[1] && strcasestr(argv[1], ".out")) {
            file_name_out = argv[1];
        }
    }

    if (!(file_in = fopen(file_name_in,"r"))) {
        printf("File IN not found '%s'\n", file_name_in);
        return 0;
    }

    node_t **nodes = NULL;
    int length_nodes;

    get_data(file_in, &nodes, &length_nodes);

#ifdef DEBUG
    for (int i = 1; i < length_nodes; ++i) {
        printf("node: %d, type: %s\n", i, nodes[i]->type == CLIENT ? "CLIENT" : nodes[i]->type == ROUTE ? "ROUTE" : "SERVER");
        list_edges_t *list = nodes[i]->head;
        while (list) {
            printf("edges: from:%d to %d latency: %lld\n", list->data->startnode->number, list->data->endtnode->number, list->data->latency);
            list = list->next;
        }
    }
#endif

    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    unsigned long long res = implementation(nodes, length_nodes);

//#ifdef DEBUG
    printf("res: %llu\n", res);
//#endif
    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%llu\n", res);

    /*Free data*/
    for (int i = 1; i < length_nodes; ++i) {
        free(nodes[i]);
    }
    free(nodes);

    fclose(file_in);
    fclose(file_out);

    return 0;
}
