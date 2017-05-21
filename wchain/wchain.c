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

t_wchain *check(t_wchain *root, t_wchain *item,  t_wchain *next_item)
{
    if (!item || !root) {
        return NULL;
    }
    char data[51] = {0, };
    t_wchain *current = NULL;
    t_wchain *res = NULL;
    t_wchain *current_root = root;


    while (current_root) {
        int len = strlen(current_root->data);

        for (int i = 1; i <= len; ++i) {
            sprintf(data, "%.*s%.*s", i - 1, current_root->data, len - i + 1, current_root->data + i);
            current = item;
            while (current) {
                if (!strcmp(current->data, data)) {
                    res = current;
                    if (next_item) {
                        if (check(current, next_item, NULL)) {
                            res = current;
                            goto end;
                        }
                    } else {
                        res = current;
                        goto end;
                    }
                }
                current = current->next;
            }
        }
        if (next_item) {
            break;
        }
        current_root = current_root->next;
    }

end:
    return res;
}

int calculate_result(t_wchain **root)
{
    int i = 50;
    int res = 1, max = 0;
    t_wchain *current;
    while (root[i] == NULL) {
        i--;
    }

    current = root[i];

    while (i > 0) {
        if (root[i] == NULL) {
            i--;
            if (res > max) {
                max = res;
            }
//            res = 0;
            continue;
        }
        if (!current) {
            current = root[i];
        }
        current = check(current, root[i - 1], i >= 2 ? root[i - 2] : NULL);
        if (!current) {
            if (root[i - 1]) {
                current = root[i - 1];
            }
            if (res > max) {
                max = res;
            }
//            res = 0;
        } else {
            res++;
        };
        i--;
    }

    if (res > max) {
        max = res;
    }

    return max;
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
