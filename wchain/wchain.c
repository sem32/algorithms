#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG

#define FILE_IN "wchain.in"
#define FILE_OUT "wchain.out"

typedef struct wchain {
    char data[51];
    struct wchain *next;
    struct wchain *tail;
} t_wchain;

void get_data(FILE *file, int *length, t_wchain **root)
{
    int i, len;
    char data[51];
    t_wchain *current;
    fscanf(file, "%d", length);
    for (i = 0; i < *length; ++i) {
        fscanf(file, "%s", data);
        len = strlen(data);
        current = calloc(1, sizeof(t_wchain));
        strcpy(current->data, data);
        current->next = NULL;

        if (root[len] == NULL) {
            root[len] = current;
        } else {
            root[len]->tail->next = current;
        }
        root[len]->tail = current;
    }
    return;
}

char *check(char *_data, int len, t_wchain *item)
{
    if (!item) {
        return NULL;
    }
    char data[51] = {0, };
    t_wchain *current = item;

    for (int i = 1; i <= len; ++i) {
        sprintf(data, "%.*s%.*s", i - 1, _data, len - i + 1, _data + i);
        current = item;
        while (current) {
            if (!strcmp(current->data, data)) {
                return current->data;
            }
            current = current->next;
        }
    }

    return NULL;
}

int calculate_result(t_wchain **root)
{
    int i = 50;
    int res = 1;
    char *data = NULL;
    while (root[i] == NULL) {
        i--;
    }

    data = root[i]->data;
    while (i > 0 && root[i]) {
        data = check(data, strlen(data), root[i - 1]);
        if (!data) {
            data = root[i - 1]->data;
        } else {
            res++;
        };
        i--;
    }

    return res;
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    t_wchain *root[51] = {0, };
    int length;

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

    get_data(file_in, &length, root);

#ifdef DEBUG
    /*Debug data*/
    for (i = 0; i < C; i++) {
        printf("H[%d]: %d, G[%d]: %d\n", i, H[i].h, i, H[i].g);
    }
#endif

    int res = calculate_result(root);
    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%d\n", res);
//#ifdef DEBUG
    printf("res: %d\n", res);
//#endif

    fclose(file_in);
    fclose(file_out);

    return 0;
}
