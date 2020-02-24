#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "hash_string.h"

int HAYSTACK_LEN = 1000;

int read_part_of_file(FILE* f, char* s, int haystack_len, int needle_len);

int main() {
    char filename[200];
    FILE* f;
    char needle[500];
    char* haystack;
    int n;

    printf("Filename: ");
    scanf("%s", filename);
    f = fopen(filename, "r");
    assert(f != NULL);

    printf("Search word: ");
    scanf("%s", needle);

    haystack = (char*) calloc((unsigned long long) (HAYSTACK_LEN), sizeof (char));
    n = 0;
    while(read_part_of_file(f, haystack, HAYSTACK_LEN, strlen(needle)) != 0) {
        n += rabin_karp_counter(needle, haystack);
    }
    free(haystack);

    printf("Number of words: %d", n);
    fclose(f);
    return 0;
}

int read_part_of_file(FILE* f, char* s, int haystack_len, int needle_len) {
    int i;
    int c;
    assert(s != NULL);
    for(i = 0; i < (haystack_len - 1); i++) {
        c = getc(f);
        if(c == EOF) {
            s[i] = '\0';
            return i;
        }
        s[i] = (char) c;
    }
    s[haystack_len - 1] = '\0';
    fseek(f, 1 - (int) needle_len, SEEK_CUR);
    return (haystack_len - 1);
}
