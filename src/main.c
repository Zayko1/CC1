#include "btree.h"
#include "repl.h"

int main() {
    BTree* tree = create_btree();
    repl(tree);
    free_btree(tree);
    return 0;
}

