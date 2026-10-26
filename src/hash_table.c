#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_SIZE 53
#define LOAD_FACTOR 0.7

typedef enum { EMPTY, OCCUPIED, DELETED } EntryState;

typedef struct {
    int key;
    EntryState state;
} HashEntry;

typedef struct {
    HashEntry* table;
    int size;
    int count;
} HashTable;

int prime_sizes[] = {
    53, 97, 193, 389, 769, 1543, 3079,
    6151, 12289, 24593, 49157, 98317,
    196613, 393241, 786433, 1572869,
    3145739, 6291469, 12582917, 25165843
};
int prime_count = sizeof(prime_sizes) / sizeof(prime_sizes[0]);

unsigned int hash1(int key, int size) {
    return key % size;
}

unsigned int hash2(int key, int size) {
    return 1 + (key % (size - 1));
}

void initTable(HashTable* ht, int sizeIndex) {
    ht->size = prime_sizes[sizeIndex];
    ht->count = 0;
    ht->table = (HashEntry*)malloc(sizeof(HashEntry) * ht->size);
    for (int i = 0; i < ht->size; i++) {
        ht->table[i].state = EMPTY;
    }
}

void rehash(HashTable* ht, int sizeIndex) {
    HashTable newTable;
    initTable(&newTable, sizeIndex);

    for (int i = 0; i < ht->size; i++) {
        if (ht->table[i].state == OCCUPIED) {
            int key = ht->table[i].key;
            unsigned int h1 = hash1(key, newTable.size);
            unsigned int h2 = hash2(key, newTable.size);

            int j = 0;
            while (newTable.table[(h1 + j * h2) % newTable.size].state == OCCUPIED) {
                j++;
            }
            int index = (h1 + j * h2) % newTable.size;
            newTable.table[index].key = key;
            newTable.table[index].state = OCCUPIED;
            newTable.count++;
        }
    }

    free(ht->table);
    *ht = newTable;
}

void insert(HashTable* ht, int key, int sizeIndex) {
    if ((float)ht->count / ht->size > LOAD_FACTOR && sizeIndex + 1 < prime_count) {
        rehash(ht, sizeIndex + 1);
    }

    unsigned int h1 = hash1(key, ht->size);
    unsigned int h2 = hash2(key, ht->size);

    int j = 0;
    int index;
    do {
        index = (h1 + j * h2) % ht->size;
        if (ht->table[index].state != OCCUPIED) {
            ht->table[index].key = key;
            ht->table[index].state = OCCUPIED;
            ht->count++;
            return;
        }
        j++;
    } while (j < ht->size);
}

int search(HashTable* ht, int key) {
    unsigned int h1 = hash1(key, ht->size);
    unsigned int h2 = hash2(key, ht->size);

    int j = 0;
    int index;
    do {
        index = (h1 + j * h2) % ht->size;
        if (ht->table[index].state == EMPTY) return 0;
        if (ht->table[index].state == OCCUPIED && ht->table[index].key == key) return 1;
        j++;
    } while (j < ht->size);
    return 0;
}

void delete(HashTable* ht, int key) {
    unsigned int h1 = hash1(key, ht->size);
    unsigned int h2 = hash2(key, ht->size);

    int j = 0;
    int index;
    do {
        index = (h1 + j * h2) % ht->size;
        if (ht->table[index].state == EMPTY) return;
        if (ht->table[index].state == OCCUPIED && ht->table[index].key == key) {
            ht->table[index].state = DELETED;
            ht->count--;
            return;
        }
        j++;
    } while (j < ht->size);
}

void process_test_file(const char* filename, HashTable* ht, int* sizeIndex) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Nepodarilo sa otvorit subor %s\n", filename);
        return;
    }

    char command[16];
    int key;
    clock_t start = clock();

    while (fscanf(file, "%s %d", command, &key) == 2) {
        if (strcmp(command, "INSERT") == 0) {
            insert(ht, key, *sizeIndex);
        } else if (strcmp(command, "SEARCH") == 0) {
            search(ht, key);
        } else if (strcmp(command, "DELETE") == 0) {
            delete(ht, key);
        }
    }

    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTestovanie dokoncene za %.4f sekund.\n", duration);
    fclose(file);
}

int main() {
    HashTable ht;
    int sizeIndex = 0;
    initTable(&ht, sizeIndex);

    int choice, key;

    while (1) {
        printf("\n===== HASH TABULKA (dvojite rozptylenie) =====\n");
        printf("1. Vlozit cislo\n");
        printf("2. Vyhladat cislo\n");
        printf("3. Zmazat cislo\n");
        printf("4. Spusti test\n");
        printf("5. Ukoncit program\n");

        printf("Zvol moznost: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Zadaj cislo na vlozenie: ");
                scanf("%d", &key);
                insert(&ht, key, sizeIndex);
                break;
            case 2:
                printf("Zadaj cislo na vyhladanie: ");
                scanf("%d", &key);
                printf(search(&ht, key) ? "Nasiel sa!\n" : "Nenasiel sa.\n");
                break;
            case 3:
                printf("Zadaj cislo na zmazanie: ");
                scanf("%d", &key);
                delete(&ht, key);
                break;
            case 4:
                process_test_file("test_input.txt", &ht, &sizeIndex);
                break;
            case 5:
                printf("Ukoncujem program.\n");
                free(ht.table);
                return 0;
            default:
                printf("Neplatna volba!\n");
        }
    }

    return 0;
}
