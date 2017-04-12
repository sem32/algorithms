#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG

#define FILE_IN "hamstr.in"
#define FILE_OUT "hamstr.out"

#define RADIX 10

struct hamster {
    int g;
    int h;
};

typedef struct hamster hamster_t;

void count_sort(hamster_t *H, int length, int exp)
{
    int i;
    int *hist = calloc(RADIX, sizeof(int));
    hamster_t *out = calloc(length, sizeof(hamster_t *));

    for (i = 0; i < length; i++) {
        int r = (H[i].g / exp) % RADIX;
        hist[r] = hist[r] + 1;
    }

    for (i = 1; i < RADIX; i++) {
        hist[i] = hist[i] + hist[i - 1];
    }

    for (i = length - 1; i >= 0; i--) {
        int r = (H[i].g / exp) % RADIX;
        out[hist[r] - 1] = H[i];
        hist[r] = hist[r] - 1;
    }

    for (i = 0; i < length; i++) {
        H[i] = out[i];
    }

    free(out);
    free(hist);
}

void sort(hamster_t *H, int length, int range)
{
	int exp;
	for (exp = 1; (range / exp) > 0; exp = exp * RADIX) {
		count_sort(H, length, exp);
	}
	return;
}

void get_data(FILE *file, int *S, int *C, hamster_t **H)
{
    int i;
    fscanf(file, "%d", S);
    fscanf(file, "%d", C);

    hamster_t *h_data = (hamster_t *)malloc(*C * sizeof(hamster_t));

    for (i = 0; i < *C; i++) {
        fscanf(file, "%d %d", &h_data[i].h, &h_data[i].g);
    }

    *H = h_data;
}

int calculate_result(hamster_t *H, int length, int food)
{
    int res = 0;
    int k = 0;

    while(1) {

        food = food - H[res].h;
        if (res > 0) {
            for (k = 0; k < res; k++) {
                food = food - H[k].g;
                if (food < 0) {
                    break;
                }
            }
        }

        res++;

        if (food < 0) {
            break;
        }
    }

    return res - (food < 0 ? 1 : 0);
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    int i;
    int S = 0, C = 0;
    hamster_t *H;

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

    get_data(file_in, &S, &C, &H);

#ifdef DEBUG
    /*Debug data*/
    for (i = 0; i < C; i++) {
        printf("H[%d]: %d, G[%d]: %d\n", i, H[i].h, i, H[i].g);
    }
#endif

    sort(H, C, 1000000000);


#ifdef DEBUG
    for (i = 0; i < C; i++) {
        printf("Sorted H[%d]: %d, G[%d]: %d\n", i, H[i].h, i, H[i].g);
    }
#endif

    int res = calculate_result(H, C, S);
    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%d\n", res);
//#ifdef DEBUG
    printf("res: %d\n", res);
//#endif

    /*Free data*/
    if (H) {
        free(H);
    }

    fclose(file_in);
    fclose(file_out);

    return 0;
}
