/*
 * BDD Implementation in C (C11 compatible)
 * Supports up to 17 variables for Boolean function evaluation
 * Includes basic reduction and a correctness tester
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VARS 17
#define MAX_NODES 1000000

typedef struct BDDNode {
    int var_index; // index of the variable (0 to MAX_VARS-1)
    struct BDDNode *low;
    struct BDDNode *high;
    char value; // '0', '1' or -1 if not terminal
} BDDNode;

typedef struct {
    int var_count;
    int node_count;
    BDDNode *root;
} BDD;

// Global pool of BDD nodes for simplicity
BDDNode node_pool[MAX_NODES];
int node_pool_index = 0;

BDDNode *create_node(int var_index, BDDNode *low, BDDNode *high) {
    if (node_pool_index >= MAX_NODES) {
        printf("Node pool overflow\n");
        exit(1);
    }
    BDDNode *node = &node_pool[node_pool_index++];
    node->var_index = var_index;
    node->low = low;
    node->high = high;
    node->value = -1;
    return node;
}

BDDNode *create_terminal(char val) {
    BDDNode *node = create_node(-1, NULL, NULL);
    node->value = val;
    return node;
}

// A naive parser/evaluator for boolean function in DNF format, e.g., "A!B+C!D"
char eval_expr(const char *expr, const char *inputs, const char *vars) {
    int n = strlen(expr);
    bool match = false;
    for (int i = 0; i < n;) {
        bool term_match = true;
        while (i < n && expr[i] != '+') {
            if (expr[i] == ' ' || expr[i] == '\t' || expr[i] == '*') {
                i++;
                continue;
            }

            bool neg = (expr[i] == '!');
            if (neg) i++;
            if (i >= n) break;

            char var = expr[i++];
            const char *pos = strchr(vars, var);
            if (pos == NULL) {
                term_match = false;
                continue;
            }

            int idx = (int)(pos - vars);
            if (inputs[idx] == (neg ? '1' : '0')) {
                term_match = false;
            }
        }
        if (term_match) match = true;
        if (i < n && expr[i] == '+') i++;
    }
    return match ? '1' : '0';
}

// Build BDD recursively (simple version without memoization for now)
BDDNode *build_bdd(const char *expr, const char *vars, int var_count, int depth, const char *inputs) {
    if (depth == var_count) {
        return create_terminal(eval_expr(expr, inputs, vars));
    }
    char inputs0[MAX_VARS+1], inputs1[MAX_VARS+1];
    strcpy(inputs0, inputs);
    strcpy(inputs1, inputs);
    inputs0[depth] = '0';
    inputs1[depth] = '1';
    inputs0[depth+1] = 0;
    inputs1[depth+1] = 0;
    BDDNode *low = build_bdd(expr, vars, var_count, depth + 1, inputs0);
    BDDNode *high = build_bdd(expr, vars, var_count, depth + 1, inputs1);

    // Reduction: eliminate redundant nodes
    if (low == high) return low;
    return create_node(depth, low, high);
}

BDD *BDD_create(const char *expr, const char *var_order) {
    BDD *bdd = malloc(sizeof(BDD));
    bdd->var_count = strlen(var_order);
    node_pool_index = 0;
    char inputs[MAX_VARS + 1] = {0};
    bdd->root = build_bdd(expr, var_order, bdd->var_count, 0, inputs);
    bdd->node_count = node_pool_index;
    return bdd;
}

char BDD_use(BDD *bdd, const char *input) {
    BDDNode *node = bdd->root;
    while (node->value == -1) {
        if (*input == 0) return -1;
        node = (input[node->var_index] == '0') ? node->low : node->high;
    }
    return node->value;
}

void test_bdd(const char *expr, const char *var_order) {
    printf("Testing expression: %s\n", expr);
    BDD *bdd = BDD_create(expr, var_order);
    printf("Created BDD with %d nodes\n", bdd->node_count);

    size_t var_count = strlen(var_order);
    int total = 1 << var_count;
    char inputs[MAX_VARS + 1];
    for (int i = 0; i < total; i++) {
        for (size_t j = 0; j < var_count; j++) {
            inputs[j] = ((i >> (var_count - j - 1)) & 1) + '0';
        }
        inputs[var_count] = 0;
        char expected = eval_expr(expr, inputs, var_order);
        char result = BDD_use(bdd, inputs);
        if (result != expected) {
            printf("Mismatch: %s -> expected %c, got %c\n", inputs, expected, result);
        }
    }
    printf("Test complete.\n");
}

int main() {
    // Example with 4 variables. Replace with larger test case up to 17
    test_bdd("a * b * c", "abc");
    return 0;
}
