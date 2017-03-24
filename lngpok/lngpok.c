#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG

#define FILE_IN "lngpok.in"
#define FILE_OUT "lngpok.out"

#define RADIX 10

void count_sort(int *array, int length, int exp)
{
    int i;
    int *hist = calloc(RADIX, sizeof(int));
    int *out = calloc(length, sizeof(int));

    for (i = 0; i < length; i++) {
        int r = (array[i] / exp) % RADIX;
        hist[r] = hist[r] + 1;
    }

    for (i = 1; i < RADIX; i++) {
        hist[i] = hist[i] + hist[i - 1];
    }

    for (i = length - 1; i >= 0; i--) {
        int r = (array[i] / exp) % RADIX;
        out[hist[r] - 1] = array[i];
        hist[r] = hist[r] - 1;
    }

    for (i = 0; i < length; i++) {
        array[i] = out[i];
    }

    free(out);
    free(hist);
}

int *sort(int *array, int length, int range)
{
	int exp;
	for (exp = 1; (range / exp) > 0; exp = exp * RADIX) {
		count_sort(array, length, exp);
	}
	return array;
}

void get_data(FILE *file, int **array, int *length)
{
    int current_length = 0, foo;
    int i;
    while ((fscanf(file, "%d", &foo) != EOF)) {
    	current_length++;
    }

    int *raw_data = (int*)malloc(current_length * sizeof(int));
    rewind(file);

    for(i = 0; i < current_length; i++) {
        fscanf(file, "%d", &raw_data[i]);
    }

    *length = current_length;
    *array = raw_data;
}

int find_sequence(int *array, int length, int start_index, int count_joker)
{
    int index = start_index;
    int i;
    int current_length = 1, current_count_joker = count_joker,
            current_value = 0, max = 0;

    if (length - start_index <= 0) {
        return current_count_joker;
    }

    for (i = start_index; i < length - 1; i++) {
        if (array[i] == array[i + 1]) {
            continue;
        }
        if ((array[i + 1] - array[i]) == 1) {
            current_length++;
        } else if (count_joker > 0 && (array[i + 1] - array[i]) > (count_joker + 1)) {
            index = i + 1;
        } else if (current_count_joker > 0) {
            current_value = array[i];
            do {
                if (index <= start_index && i > start_index) {
                    index = i;
                }
                current_count_joker--;
                current_length++;
                current_value++;
                if ((array[i + 1] - current_value) == 1) {
                    current_length++;
                    break;
                }
            } while (current_count_joker > 0);
        } else if (index > start_index) {
            max = find_sequence(array, length, index, count_joker);
            break;
        } else {
            if (current_length > max) {
                max = current_length;
            }
            current_length = 1;
        }
    }

    if (current_count_joker > 0) {
        current_length += current_count_joker;
    }

    if (current_length > max) {
        max = current_length;
    }

    return max;
}

int calculate_result(int *array, int length) {
    int count_joker = 0;
    int i;
    for (i = 0; i < length; i++) {
        if (array[i] == 0) {
            count_joker++;
        } else {
            break;
        }
    }

    int max = find_sequence(array, length, count_joker, count_joker);

    printf("%d\n", max);

    return max;
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    int *array;
    int length = 0;
    int i;

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

    get_data(file_in, &array, &length);

#ifdef DEBUG
    /*Debug data*/
    for (i = 0; i < length; i++) {
        printf("raw_data[%d]: %d\n", i, array[i]);
    }
#endif

    sort(array, length, 100000);

#ifdef DEBUG
    for (i = 0; i < length; i++) {
        printf("sorted[%d]: %d\n", i, array[i]);
    }
#endif

    int res = calculate_result(array, length);
    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%d\n", res);
#ifdef DEBUG
    printf("res: %d\n", res);
#endif

    /*Free data*/
    if (array) {
        free(array);
    }
    fclose(file_in);
    fclose(file_out);

    return 0;
}
