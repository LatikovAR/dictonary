#include "hash_string.h"
#include <time.h>

int MAX_NEEDLE_LEN = 200;

int main() {
    char filename[200];
    FILE* f;
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
    f = fopen(filename, "r");
    assert(f != NULL);
    hash_t = hash_file(f, MAX_NEEDLE_LEN);
    fclose(f);

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
