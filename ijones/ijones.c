#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG

#define FILE_IN "ijones.in"
#define FILE_OUT "ijones.out"

typedef struct plate {
    char letter;
    int length;
} t_plate;

void get_data(FILE *file, int *length_i, int *length_j, t_plate ***_root)
{
    int i, j;
    char *data = NULL;
    fscanf(file, "%d %d", length_j, length_i);

    t_plate **root = calloc((*length_j), sizeof(t_plate *));
    *_root = root;

    for (i = 0; i < *length_i; i++) {
        data = calloc(*length_j, 1);
        fscanf(file, "%s", data);

        root[i] = calloc(*length_i, sizeof(t_plate));

        for (j = 0; j < *length_j; j++) {
            root[i][j].letter = data[j];
            root[i][j].length = 0;
        }
        free(data);
    }

    return;
}

int calculate_result(t_plate **root, int len_i, int len_j)
{
    for (int j = 0; j < len_j; ++j) {
        for (int i = 0; i < len_i; ++i) {
            int i_jm2 = (j >= 2 ? root[i][j - 2].letter == root[i][j].letter ? root[i][j - 2].length : 0 : 0);
            int i_jm1 = (j >= 1 ? root[i][j - 1].length : 0);
            int im1_jm1 = (j >= 1 && i >= 1 ? root[i - 1][j - 1].letter == root[i][j].letter ? root[i - 1][j - 1].length : 0 : 0);
            int im2_jm2 = (j >= 2 && i >= 2 ? root[i - 2][j - 2].letter == root[i][j].letter ? root[i - 2][j - 2].length : 0 : 0);
            int ip1_jm1 = (j >= 1 && i + 1 < len_i ? root[i + 1][j - 1].letter == root[i][j].letter ? root[i + 1][j - 1].length : 0 : 0);
            int ip2_jm2 = (j >= 2 && i + 2 < len_i ? root[i + 2][j - 2].letter == root[i][j].letter ? root[i + 2][j - 2].length : 0 : 0);
            if (j == 0) {
                root[i][j].length = 1;
            } else {
                root[i][j].length = i_jm2 + i_jm1 + im1_jm1 + im2_jm2 + ip1_jm1 + ip2_jm2;
            }

#ifdef DEBUG
            printf("root[%d][%d]: %c, length: %d i_jm2: %d (%c), i_jm1: %d (%c), im1_jm1: %d (%c), im2_jm2: %d (%c), ip1_jm1: %d (%c), ip2_jm2: %d (%c)\n",
                   i, j, root[i][j].letter, root[i][j].length,
                   i_jm2, j >= 2 ? root[i][j - 2].letter : 'X', i_jm1, j >= 1 ? root[i][j - 1].letter : 'X', im1_jm1, j >= 1 && i >= 1 ? root[i-1][j-1].letter : 'X',
                   im2_jm2, j >= 2 && i >= 2 ? root[i -2][j - 2].letter : 'X', ip1_jm1, j >= 1 && i+1 < len_i ? root[i + 1][j - 1].letter : 'X',
                   ip2_jm2, j >= 2 && i+2 < len_i ? root[i + 2][j - 2].letter : 'X');
#endif
        }
    }

    return root[(len_i-1)][(len_j-1)].length + root[0][len_j-1].length;
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
    for (int j = 0; j < len_j; ++j) {
        for (int i = 0; i < len_i; ++i) {
            printf("root[%d][%d]: %c\n", i, j, root[i][j].letter);
        }
    }
#endif

    int res = calculate_result(root, length_i, length_j);
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
