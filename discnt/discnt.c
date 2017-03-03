#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define INSERT_SORT
//#define DEBUG
//#define GETLINE

#define FILE_IN "discnt.in"
#define FILE_OUT "discnt.out"

#ifdef GETLINE
void get_array(char *data, int **ret, int *length)
{
    int length_str, i;
    int number = 0;
    int counter = 0, index = 0;
    int *array;

    if (!data || !ret || !length) {
        return;
    }

    length_str = strlen(data);

    for (i = 0; i < length_str; i++) {
        if (data[i] == ' ' || data[i] == '\n') {
            counter++;
        }
    }

    *length = counter;
    *ret = (int *)malloc(counter * sizeof(int));
    array = *ret;

    for (i = 0; i < length_str; i++) {
        if (data[i] == ' ' || data[i] == '\n') {
            if (counter > 0) {
                array[index++] = number;
                number = 0;
                counter = 0;
            }
        } else {
            counter++;
            number = number * 10 + (data[i] - '0');
        }
    }
    if (counter > 0) {
        array[index++] = number;
        number = 0;
        counter = 0;
    };
    return;
}
#endif

int compare(int a, int b)
{
    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    }
    return 0;
}

void shift(int *array, int length, int index, int element)
{
    int i;
    for (i = length; i > index; i--) {
        array[i] = array[i-1];
    }
    array[index] = element;
}

#ifdef INSERT_SORT
int *sort(int *array, int length)
{
    int mid, sorted_length, index, i, j, res, flag;
    int *sorted = (int *)calloc(length, sizeof(int));

    if (!sorted) {
        fprintf(stderr, "No memory\n");
        return NULL;
    }

    sorted[0] = array[0];

    for (i = 1; i < length; i++) {
        flag = 0;
        for (j = 0; j < i; j++) {
            res = compare(array[i], sorted[j]);
            if (res < 0) {
                shift(sorted, i, j, array[i]);
#ifdef DEBUG
                printf("-----------------\n");
                int k;
                for (k = 0; k < i; k++) {
                    printf("sorted[%d]: %d\n", k, sorted[k]);
                }
#endif
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            sorted[i] = array[i];
        }
    }

    return sorted;
}
#else

int search_index(int *array, int left, int right, int element, int length)
{
    int mid, res;

    if ((right - left == 1) || right == left) {
#ifdef DEBUG
        printf("-----------------\n");
        printf("index: %d, left: %d, right: %d, element: %d\n", left, left, right, element);
        int k;
        for (k = 0; k < length; k++) {
            printf("sorted[%d]: %d\n", k, array[k]);
        }
#endif
        res = compare(element, array[left]);
        if (res <= 0) {
            return left;
        } else {
            return right;
        }
    }
    mid = (left + right) / 2;

    res = compare(element, array[mid]);
#ifdef DEBUG
    printf("=====================\n");
    int k;
    for (k = 0; k < length; k++) {
        printf("array[%d]: %d\n", k, array[k]);
    }
    printf("try %s, left: %d, right: %d, mid: %d, element: %d, array[%d]: %d\n", res > 0 ? "right" : "left", left, right, mid, element, mid, array[mid]);
#endif
    if (res < 0) {
        return search_index(array, left, mid, element, length);
    } else if (res > 0) {
        return search_index(array, mid, right, element, length);
    }

    return mid;
}

int *sort(int *array, int length)
{
    int index, i;
    int *sorted = (int *)calloc(length, sizeof(int));

    if (!sorted) {
        fprintf(stderr, "No memory\n");
        return NULL;
    }

    sorted[0] = array[0];

    for (i = 1; i < length; i++) {
        index = search_index(sorted, 0, i, array[i], length);
        shift(sorted, i, index, array[i]);
    }

    return sorted;
}
#endif

float calculate_best_summ(int *price, int length, int discount)
{
    float res = 0;
    int i;
    int count_purchase = length / 3;
    float koef = (100 - (float)discount) / 100;

    for (i = (length-count_purchase); i < length; i++) {
        res += price[i];
    }
    res = res * koef;

    for (i = 0; i < (length-count_purchase); i++) {
        res += price[i];
    }

    return res;
}

void get_data(FILE *file, int **array, int *length, int *discount)
{
    int price_length = 0;
    int i;
    while ((fscanf(file, "%d", discount) != EOF)) {
        price_length++;
    }
    // Decrement for minus last discount element
    price_length--;
    int *price = (int*)malloc(price_length * sizeof(int));
    rewind(file);

    for(i = 0; i < price_length; i++) {
        fscanf(file, "%d", &price[i]);
    }

    *length = price_length;
    *array = price;
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    int i;
    char *line = NULL;
    size_t n = 0;
    int *price, *sorted_price;
    int discount;
    int price_length = 0;

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

#ifdef GETLINE
    /*Get integer array 'price' from .in file*/
    if (getline(&line, &n, file_in) > 0) {
        get_array(line, &price, &price_length);
        if (line) {
            free(line);
            line = NULL;
        }
    }

    /*Get integer 'discount' from .in file*/
    if (getline(&line, &n, file_in) > 0) {
        discount = atoi(line);
        if (line) {
            free(line);
            line = NULL;
        }
    }
#else
    get_data(file_in, &price, &price_length, &discount);
#endif

#ifdef DEBUG
    /*Debug data*/
    for (i = 0; i < price_length; i++) {
        printf("price[%d]: %d\n", i, price[i]);
    }
    printf("Discount: %d\n", discount);
#endif

#ifdef INSERT_SORT
    sorted_price = sort(price, price_length);
#else
    sorted_price = sort(price, price_length);
#endif

#ifdef DEBUG
    for (i = 0; i < price_length; i++) {
        printf("sorted_price[%d]: %d\n", i, sorted_price[i]);
    }
#endif

    float res = calculate_best_summ(sorted_price, price_length, discount);

    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%.2f\n", res);
#ifdef DEBUG
    printf("res: %.2f\n", res);
#endif

    /*Free data*/
    if (price) {
        free(price);
    }
    if (sorted_price) {
        free(sorted_price);
    }
    fclose(file_in);
    fclose(file_out);

    return 0;
}
