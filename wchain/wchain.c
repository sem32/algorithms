#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG

#define FILE_IN "wchain.in"
#define FILE_OUT "wchain.out"

#define WORLD_LENGTH 50 + 2

typedef struct combination {
    char data[WORLD_LENGTH];
    struct combination *next;
    struct combination *tail;
} t_combination;

typedef struct wchain {
    char data[WORLD_LENGTH];
    t_combination *combination;
    struct wchain *next;
    struct wchain *tail;
    int counter;
} t_wchain;

void get_data(FILE *file, int *length, t_wchain **root)
{
    int i, len;
    char data[WORLD_LENGTH];
    t_wchain *current;
    t_combination *current_combination = NULL;
    fscanf(file, "%d", length);
    for (i = 0; i < *length; ++i) {
        fscanf(file, "%s", data);
        len = strlen(data);
        current = calloc(1, sizeof(t_wchain));
        strcpy(current->data, data);
        current->next = NULL;

        for (int i = 1; i <= len; ++i) {
            current_combination = calloc(1, sizeof(*current_combination));

            if (current->combination == NULL) {
                current->combination = current_combination;
                current->combination->tail = current_combination;
                current->combination->next = NULL;
            } else {
                current->combination->tail->next = current_combination;
                current->combination->tail = current_combination;
            }

            sprintf(current_combination->data, "%.*s%.*s", i - 1, data, len - i + 1, data + i);
        }

        if (root[len] == NULL) {
            root[len] = current;
        } else {
            root[len]->tail->next = current;
        }
        root[len]->tail = current;
    }
    return;
}

#if 0
t_wchain *check(t_wchain *root, t_wchain *item, t_wchain *next_item)
{
    if (!item || !root) {
        return NULL;
    }
    char data[WORLD_LENGTH] = {0, };
    t_wchain *current = NULL;
    t_wchain *res = NULL;
    t_wchain *current_root = root;
    int len;


    while (current_root) {
        len = strlen(current_root->data);

        for (int i = 1; i <= len; ++i) {
            sprintf(data, "%.*s%.*s", i - 1, current_root->data, len - i + 1, current_root->data + i);
            current = item;
            while (current) {
                if (!strcmp(current->data, data)) {
                    printf("FOUND current: %s, check_next: %s, next_item: %p\n", data, next_item ? "YES" : "NO", next_item);
                    if (next_item) {
                        if (check(current, next_item, NULL)) {
                            res = current;
                            goto end;
                        } else {
                            res = current;
                            goto next;
                        }
                    } else {
                        res = current;
                        goto end;
                    }
                }
                current = current->next;
            }
        }
    next:
        if (!next_item) {
            break;
        }
        current_root = current_root->next;
    }

end:
    return res;
}

int calculate_result(t_wchain **root)
{
    int i = WORLD_LENGTH - 1;
    int res = 1;
    t_wchain *current;
    int max = 0;
    while (root[i] == NULL) {
        i--;
    }

    current = root[i];

    while (i > 0) {
        if (root[i] == NULL) {
            printf("(2) Clear res: %d\n", res);
            if (max < res) {
                max = res;
            }
            res = 1;
            i--;
            continue;
        }
        if (!current) {
            printf("(1) Clear res: %d\n", res);
            if (max < res) {
                max = res;
            }
            res = 1;
            current = root[i];
        }
        if ((current = check(current, root[i - 1], res > 1 ? NULL : (i > 2 ? root[i - 2] : NULL)))) {
            res++;
            printf("Incriment res: %d\n", res);
        };
        i--;
    }

    if (max < res) {
        max = res;
    }

    return max;
}
#endif

static inline int find(char *src, char *dst)
{
    int sum_src = 0;
    int sum_dst = 0;

    for (int i = 0; i < strlen(src); ++i) {
        sum_src += src[i];
    }
    for (int j = 0; j < strlen(dst); ++j) {
        sum_dst += dst[j];
    }

    int letter = sum_dst - sum_src;

    char data[2];
    sprintf(data, "%c", letter);

    if ((letter <= 'z' || letter >= 'a') && strstr(dst, data)) {
//        printf("!! src: %s, dst: %s, letter: %d (%c)\n", src, dst, letter, letter);
        return 1;
    }

    return 0;
}

int calculate_result(t_wchain **root)
{
    int res = 0;
    t_wchain *current;
    t_wchain *next;
    t_combination *combination = NULL;
    int max = 0;

    for (int i = 1; i < WORLD_LENGTH - 1; i++) {
        if ((current = root[i]) == NULL) {
            continue;
        }

        while (current) {
            next = root[i + 1];

            while (next) {

                if (!find(current->data, next->data)) {
//                    printf("Skip %s VS %s\n", current->data, next->data);
                    next = next->next;
                    continue;
                }
//                printf("current: %s, next %s, i: %d\n", current->data, next->data, i);
                combination = next->combination;
                while (combination) {
                    if (!strcmp(combination->data, current->data)) {
//                        printf("\tCompare %s vs %s\n", combination->data, current->data);
                        next->counter = current->counter + 1;
                        if (res < next->counter) {
                            res = next->counter;
                        }
//                        printf("\tcounter: %d for %s\n", next->counter, next->data);
                    } else {
//                        printf("\tDon't Compare %s vs %s\n", combination->data, current->data);
                    }
                    combination = combination->next;
                }
                next = next->next;
            }

            if (res < current->counter) {
                res = current->counter;
            }



            current = current->next;
        }
    }

    return res + 1;
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    t_wchain *root[WORLD_LENGTH] = {0, };
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

//#ifdef DEBUG
    /*Debug data*/
    t_wchain *current = NULL;
    for (int i = WORLD_LENGTH - 1; i >= 1; i--) {
        current = root[i];
        while (current) {
//            printf("data: %s\t\t%d\n", current->data, i);
            current = current->next;
        }
    }
//#endif

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
