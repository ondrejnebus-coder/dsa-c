#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
typedef struct TreapNode {
    int key;
    int priority;
    struct TreapNode* left;
    struct TreapNode* right;
} TreapNode;

typedef struct Treap {
    TreapNode* root;
} Treap;

TreapNode* newNode(int key) {
    TreapNode* node = (TreapNode*)malloc(sizeof(TreapNode));
    node->key = key;
    node->priority = ((rand() << 15) | rand());
    node->left = node->right = NULL;
    return node;
}


TreapNode* rotateRight(TreapNode* y) {
    TreapNode* x = y->left;
    TreapNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

TreapNode* rotateLeft(TreapNode* x) {
    TreapNode* y = x->right;
    TreapNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

TreapNode* insert(TreapNode* root, int key) {
    if (root == NULL)
        return newNode(key);

    if (key < root->key) {
        root->left = insert(root->left, key);
        if (root->left->priority > root->priority)
            root = rotateRight(root);
    } else if (key > root->key) {
        root->right = insert(root->right, key);
        if (root->right->priority > root->priority)
            root = rotateLeft(root);
    }
    return root;
}

int search(TreapNode* root, int key) {
    if (root == NULL) return 0;
    if (key == root->key) return 1;
    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}

TreapNode* deleteNode(TreapNode* root, int key) {
    if (root == NULL) return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // uzol nájdený
        if (root->left == NULL) {
            TreapNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreapNode* temp = root->left;
            free(root);
            return temp;
        } else {
            if (root->left->priority > root->right->priority) {
                root = rotateRight(root);
                root->right = deleteNode(root->right, key);
            } else {
                root = rotateLeft(root);
                root->left = deleteNode(root->left, key);
            }
        }
    }
    return root;
}
void freeTree(TreapNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void process_test_file(const char* filename, Treap* tree) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Chyba pri otvarani suboru");
        return;
    }

    char line[256];
    clock_t start_time = clock();

    while (fgets(line, sizeof(line), file)) {
        char command[20];
        int value;

        if (sscanf(line, "%s %d", command, &value) != 2)
            continue;

        if (strcmp(command, "INSERT") == 0) {
            tree->root = insert(tree->root, value);
        }
        else if (strcmp(command, "SEARCH") == 0) {
            search(tree->root, value);
        }
        else if (strcmp(command, "DELETE") == 0) {
            tree->root = deleteNode(tree->root, value);
        }
    }

    fclose(file);
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Cas spracovania suboru: %.6f sekund\n", elapsed_time);
}
int main() {
    srand(time(NULL));

    Treap* tree = (Treap*)malloc(sizeof(Treap));
    tree->root = NULL;

    int choice, key;

    while (1) {
        printf("\n===== TREAP Strom =====\n");
        printf("1. Vlozit cislo\n");
        printf("2. Vyhladat cislo\n");
        printf("3. Ukoncit program\n");
        printf("4. Zmaz cislo\n");
        printf("5. Spusti test\n");

        printf("Vyberte moznost: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Zadajte cislo na vlozenie: ");
                scanf("%d", &key);
                tree->root = insert(tree->root, key);
                printf("Cislo %d bolo vlozene.\n", key);
                break;

            case 2:
                printf("Zadajte cislo na vyhladanie: ");
                scanf("%d", &key);
                if (search(tree->root, key))
                    printf("Cislo %d sa nachadza v strome.\n", key);
                else
                    printf("Cislo %d sa v strome nenachadza.\n", key);
                break;


            case 3:
                printf("Ukoncujem program...\n");
                freeTree(tree->root);
                free(tree);
                return 0;

            case 4:
                printf("Zadajte cislo na zmazanie: ");
                scanf("%d", &key);
                tree->root = deleteNode(tree->root, key);
                printf("Cislo %d bolo zmazane (ak existovalo).\n", key);
                break;

            case 5:
                process_test_file("test_input.txt", tree);
                break;

            default:
                printf("Neplatna volba! Skuste znova.\n");
        }
    }
    return 0;
}
