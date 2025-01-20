#ifndef BTREE_H
#define BTREE_H

#include <stdbool.h>
#include <stddef.h>

// Structure pour une ligne
typedef struct {
    int id;
    char username[32];
    char email[255];
} Row;

typedef struct BTreeNode {
    Row data;
    struct BTreeNode* left;
    struct BTreeNode* right;
    int height;
} BTreeNode;

typedef struct {
    BTreeNode* root;
} BTree;

BTree* create_btree();
void free_node(BTreeNode* node);
void free_btree(BTree* tree);
int get_height(BTreeNode* node);
int get_balance_factor(BTreeNode* node);
BTreeNode* create_node(Row data);
BTreeNode* rotate_right(BTreeNode* y);
BTreeNode* rotate_left(BTreeNode* x);
BTreeNode* insert_node(BTreeNode* node, Row data);
void print_node(BTreeNode* node);
void insert_btree(BTree* tree, Row data);
bool search_btree(BTree* tree, int id, Row* result);
BTreeNode* find_min(BTreeNode* node);
BTreeNode* delete_node(BTreeNode* node, int id, bool* deleted);
bool delete_btree(BTree* tree, int id);
void print_btree(BTree* tree);

#endif
