#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

//#define DEBUG

#define FILE_IN "ijones.in"
#define FILE_OUT "ijones.out"

#define SIZE_ALPHABET ('z' - 'a' + 1)

/************************************************************************************************************/
// We'll represent big integers using 64-bit blocks in little-endian order:
//     [least significant 18 decimal digits] [more significant 18] ... [most significant 18]
//
// For example, 12945734322108635523 will be represented as:
//     [945734322108635523] [000000000000000012] ... [000000000000000000]

typedef uint64_t BLOCK;

typedef struct {
    uint16_t length;
    BLOCK *blocks;
} BIGINT;

const BLOCK MAX_BLOCK_VALUE = 1000000000000000000;
const char * BLOCK_FORMAT_SPEC = "%" PRId64;
const char * BLOCK_ZERO_PADDED_FORMAT_SPEC = "%018" PRId64;

// 380 * 18 = 6840 digits should be enough for our problem.
const uint16_t BIGINT_MAX_BLOCKS = 380;

void bigFree(BIGINT *value) {
    value->length = 0;
    if (value->blocks) {
        free(value->blocks);
    }
}

void bigAllocate(BIGINT *value, BLOCK data) {
    value->length = 1;
    value->blocks = (BLOCK *)calloc(BIGINT_MAX_BLOCKS, sizeof(BLOCK));
    value->blocks[0] = data;
}

void bigIncrement(BIGINT *base, BIGINT *increment) {
    BLOCK carry = 0;
    int i = 0;

    // Add common blocks together.
    for (i = 0; i < base->length && i < increment->length; i++) {
        BLOCK sum = base->blocks[i] + increment->blocks[i] + carry;
        base->blocks[i] = sum % MAX_BLOCK_VALUE;
        carry = sum / MAX_BLOCK_VALUE;
    }
    // Add remaining base blocks, if any.
    while (i < base->length) {
        BLOCK sum = base->blocks[i] + carry;
        base->blocks[i] = sum % MAX_BLOCK_VALUE;
        carry = sum / MAX_BLOCK_VALUE;
        i++;
    }
    // Otherwise, add remaining increment blocks, if any.
    while (i < increment->length) {
        BLOCK sum = increment->blocks[i] + carry;
        base->length++;
        base->blocks[i] = sum % MAX_BLOCK_VALUE;
        carry = sum / MAX_BLOCK_VALUE;
        i++;
    }

    // If there's something we still need to carry over, let's add one more block.
    if (carry > 0) {
        base->blocks[base->length] = carry;
        base->length++;
    }
}

void bigDecrement(BIGINT *base, BIGINT *decrement) {
    BLOCK carry = 0;
    int i = 0;

    // Add common blocks together.
    for (i = 0; i < decrement->length; i++) {
        if (decrement->blocks[i] > base->blocks[i]) {
            base->blocks[i] = MAX_BLOCK_VALUE + base->blocks[i] - decrement->blocks[i] - carry;
            carry = 1;
        } else {
            base->blocks[i] = base->blocks[i] - decrement->blocks[i] - carry;
            carry = 0;
        }
    }
}

void bigCopy(BIGINT *dest, BIGINT *src) {
    for (int i = 0; i < src->length; i++) {
        dest->blocks[i] = src->blocks[i];
    }
    dest->length = src->length;
}

void bigFprint(FILE *outputFile, BIGINT *value) {
    fprintf(outputFile, BLOCK_FORMAT_SPEC, value->blocks[value->length - 1]);
    for (int i = value->length - 2; i >= 0; i--) {
        fprintf(outputFile, BLOCK_ZERO_PADDED_FORMAT_SPEC, value->blocks[i]);
    }
}

char *bigPrint(BIGINT *value) {
    char *data = calloc(18, value->length);
    sprintf(data, BLOCK_FORMAT_SPEC, value->blocks[value->length - 1]);
    for (int i = value->length - 2; i >= 0; i--) {
        sprintf(data + strlen(data), BLOCK_ZERO_PADDED_FORMAT_SPEC, value->blocks[i]);
    }
    return data;
}
/************************************************************************************************************/


typedef struct plate {
    char letter;
    BIGINT size;
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
        }
        free(data);
    }
    return;
}

char *calculate_result(t_plate **root, int len_i, int len_j)
{
    int i, j;
    BIGINT map[SIZE_ALPHABET] = {0, };
    for (i = 0; i < SIZE_ALPHABET; i++) {
        bigAllocate(&map[i], 0);
    }

    for (j = 0; j < len_j; j++) {
        for (i = 0; i < len_i; i++) {
            if (j == 0) {
                bigAllocate(&root[i][j].size, 1);
            } else {
                bigAllocate(&root[i][j].size, 0);
                if (j >= 1 && root[i][j-1].letter != root[i][j].letter) {
                    bigIncrement(&root[i][j].size, &root[i][j-1].size);
                }
                bigIncrement(&root[i][j].size, &map['z' - root[i][j].letter]);
            }
        }
        for (i = 0; i < len_i; i++) {
            bigIncrement(&map['z' - root[i][j].letter], &root[i][j].size);
            if (j > 2) {
                bigFree(&root[i][j-2].size);
            }
        }
    }
    BIGINT res;
    bigAllocate(&res, 0);
    bigIncrement(&res, &root[0][len_j-1].size);
    if (len_i > 1) {
        bigIncrement(&res, &root[(len_i-1)][(len_j-1)].size);
    }

    return bigPrint(&res);
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

    char *res = calculate_result(root, length_i, length_j);
    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%s\n", res);
//#ifdef DEBUG
    printf("res: %s\n", res);
//#endif

    fclose(file_in);
    fclose(file_out);

    return 0;
}
