#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Créer un nouvel arbre binaire
BTree* create_btree() {
    BTree* tree = (BTree*)malloc(sizeof(BTree));
    tree->root = NULL;
    return tree;
}

void free_node(BTreeNode* node) {
    if (node != NULL) {
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

void free_btree(BTree* tree) {
    free_node(tree->root);
    free(tree);
}

int get_height(BTreeNode* node) {
    return node ? node->height : 0;
}

// Obtenir le facteur de déséquilibre
int get_balance_factor(BTreeNode* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

BTreeNode* create_node(Row data) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

BTreeNode* rotate_right(BTreeNode* y) {
    BTreeNode* x = y->left;
    BTreeNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));
    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));

    return x;
}

BTreeNode* rotate_left(BTreeNode* x) {
    BTreeNode* y = x->right;
    BTreeNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));
    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));

    return y;
}

BTreeNode* insert_node(BTreeNode* node, Row data) {
    if (node == NULL) {
        return create_node(data);
    }

    if (data.id < node->data.id) {
        node->left = insert_node(node->left, data);
    } else if (data.id > node->data.id) {
        node->right = insert_node(node->right, data);
    } else {
        return node;
    }

    node->height = 1 + (get_height(node->left) > get_height(node->right) ? get_height(node->left) : get_height(node->right));

    int balance = get_balance_factor(node);

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

void insert_btree(BTree* tree, Row data) {
    tree->root = insert_node(tree->root, data);
}

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

void print_node(BTreeNode* node) {
    if (node != NULL) {
        print_node(node->left);
        printf("(%d, %s, %s)\n", node->data.id, node->data.username, node->data.email);
        print_node(node->right);
    }
}

BTreeNode* find_min(BTreeNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

BTreeNode* delete_node(BTreeNode* node, int id, bool* deleted) {
    if (node == NULL) {
        *deleted = false;
        return NULL;
    }

    if (id < node->data.id) {
        node->left = delete_node(node->left, id, deleted);
    } else if (id > node->data.id) {
        node->right = delete_node(node->right, id, deleted);
    } else {
        *deleted = true;
        
        if (node->left == NULL) {
            BTreeNode* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            BTreeNode* temp = node->left;
            free(node);
            return temp;
        }

        BTreeNode* temp = find_min(node->right);
        node->data = temp->data;
        node->right = delete_node(node->right, temp->data.id, deleted);
    }
    
    node->height = 1 + (get_height(node->left) > get_height(node->right) ? 
                        get_height(node->left) : 
                        get_height(node->right));

    int balance = get_balance_factor(node);

    if (balance > 1 && get_balance_factor(node->left) >= 0) {
        return rotate_right(node);
    }
    if (balance > 1 && get_balance_factor(node->left) < 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && get_balance_factor(node->right) <= 0) {
        return rotate_left(node);
    }
    if (balance < -1 && get_balance_factor(node->right) > 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

bool delete_btree(BTree* tree, int id) {
    bool deleted = false;
    tree->root = delete_node(tree->root, id, &deleted);
    return deleted;
}

// Afficher l'arbre
void print_btree(BTree* tree) {
    print_node(tree->root);
}
