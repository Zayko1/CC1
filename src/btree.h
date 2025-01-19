#ifndef BTREE_H
#define BTREE_H

#include <stdbool.h>
#include <stddef.h>

// Définir la structure pour une ligne
typedef struct {
    int id;
    char username[32];
    char email[255];
} Row;

// Définir un nœud de l'arbre
typedef struct BTreeNode {
    Row data;
    struct BTreeNode* left;
    struct BTreeNode* right;
    int height; // Utilisé pour équilibrer l'arbre
} BTreeNode;

// Définir la structure de l'arbre
typedef struct {
    BTreeNode* root;
} BTree;

// Prototypes des fonctions
BTree* create_btree();
void free_btree(BTree* tree);
void insert_btree(BTree* tree, Row data);
bool search_btree(BTree* tree, int id, Row* result);
void delete_btree(BTree* tree, int id);
void print_btree(BTree* tree);

#endif

