#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG

#define FILE_IN "ijones.in"
#define FILE_OUT "ijones.out"

typedef struct plate {
    char letter;
    unsigned long length;
} t_plate;

void get_data(FILE *file, int *length_i, int *length_j, t_plate ***_root)
{
    int i, j;
    char *data = NULL;
    fscanf(file, "%d %d", length_j, length_i);

    t_plate **root = calloc((*length_i), sizeof(t_plate *));
    *_root = root;

    for (i = 0; i < *length_i; i++) {
        data = calloc(*length_j, 1);
        fscanf(file, "%s", data);

        root[i] = calloc(*length_j, sizeof(t_plate));

        for (j = 0; j < *length_j; j++) {
            root[i][j].letter = data[j];
            root[i][j].length = 0;
        }
        free(data);
    }
    return;
}

unsigned long calculate_result(t_plate **root, int len_i, int len_j)
{
    int i, j;
    unsigned long map[26] = {0, };
    unsigned long *del_map = calloc(len_i, sizeof(unsigned long));

    for (j = 0; j < len_j; j++) {
        for (i = 0; i < len_i; i++) {
            unsigned long i_jm1 = (j >= 1 ? root[i][j - 1].length : 0);


//            printf("root[%d][%d]: %c %lu, i_jm1: %lu, map[%c]: %lu, del_map[%d]: %lu\n", i, j, root[i][j].letter, root[i][j].length, i_jm1, root[i][j].letter, map['z' - root[i][j].letter], i, del_map[i]);

            if (j >= 1  && root[i][j-1].letter == root[i][j].letter) {
                del_map[i] = root[i][j-1].length;
            } else {
                del_map[i] = 0;
            }

            if (j == 0) {
                root[i][j].length = 1;
            } else {
                root[i][j].length = i_jm1 + map['z' - root[i][j].letter] - del_map[i];
            }
        }
        for (i = 0; i < len_i; i++) {
            map['z' - root[i][j].letter] += root[i][j].length;
        }
    }
    return (len_i > 1 ? root[(len_i-1)][(len_j-1)].length : 0) + root[0][len_j-1].length;
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    t_plate **root;
    int length_i, length_j;

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

    get_data(file_in, &length_i, &length_j, &root);

#ifdef DEBUG
    /*Debug data*/
    for (int j = 0; j < length_j; ++j) {
        for (int i = 0; i < length_i; ++i) {
            printf("root[%d][%d]: %c\n", i, j, root[i][j].letter);
        }
    }
#endif

    unsigned long res = calculate_result(root, length_i, length_j);
    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%lu\n", res);
//#ifdef DEBUG
    printf("res: %lu\n", res);
//#endif

    fclose(file_in);
    fclose(file_out);

    return 0;
}
