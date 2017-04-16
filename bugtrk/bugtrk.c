#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//#define DEBUG

#define FILE_IN "bugtrk.in"
#define FILE_OUT "bugtrk.out"

void get_data(FILE *file, int *N, int *W, int *H)
{
    fscanf(file, "%d %d %d", N, W, H);
    return;
}

long int calculate_result(int N, int W, int H)
{
    long int res = 0;
    int square_root = sqrt(N);

    if (N <= 2) {
        if (W > H) {
            return H * N;
        } else {
            return W * N;
        }
    }

    if (W > H) {
        res = square_root * W;
    } else {
        res = square_root * H;
    }

    return res;
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    int N = 0, W = 0, H = 0;

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

    get_data(file_in, &N, &W, &H);

#ifdef DEBUG
    /*Debug data*/
    for (i = 0; i < C; i++) {
        printf("H[%d]: %d, G[%d]: %d\n", i, H[i].h, i, H[i].g);
    }
#endif

//    print_tree(root);

    long int res = calculate_result(N, W, H);
    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%ld\n", res);
//#ifdef DEBUG
    printf("res: %ld\n", res);
//#endif

    fclose(file_in);
    fclose(file_out);

    return 0;
}
