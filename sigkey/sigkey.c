#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//#define DEBUG

#define FILE_IN "sigkey.in"
#define FILE_OUT "sigkey.out"

#define RADIX 10

static int etalon[27] = {0, };

struct keys {
    char key[27];
    int weight;
    int length;
    int fouded;
    int mask;

    struct keys *left;
    struct keys *right;
};
typedef struct keys keys_t;

void set_key_data(keys_t *key)
{
    int i, weight = 0;
    int length = strlen(key->key);
    char *key_str = key->key;
    int max = 0;
    int mask = 0;

    for (i = 0; i < length; i++) {
        weight |= 1 << (key_str[i] - 'a');
        if (max < (key_str[i] - 'a')) {
            max = key_str[i] - 'a';
        }
    }

    for (i = 0; i <= max; i++) {
        mask |= 1 << i;
    }

    key->mask = mask;
    key->length = length;
    key->fouded = 0;
    key->left = NULL;
    key->right = NULL;
    key->weight = weight;

    return;
}

int check_keys(keys_t *key1, keys_t *key2)
{
    if (key1->fouded || key2->fouded) {
        return 0;
    }

    if (key1->weight & key2->weight) {
        return 0;
    }

    int lens = key1->length + key2->length;
    int weight = key1->weight + key2->weight;

    if (etalon[lens] == weight) {
        key1->fouded = key2->fouded = 1;
        return 1;
    }
    return 0;
}

static keys_t *root;

void add(keys_t *parent, keys_t *element)
{
    if (parent == NULL) {
        parent = root;
    }

    if (root == NULL) {
        root = element;
        return;
    }

    if (element->weight > parent->weight) {
        if (parent->right) {
            add(parent->right, element);
        } else {
            parent->right = element;
        }
    } else {
        if (parent->left) {
            add(parent->left, element);
        } else {
            parent->left = element;
        }
    }
}

void get_data(FILE *file, int *length)
{
    int i, k;
    fscanf(file, "%d", length);
    keys_t *item;

    for (i = 0; i < *length; i++) {
        item = (keys_t *)calloc(sizeof(keys_t), 1);
        fscanf(file, "%s", item->key);
        set_key_data(item);

        add(root, item);
    }

    return;
}

keys_t *find(keys_t *parrent, int weight, keys_t *key)
{
    if (parrent == NULL) {
        parrent = root;
    }

    if (parrent->weight == weight) {
//        printf("found: %s vs %s\n", key->key, parrent->key);
        return parrent;
    }
    if (parrent->right && weight > parrent->weight) {
        return find(parrent->right, weight, key);
    } else if (parrent->left && weight < parrent->weight) {
        return find(parrent->left, weight, key);
    }

    return NULL;
}

int find_key(keys_t *key)
{
    int ret = 0;
    if (!key || key->fouded) {
        return 0;
    }

    int found = key->mask - key->weight;
    keys_t *found_key  = find(NULL, found, key);
    key->fouded = 1;
    if (found_key) {
        found_key->fouded = 1;
        ret = 1;
    }

    return ret;
}

int calculate_result(keys_t *root)
{
    int res = 0;

    if (!root || root->fouded) {
        return 0;
    }

    res += calculate_result(root->left);
    res += find_key(root);
    res += calculate_result(root->right);

    return res;
}

void print_tree(keys_t *root)
{
    if (!root) {
        return;
    }
    print_tree(root->left);
    printf("key: %d\n", root->weight);
    print_tree(root->right);
}

int main(int argc, char* argv[])
{
    FILE *file_in = NULL, *file_out = NULL;
    char *file_name_in = FILE_IN, *file_name_out = FILE_OUT;
    int length = 0;

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

    get_data(file_in, &length);

#ifdef DEBUG
    /*Debug data*/
    for (i = 0; i < C; i++) {
        printf("H[%d]: %d, G[%d]: %d\n", i, H[i].h, i, H[i].g);
    }
#endif

//    print_tree(root);

    int res = calculate_result(root);
    if (!(file_out = fopen(file_name_out,"w"))) {
        printf("File OUT not found '%s'\n", file_name_out);
        return 0;
    }

    fprintf(file_out, "%d\n", res);
//#ifdef DEBUG
    printf("res: %d\n", res);
//#endif

    /*Free data*/
    if (root) {
        free(root);
    }

    fclose(file_in);
    fclose(file_out);

    return 0;
}
