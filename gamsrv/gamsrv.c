#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//#define DEBUG

#define FILE_IN "gamsrv.in"
#define FILE_OUT "gamsrv.out"

struct went;

typedef enum NODE_TYPE {
    ROUTE,
    CLIENT,
    SERVER
} node_t;

typedef struct edge {
    int latency;
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

    node_t type;
} went_t;

void set_array(char *data, went_t **ret, int **clients)
{
    int length_str, i, client_index = 0;
    int number = 0, counter = 0;
    went_t *array;

    if (!data || !ret) {
        return;
    }

    length_str = strlen(data);

    for (i = 0; i < length_str; i++) {
        if (data[i] == ' ' || data[i] == '\n') {
            counter++;
        }
    }

    array = *ret;
    for (i = 0; i < length_str; i++) {
        if (data[i] == ' ' || data[i] == '\n') {
            if (counter > 0) {
                array[number].type = CLIENT;
                clients[++client_index] = number;
                number = 0;
                counter = 0;
            }
        } else {
            counter++;
            number = number * 10 + (data[i] - '0');
        }
    }
    if (counter > 0) {
        array[number].type = CLIENT;
        clients[++client_index] = number;
    }
    return;
}

void get_data(FILE *file, went_t **_nodes, edge_t **_edges, int *length_nodes, int *length_edges, int **clients)
{
    char *line = NULL;
    size_t n = 0;
    went_t *nodes = *_nodes;
    edge_t *edges = *_edges;
    int startnode = 0, endnode = 0, latency = 0;

    fscanf(file, "%d %d\n", length_nodes, length_edges);

    nodes = calloc(*length_nodes, sizeof(went_t));
    edges = calloc(*length_edges, sizeof(edge_t));
    clients = calloc(*length_nodes, sizeof(int));;

    if (getline(&line, &n, file) > 0) {
        set_array(line, &nodes, &clients);
        if (line) {
            free(line);
            line = NULL;
        }
    }

    int index_eges = 0;
    while ((fscanf(file, "%d %d %d", &startnode, &endnode, &latency) != EOF)) {
        edges[index_eges].startnode = &nodes[startnode];
        edges[index_eges].endtnode = &nodes[endnode];
        edges[index_eges].latency = latency;

        if (nodes[startnode].head == NULL) {
            nodes[startnode].head = calloc(1, sizeof(list_edges_t));
            nodes[startnode].head->data = &edges[index_eges];
            nodes[startnode].head->next= NULL;
            nodes[startnode].tail = nodes[startnode].head;
        } else {
            nodes[startnode].tail->next = calloc(1, sizeof(list_edges_t));
            nodes[startnode].tail->next->data = &edges[index_eges];
            nodes[startnode].head->next->next= NULL;
            nodes[startnode].tail = nodes[startnode].tail->next;
        }

        index_eges++;
    }

    return;
}

int getCut(went_t *nodes, int vertexCount)
{
    went_t *G = nodes;
    int edgeCount = 0;
    while (vertexCount > 2) {
        edge = случайное ребро из G
        contract(edge);
        vertexCount--;
        edgeCount = количество ребер в G
    }
    return edgeCount;
}


int maxL(went_t *nodes, edge_t *edges, int length_nodes, int length_edges)
{
    int answer = 0;
    int i, curCut;
    for (i = 1; i < length_nodes; i++) {
        curCut = getCut(nodes, length_nodes);
        if (curCut < answer || answer == 0) {
            answer = curCut;
        }
    }
    return answer;
}

void implementation(went_t *_nodes, edge_t *_edges, int length_nodes, int length_edges, int *clients)
{
    int i;


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

    went_t *nodes = NULL;
    edge_t *edges = NULL;
    int *clients = NULL;
    int length_nodes, length_edges;

    get_data(file_in, &nodes, &edges, &length_nodes, &length_edges, &clients);

#ifdef DEBUG
    /*Debug data*/
    for (i = 0; i < C; i++) {
        printf("H[%d]: %d, G[%d]: %d\n", i, H[i].h, i, H[i].g);
    }
#endif

//    print_tree(root);

    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    implementation(nodes, edges, length_nodes, length_edges, clients);

//#ifdef DEBUG
//    printf("res: %d\n", res);
//#endif

    /*Free data*/
    //

    fclose(file_in);
    fclose(file_out);

    return 0;
}
