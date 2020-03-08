#include "hash_string.h"
#include "list.h"

int P = 23;
int Q = 977;

struct hashtable {
    struct hashtable_cer_w_len* hash_w_len;
    char* str;
    unsigned long long str_len;
};

struct hashtable_cer_w_len {
    struct sl_list** list;
    int str_len;
};

// В struct hashtable содержится хешированная строка, ее длина и общая хеш-таблица.
// Общая хеш-таблица состоит из набора хеш-таблиц (struct hashtable_cer_w_len) для всевозможных заданных длин искомой строки.
// Хеш-таблица на слова конкретной длины содержит эту самую длину
// и наборы коллизий на каждый хеш, записанные при помощи односвязных списков (struct sl_list).

static int get_hash(const char *str, int str_len);
static int check_str(struct hashtable* hash_t, const char* needle, long long str_position);
static struct hashtable* make_hash_t(char* text, const int max_needle_len);

struct hashtable* hash_str(char* text, const int max_needle_len) {
    struct hashtable* hash_t;
    unsigned long long i, j;
    int* buff;
    assert(text != nullptr);
    assert(max_needle_len > 0);

    hash_t = make_hash_t(text, max_needle_len);

    buff = (int*) calloc((unsigned long long) max_needle_len, sizeof (int));
    assert(buff != nullptr);
    i = 0;
    while(i < hash_t->str_len) {
        j = i;
        if(j > (unsigned long long) (max_needle_len - 1)) {
            j = (unsigned long long) (max_needle_len - 1);
        }
        while(j > 0) {
            buff[j] = (buff[j - 1] * P + hash_t->str[i]) % Q;
            add_list(hash_t->hash_w_len[j].list, buff[j], i - j);
            j--;
        }
        buff[0] = hash_t->str[i] % Q;
        add_list(hash_t->hash_w_len[0].list, buff[0], i);
        i++;
    }
    free(buff);

    return hash_t;
}

int rabin_karp_counter(struct hashtable* hash_t, const char *needle) {
    int counter, needle_len, needle_hash;
    long long check_pos;
    assert(needle != NULL);

    needle_len = (int) strlen(needle);
    counter = 0;
    assert(hash_t->hash_w_len[needle_len - 1].str_len == needle_len);
    needle_hash = get_hash(needle, needle_len);
    check_pos = read_list(hash_t->hash_w_len[needle_len - 1].list[needle_hash]);

    while(check_pos != -1) {
        if(check_str(hash_t, needle, check_pos) == 1) {
            counter++;
        }
        check_pos = read_list(nullptr);
    }

    return counter;
}

int get_hash(const char *str, int str_len) {
    int hash;
    int i;
    assert(str != NULL);

    hash = 0;
    for(i = 0; i < str_len; i++) {
        hash = (hash * P + (int) str[i]) % Q;
    }
    return hash;
}

int check_str(struct hashtable* hash_t, const char* needle, long long str_position) {
    int needle_len, i;
    assert(hash_t != nullptr);
    assert(needle != nullptr);

    needle_len = (int) strlen(needle);

    for(i = 0; i < needle_len; i++) {
        if(hash_t->str[str_position + i] != needle[i]) {
            //printf("%d %c %c \n", f_position, (char) s, needle[i]);
            return 0;
        }
    }
    //printf("%d ", f_position);
    return 1;
}

struct hashtable* make_hash_t(char* text, const int max_needle_len) {
    int i, j;
    struct hashtable* hash_t;
    assert(text != nullptr);

    hash_t = (struct hashtable*) calloc(1, sizeof (struct hashtable));
    hash_t->hash_w_len = (struct hashtable_cer_w_len*) calloc((unsigned long long) max_needle_len, sizeof (struct hashtable_cer_w_len));
    assert(hash_t != NULL);
    hash_t->str = text;
    hash_t->str_len = strlen(hash_t->str);
    for(i = 0; i < max_needle_len; i++) {
        hash_t->hash_w_len[i].str_len = i + 1;
        hash_t->hash_w_len[i].list = (struct sl_list**) calloc((unsigned long long) Q, sizeof (struct sl_list*));
        assert(hash_t->hash_w_len[i].list != nullptr);
        for(j = 0; j < Q; j++) {
            hash_t->hash_w_len[i].list[j] = nullptr;
        }
    }

    return hash_t;
}

void free_hashtable(struct hashtable* hash_t, int max_word_len) {
    int i, j;
    assert(hash_t != nullptr);

    for(i = 0; i < max_word_len; i++) {
        for(j = 0; j < Q; j++) {
            free_list(hash_t->hash_w_len[i].list[j]);
        }
        free(hash_t->hash_w_len[i].list);
    }
    free(hash_t->hash_w_len);
    free(hash_t->str);
    free(hash_t);
}
