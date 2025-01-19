#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour créer un nouvel arbre binaire
BTree* create_btree() {
    BTree* tree = (BTree*)malloc(sizeof(BTree));
    tree->root = NULL;
    return tree;
}

// Fonction pour libérer un nœud
void free_node(BTreeNode* node) {
    if (node != NULL) {
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

// Fonction pour libérer l'arbre
void free_btree(BTree* tree) {
    free_node(tree->root);
    free(tree);
}

// Fonction pour obtenir la hauteur d'un nœud
int get_height(BTreeNode* node) {
    return node ? node->height : 0;
}

// Fonction pour obtenir le facteur de déséquilibre
int get_balance_factor(BTreeNode* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

// Fonction pour créer un nouveau nœud
BTreeNode* create_node(Row data) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Rotation à droite pour équilibrer l'arbre
BTreeNode* rotate_right(BTreeNode* y) {
    BTreeNode* x = y->left;
    BTreeNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));
    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));

    return x;
}

// Rotation à gauche pour équilibrer l'arbre
BTreeNode* rotate_left(BTreeNode* x) {
    BTreeNode* y = x->right;
    BTreeNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));
    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));

    return y;
}

// Fonction pour insérer un nœud dans l'arbre
BTreeNode* insert_node(BTreeNode* node, Row data) {
    if (node == NULL) {
        return create_node(data);
    }

    if (data.id < node->data.id) {
        node->left = insert_node(node->left, data);
    } else if (data.id > node->data.id) {
        node->right = insert_node(node->right, data);
    } else {
        // Clé déjà présente
        return node;
    }

    // Mettre à jour la hauteur
    node->height = 1 + (get_height(node->left) > get_height(node->right) ? get_height(node->left) : get_height(node->right));

    // Vérifier l'équilibrage
    int balance = get_balance_factor(node);

    // Rotation gauche-droite si déséquilibre détecté
    if (balance > 1 && data.id < node->left->data.id) {
        return rotate_right(node);
    }

    if (balance < -1 && data.id > node->right->data.id) {
        return rotate_left(node);
    }

    if (balance > 1 && data.id > node->left->data.id) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    if (balance < -1 && data.id < node->right->data.id) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

// Fonction pour insérer un élément dans l'arbre
void insert_btree(BTree* tree, Row data) {
    tree->root = insert_node(tree->root, data);
}

// Fonction pour rechercher un élément dans l'arbre
bool search_btree(BTree* tree, int id, Row* result) {
    BTreeNode* current = tree->root;
    while (current != NULL) {
        if (id == current->data.id) {
            *result = current->data;
            return true;
        }
        current = (id < current->data.id) ? current->left : current->right;
    }
    return false;
}

// Fonction pour imprimer les nœuds de l'arbre
void print_node(BTreeNode* node) {
    if (node != NULL) {
        print_node(node->left);
        printf("(%d, %s, %s)\n", node->data.id, node->data.username, node->data.email);
        print_node(node->right);
    }
}

// Fonction pour afficher l'arbre
void print_btree(BTree* tree) {
    print_node(tree->root);
}

