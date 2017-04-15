#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG

#define FILE_IN "sigkey.in"
#define FILE_OUT "sigkey.out"

#define RADIX 10

static int etalon[27] = {0, };

struct keys {
    char key[27];
    int weight;
    int length;
};

typedef struct keys keys_t;

void init_etalon(void)
{
    int i, summ = 0;
    char letter;

    for (letter = 'a', i = 1; letter <= 'z'; letter++, i++) {
        summ += letter;
        etalon[i] = summ;
    }
}

int calculate_weight(char *key, int length)
{
    int i, ret = 0;
    for (i = 0; i < length; i++) {
        ret += key[i];
    }

    return ret;
}

void get_data(FILE *file, int *length, keys_t **keys)
{
    int i;
    fscanf(file, "%d", length);

    keys_t *data = (keys_t *)malloc(*length * sizeof(keys_t));

    for (i = 0; i < *length; i++) {
        fscanf(file, "%s", data[i].key);
        data[i].length = strlen(data[i].key);
        data[i].weight = calculate_weight(data[i].key, data[i].length);
    }

    *keys = data;
}

int check_keys(keys_t *key1, keys_t *key2)
{
    int lens = key1->length + key2->length;
    int weight = key1->weight + key2->weight;

    if (etalon[lens] == weight) {
        return 1;
    }
    return 0;
}

int calculate_result(keys_t *array, int length)
{
    int i, j;
    int res = 0;
    for (i = 0; i < length - 1; i++) {
        for (j = i + 1; j < length; j++) {
            if (check_keys(&array[i], &array[j])) {
                res++;
                break;
            }
        }
    }
    return res;
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    int length = 0;
    keys_t *array;

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

    init_etalon();
    get_data(file_in, &length, &array);


#ifdef DEBUG
    /*Debug data*/
    for (i = 0; i < C; i++) {
        printf("H[%d]: %d, G[%d]: %d\n", i, H[i].h, i, H[i].g);
    }
#endif

    int res = calculate_result(array, length);
    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%d\n", res);
//#ifdef DEBUG
    printf("res: %d\n", res);
//#endif

    /*Free data*/
    if (array) {
        free(array);
    }

    fclose(file_in);
    fclose(file_out);

    return 0;
}
