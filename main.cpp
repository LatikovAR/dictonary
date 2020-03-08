#include "hash_string.h"
#include <time.h>

int MAX_NEEDLE_LEN = 200;

char* read_file(char* filename);

int main() {
    char filename[200];
    char* text;
    char** needle;
    int* n_words_in_text;
    struct hashtable* hash_t;
    int n, i;
    long int start_time;

    printf("Filename: ");
    scanf("%s", filename);

    printf("Input num of words: ");
    scanf("%d", &n);

    needle = (char**) calloc((unsigned long long) n, sizeof (char*));
    for(i = 0; i < n; i++) {
        needle[i] = (char*) calloc(500, sizeof (char));
    }
    printf("Input words (each in new line):\n");
    for(i = 0; i < n; i++) {
        scanf("%s", needle[i]);
    }

    start_time = clock();
    text = read_file(filename);
    hash_t = hash_str(text, MAX_NEEDLE_LEN);

    n_words_in_text = (int*) calloc((unsigned long long) n, sizeof (int));
    for(i = 0; i < n; i++) {
        n_words_in_text[i] = rabin_karp_counter(hash_t, needle[i]);
    }

    printf("\nNumber of words:\n");
    for(i = 0; i < n; i++) {
        printf("%s: %d\n", needle[i], n_words_in_text[i]);
    }
    free(n_words_in_text);
    free(needle);

    free_hashtable(hash_t, MAX_NEEDLE_LEN);
    printf("\nTime: %ld", clock() - start_time);
    return 0;
}

char* read_file(char* filename) {
    char *str, *str_copy;
    FILE* f;
    unsigned long long str_len, max_str_len, inc_str_len = 10000;
    int c;
    assert(filename != nullptr);

    f = fopen(filename, "r");
    assert(f != nullptr);
    str = (char*) calloc(inc_str_len, sizeof(char));
    assert(str != nullptr);
    str_len = 0;
    max_str_len = inc_str_len;

    while((c = getc(f)) != EOF) {
        str[str_len] = (char) c;
        str_len++;
        if(str_len == max_str_len) {
            max_str_len += inc_str_len;
            str_copy = (char*) realloc(str, max_str_len);
            assert(str_copy != nullptr);
            str = str_copy;
        }
    }
    str[str_len] = '\0';
    str_len++;
    str_copy = (char*) realloc(str, str_len);
    assert(str_copy != nullptr);
    str = str_copy;

    fclose(f);
    return str;
}
