#ifndef HASH_STRING_H
#define HASH_STRING_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

struct hashtable;

int rabin_karp_counter(struct hashtable* hash_t, const char *needle);
void free_hashtable(struct hashtable* hash_t, int max_word_len);
struct hashtable* hash_str(char* text, const int max_needle_len);

#endif // HASH_STRING_H
