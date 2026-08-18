/* Reconstructed from kvstore.exe by Synthex v10
 *
 * Source: Ghidra decompiled output (1891 lines, 35 functions)
 * Method: Multi-generational LLM refinement + manual type recovery
 *
 * Key recoveries:
 * - KVStore struct: Entry *buckets[64] + int count (520 bytes)
 * - Entry struct: char key[128] + char value[512] + Entry *next (648 bytes)
 * - djb2 hash function with TABLE_SIZE=64 mask
 * - Interactive command loop with set/get/del/save/quit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 64
#define MAX_KEY_LEN 128
#define MAX_VAL_LEN 512

typedef struct Entry {
    char key[MAX_KEY_LEN];
    char value[MAX_VAL_LEN];
    struct Entry *next;
} Entry;

typedef struct {
    Entry *buckets[TABLE_SIZE];
    int count;
} KVStore;

/* djb2 hash — recovered from FUN_140001020 */
static unsigned long hash_key(const char *key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

/* kv_create — recovered from main (calloc 0x208 = 520 bytes = KVStore) */
KVStore *kv_create(void) {
    KVStore *store = calloc(1, sizeof(KVStore));
    return store;
}

/* kv_set — recovered from FUN_1400014b0 */
int kv_set(KVStore *store, const char *key, const char *value) {
    if (!store || !key || !value) return -1;
    if (strlen(key) >= MAX_KEY_LEN || strlen(value) >= MAX_VAL_LEN) return -1;

    unsigned long idx = hash_key(key);
    Entry *cur = store->buckets[idx];

    /* Update existing entry if key matches */
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            strncpy(cur->value, value, MAX_VAL_LEN - 1);
            cur->value[MAX_VAL_LEN - 1] = '\0';
            return 0;
        }
        cur = cur->next;
    }

    /* Insert new entry at head of chain */
    Entry *entry = malloc(sizeof(Entry));
    if (!entry) return -1;
    strncpy(entry->key, key, MAX_KEY_LEN - 1);
    entry->key[MAX_KEY_LEN - 1] = '\0';
    strncpy(entry->value, value, MAX_VAL_LEN - 1);
    entry->value[MAX_VAL_LEN - 1] = '\0';
    entry->next = store->buckets[idx];
    store->buckets[idx] = entry;
    store->count++;
    return 0;
}

/* kv_get — recovered from FUN_140001640 */
const char *kv_get(KVStore *store, const char *key) {
    if (!store || !key) return NULL;
    unsigned long idx = hash_key(key);
    Entry *cur = store->buckets[idx];
    while (cur) {
        if (strcmp(cur->key, key) == 0)
            return cur->value;
        cur = cur->next;
    }
    return NULL;
}

/* kv_delete — recovered from FUN_140001700 */
int kv_delete(KVStore *store, const char *key) {
    if (!store || !key) return -1;
    unsigned long idx = hash_key(key);
    Entry *prev = NULL;
    Entry *cur = store->buckets[idx];
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            if (prev)
                prev->next = cur->next;
            else
                store->buckets[idx] = cur->next;
            free(cur);
            store->count--;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return -1;
}

/* kv_save — recovered from FUN_1400017f0 */
int kv_save(KVStore *store, const char *filename) {
    if (!store || !filename) return -1;
    FILE *fp = fopen(filename, "w");
    if (!fp) return -1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *cur = store->buckets[i];
        while (cur) {
            fprintf(fp, "%s=%s\n", cur->key, cur->value);
            cur = cur->next;
        }
    }
    fclose(fp);
    return 0;
}

/* kv_load — recovered from FUN_140001880 */
int kv_load(KVStore *store, const char *filename) {
    if (!store || !filename) return -1;
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    char line[MAX_KEY_LEN + MAX_VAL_LEN + 2];
    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        char *eq = strchr(line, '=');
        if (!eq) continue;
        *eq = '\0';
        kv_set(store, line, eq + 1);
    }
    fclose(fp);
    return 0;
}

/* kv_destroy — recovered from FUN_140001940 */
void kv_destroy(KVStore *store) {
    if (!store) return;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *cur = store->buckets[i];
        while (cur) {
            Entry *next = cur->next;
            free(cur);
            cur = next;
        }
    }
    free(store);
}

/* main — recovered from FUN_140001a00 */
int main(int argc, char *argv[]) {
    KVStore *store = kv_create();
    if (!store) {
        fprintf(stderr, "Failed to create store\n");
        return 1;
    }

    /* Load from file if argument provided */
    if (argc > 1) {
        kv_load(store, argv[1]);
    }

    /* Interactive command loop */
    char cmd[16];
    char key[MAX_KEY_LEN];
    char value[MAX_VAL_LEN];

    printf("kvstore> ");
    while (scanf("%15s", cmd) == 1) {
        if (strcmp(cmd, "set") == 0) {
            if (scanf("%127s %511[^\n]", key, value) == 2) {
                if (kv_set(store, key, value) == 0)
                    printf("OK\n");
                else
                    printf("ERROR\n");
            }
        } else if (strcmp(cmd, "get") == 0) {
            if (scanf("%127s", key) == 1) {
                const char *val = kv_get(store, key);
                if (val)
                    printf("%s\n", val);
                else
                    printf("(nil)\n");
            }
        } else if (strcmp(cmd, "del") == 0) {
            if (scanf("%127s", key) == 1) {
                if (kv_delete(store, key) == 0)
                    printf("OK\n");
                else
                    printf("(nil)\n");
            }
        } else if (strcmp(cmd, "save") == 0) {
            if (scanf("%127s", key) == 1) {
                if (kv_save(store, key) == 0)
                    printf("OK\n");
                else
                    printf("ERROR\n");
            }
        } else if (strcmp(cmd, "quit") == 0) {
            break;
        } else {
            printf("Commands: set <key> <value> | get <key> | del <key> | save <file> | quit\n");
        }
        printf("kvstore> ");
    }

    kv_destroy(store);
    return 0;
}
