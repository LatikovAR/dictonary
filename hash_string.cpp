#include "hash_string.h"
#include "list.h"

int P = 23;
int Q = 977;

struct hashtable {
    struct hashtable_cer_w_len* hash_w_len;
    char* str;
    int str_len;
};

struct hashtable_cer_w_len {
    struct sl_list** list;
    int str_len;
};

int get_hash(const char *str, int str_len);
int check_str(struct hashtable* hash_t, const char* needle, int str_position);

struct hashtable* hash_file(FILE* f, int max_str_len) {
    struct hashtable* hash_t;
    int i, j, c;
    int* buff;
    char* str_p;

    assert(f != nullptr);
    assert(max_str_len > 0);

    hash_t = (struct hashtable*) calloc(1, sizeof (struct hashtable));
    hash_t->hash_w_len = (struct hashtable_cer_w_len*) calloc((unsigned long long) max_str_len, sizeof (struct hashtable_cer_w_len));
    assert(hash_t != NULL);
    for(i = 0; i < max_str_len; i++) {
        hash_t->hash_w_len[i].str_len = i + 1;
        hash_t->hash_w_len[i].list = (struct sl_list**) calloc((unsigned long long) Q, sizeof (struct sl_list*));
        assert(hash_t->hash_w_len[i].list != nullptr);
        for(j = 0; j < Q; j++) {
            hash_t->hash_w_len[i].list[j] = nullptr;
        }
    }

    buff = (int*) calloc((unsigned long long) max_str_len, sizeof (int));
    assert(buff != nullptr);
    hash_t->str_len = 10000;
    hash_t->str = (char*) calloc((size_t) hash_t->str_len, sizeof (char));
    assert(hash_t->str != nullptr);
    i = 0;
    while((c = getc(f)) != EOF) {
        if(i >= hash_t->str_len) {
            hash_t->str_len += 10000;
            str_p = (char*) realloc(hash_t->str, (size_t) hash_t->str_len);
            assert(str_p != nullptr);
            hash_t->str = str_p;
        }
        hash_t->str[i] = (char) c;


        j = i;
        if(j > (max_str_len - 1)) {
            j = max_str_len - 1;
        }
        while(j > 0) {
            buff[j] = (buff[j - 1] * P + c) % Q;
            add_list(hash_t->hash_w_len[j].list, buff[j], i - j);
            j--;
        }
        buff[0] = c % Q;
        add_list(hash_t->hash_w_len[0].list, buff[0], i);
        i++;
    }
    free(buff);

    if(i >= hash_t->str_len) {
        hash_t->str_len += 10000;
        str_p = (char*) realloc(hash_t->str, (size_t) hash_t->str_len);
        assert(str_p != nullptr);
        hash_t->str = str_p;
    }
    hash_t->str[i] = '\0';
    i++;

    str_p = (char*) realloc(hash_t->str, (size_t) i);
    assert(str_p != nullptr);
    hash_t->str = str_p;
    hash_t->str_len = i;
    //printf("%p ", hash_t[0].list[((int) 'a') % Q]);

    return hash_t;
}

int rabin_karp_counter(struct hashtable* hash_t, const char *needle) {
    int counter, needle_len, needle_hash, check_pos;
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

int check_str(struct hashtable* hash_t, const char* needle, int str_position) {
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
