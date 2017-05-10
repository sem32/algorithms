#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//#define DEBUG

#define FILE_IN "govern.in"
#define FILE_OUT "govern.out"

typedef struct edge {
    char name[51];
    struct edge **to;
    struct edge **from;
    int allocated_to;
    int allocated_from;
    int length_to;
    int length_from;

    struct edge *next;

    int added;
} edge_t;

edge_t *find_edge(edge_t *root, char *name)
{
    edge_t *current = root;

    while(current) {
        if (!strcmp(current->name, name)) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

edge_t *create_new_edge(char *name)
{
    edge_t *item = (edge_t *)calloc(sizeof(edge_t), 1);
    strncpy(item->name, name, strlen(name));
    item->to = NULL;
    item->from = NULL;
    item->length_to = item->length_from = item->allocated_to = item->allocated_from = item->added = 0;

    return item;
}

void add_edges(edge_t *parent, edge_t *element) {
    if (!element || !parent) {
        return;
    }
    if (parent->from == NULL) {
        parent->allocated_from = 10;
        parent->from = (edge_t **)calloc(parent->allocated_from, sizeof(edge_t *));
        memset(parent->from, 0, parent->allocated_from * sizeof(edge_t *));
    } else {
        if ((parent->length_from + 1) >= parent->allocated_from) {
            parent->allocated_from += 10;
            parent->from = (edge_t **) realloc(parent->from, parent->allocated_from * sizeof(edge_t *));
            memset(parent->from + ((parent->allocated_from - 10) * sizeof(edge_t *)), 0, parent->allocated_from * sizeof(edge_t *));
        }
    }
    parent->from[parent->length_from] = element;
    parent->length_from++;

    if (element->to == NULL) {
        element->allocated_to = 10;
        element->to = (edge_t **)calloc(element->allocated_to, sizeof(edge_t *));
        memset(element->to, 0, element->allocated_to * sizeof(edge_t *));
    } else {
        if ((element->length_to + 1) >= element->allocated_to) {
            element->allocated_to += 10;
            element->to = (edge_t **) realloc(element->to, element->allocated_to * sizeof(edge_t *));
            memset(element->to + ((element->allocated_to - 10) * sizeof(edge_t *)), 0, element->allocated_to * sizeof(edge_t *));
        }
    }
    element->to[element->length_to] = parent;
    element->length_to++;
}

void add_to_root(edge_t **root, edge_t *item)
{
    if (!root) {
        return;
    }
    if (!(*root)) {
        *root = item;
        return;
    }

    edge_t *current = *root;
    while (current->next) {
        current = current->next;
    }
    current->next = item;
}

void get_data(FILE *file, edge_t **_root)
{
    edge_t *item1 = NULL, *item2 = NULL;
    char name1[51] = {0, }, name2[51] = {0,};
    edge_t *root = NULL;

    while (fscanf(file, "%s %s\n", name1, name2) != EOF) {
        item1 = find_edge(root, name1);
        if (!item1) {
            item1 = create_new_edge(name1);
            add_to_root(&root, item1);
        }

        item2 = find_edge(root, name2);

        if (!item2) {
            item2 = create_new_edge(name2);
            add_to_root(&root, item2);
        }

        add_edges(item1, item2);
    }
    *_root = root;

    return;
}

#ifdef DEBUG
void print(edge_t *current)
{
    while (current) {
        printf("name: %s, to: %d, from: %d", current->name, current->length_to, current->length_from);
        if (current->length_to > 0) {
            printf(" (");
            for (int i = 0; i < current->length_to; ++i) {
                printf(" ");
                printf("%s", current->to[i]->name);
            }
            printf(" )");
        }
        if (current->length_from > 0) {
            printf(" (");
            for (int i = 0; i < current->length_from; ++i) {
                printf(" ");
                printf("%s", current->from[i]->name);
            }
            printf(" )");
        }
        printf("\n");
        current = current->next;
    }
}
#endif

typedef struct queue {
    struct queue *head;
    struct queue *next;
    struct queue *tail;

    edge_t *data;
} queue_t;

void queue_push(queue_t *queue, queue_t *posponed_queue, edge_t *item, edge_t **owner, int len)
{
    if (!queue && !posponed_queue) {
        return;
    }
    if (!queue) {
        queue = posponed_queue;
    }
    queue_t *current = queue->head;
    int have = 0;

    if (len > 0) {
        for (int i = 0; i < len; ++i) {
            if (owner[i]->added) {
                have++;
                break;
            }
        }
        while (current && !have) {
            for (int i = 0; i < len; ++i) {
                if (current->data == owner[i]) {
                    have++;
                    break;
                }
            }
            current = current->next;
        }
        if (have != len) {
            have = 0;
        }
    } else {
        have = 1;
    }

    if (!have) {
        queue_push(NULL, posponed_queue, item, NULL, 0);
        return;
    }
    current = (queue_t *) calloc(sizeof(queue_t), 1);
    current->next = NULL;
    current->data = item;

    if (queue->head == NULL) {
        queue->head = current;
    } else {
        queue->tail->next = current;
    }
    queue->tail = current;
}

edge_t *queue_pop(queue_t *queue)
{
    if (!queue) {
        return NULL;
    }

    if (queue->head == NULL) {
        return NULL;
    }

    queue_t *current = queue->head;
    edge_t *ret = current->data;
    queue->head = queue->head->next;

    free(current);

    return ret;
}

void implementation(edge_t *root, FILE *file)
{
    queue_t queue = {0 };
    queue_t posponed_queue = {0 };
    edge_t *current = root;

    // Add to queue single vertex
    while (current) {
        if (current->length_from == 0) {
            queue_push(&queue, NULL, current, NULL, 0);
        }
        current = current->next;
    }

    while ((current = queue_pop(&queue))) {
        if (current->added == 0) {
            fprintf(file, "%s\n", current->name);
            current->added = 1;
        }
        for (int i = 0; i < current->length_to; ++i) {
            queue_push(&queue, &posponed_queue, current->to[i], current->to[i]->from, current->to[i]->length_from);
        }
    }

    while ((current = queue_pop(&posponed_queue))) {
        if (current->added == 0) {
            fprintf(file, "%s\n", current->name);
            current->added = 1;
        }
        for (int i = 0; i < current->length_to; ++i) {
            queue_push(&queue, &posponed_queue, current->to[i], current->to[i]->from, current->to[i]->length_from);
        }
    }
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    edge_t *root = NULL;

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

    get_data(file_in, &root);

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

    implementation(root, file_out);

//#ifdef DEBUG
//    printf("res: %d\n", res);
//#endif

    /*Free data*/
    //

    fclose(file_in);
    fclose(file_out);

    return 0;
}
