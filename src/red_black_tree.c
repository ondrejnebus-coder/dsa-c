#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int key;
    Color color;
    struct Node *left, *right, *parent;
    char delete;
} Node;

typedef struct {
    Node *root;
} RBTree;

Node *create_node(int key, Color color) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->color = color;
    node->delete = 'Y';
    node->left = node->right = node->parent = NULL;
    return node;
}

void left_rotate(RBTree *tree, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void right_rotate(RBTree *tree, Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right)
        x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent)
        tree->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;
    x->right = y;
    y->parent = x;
}

void insert_fixup(RBTree *tree, Node *z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}
void insert(RBTree *tree, int key) {
    Node *New = create_node(key, RED);
    Node *Temp = NULL;
    Node *x = tree->root;
    while (x) {
        Temp = x;
        if (New->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    New->parent = Temp;
    if (!Temp)
        tree->root = New;
    else if (New->key < Temp->key)
        Temp->left = New;
    else
        Temp->right = New;
    insert_fixup(tree, New);
}
Node *search(Node *root, int key) {
    if (!root || root->key == key) {
        if (root && root->delete == 'N') {
            return NULL;
        }
        return root;
    }

    if (key < root->key)
        return search(root->left, key);
    return search(root->right, key);
}
void free_tree(Node *root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}
Node *deletenum( Node* root,int key) {
    if (!root) return NULL;
    if (root->key == key) {
        root->delete = 'N';
        return root;
    }
    if (key < root->key)
        return deletenum(root->left, key);
    return deletenum(root->right, key);
}
    void process_test_file(const char *filename, RBTree *tree) {
        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("Chyba pri otvarani suboru");
            return;
        }

        char line[99999];
        clock_t start_time = clock();
        printf("1");
        while (fgets(line, sizeof(line), file)) {
            char command[80];
            int value;
            if (sscanf(line, "%s %d", command, &value) !=2)
                continue;

            if (strcmp(command, "INSERT") == 0) {
                insert(tree, value);
            } else if (strcmp(command, "SEARCH") == 0) {
                search(tree->root, value);
            } else if (strcmp(command, "DELETE") == 0) {
                deletenum(tree->root, value);
            }
        }
        printf("h");
        fclose(file);
        clock_t end_time = clock();
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("Cas spracovania suboru: %.6f sekund\n", elapsed_time);
    }



int main() {
    RBTree *tree = (RBTree *)malloc(sizeof(RBTree));
    tree->root = NULL;

    int choice, key;

    while (1) {
        printf("\n===== Cerveno-Cierny Strom =====\n");
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
            insert(tree, key);
            printf("Cislo %d bolo vlozene.\n", key);
            break;
            case 2:
                printf("Zadajte cislo na vyhladanie: ");
            scanf("%d", &key);
            if (search(tree->root, key)) {
                printf("Cislo %d sa nachadza v strome.\n", key);
            }else
                printf("Cislo %d sa v strome nenachadza.\n", key);
            break;
            case 3:
                printf("Ukoncujem program...\n");
            free_tree(tree->root);
            return 0;
            case 4:
                printf("Zadajte cislo na zmazanie: ");
                scanf("%d", &key);
            deletenum(tree->root, key);
            break;
            case 5:
                process_test_file("test_input.txt", tree);
            break;
            default:
                printf("Neplatna volba! Skuste znova.\n");
        }

    }
}