#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "hash_string.h"

int P = 23;
int Q = 19441;

int get_hash(const char *start, int str_len);
int update_hash(int current, char cprev, char cnext, int n);
int pow_mod(int p, int k, int q);
int mod_p(int a, int q);

int rabin_karp_counter(const char *needle, const char *haystack) {
    int target, cur, n, word_len, counter;
    const char* check_word;

    assert(needle != NULL);
    assert(haystack != NULL);
    if(strlen(needle) > strlen(haystack)) {
        return 0;
    }

    check_word = haystack;
    word_len = (int) strlen(needle);
    target = get_hash(needle, word_len);
    cur = get_hash(check_word, word_len);
    counter = 0;
    n = pow_mod(P, word_len - 1, Q);

    do {
        if((target == cur) && (strncmp(needle, check_word, (unsigned) word_len) == 0)) {
            counter++;
        }
        cur = update_hash(cur, check_word[0], check_word[word_len], n);
        check_word++;
    } while (check_word[word_len - 1] != 0);

    return counter;
}

int get_hash(const char *str, int str_len) {
    int hash;
    unsigned i;
    assert(str != NULL);

    hash = 0;
    for(i = 0; i < str_len; i++) {
        hash = (hash * P + (int) str[i]) % Q;
    }
    return hash;
}

int pow_mod(int p, int k, int q) {
    int res;
    res = 1;
    while(k > 0) {
        if(k % 2 == 0) {
            p = (p * p) % q;
            k = k / 2;
        }
        else {
            res = (res * p) % q;
            k--;
        }
    }
    return res;
}

int update_hash(int current, char cprev, char cnext, int n) {
    current = mod_p((current - (int) cprev * n), Q);
    current = (current * P + (int) cnext) % Q;
    return current;
}

int mod_p(int a, int q) {
    int c;
    c = a / q;
    if(c * q > a) {
        return a - (c - 1) * q;
    }
    else {
        return a - c * q;
    }
}
