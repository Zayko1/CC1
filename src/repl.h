#ifndef REPL_H
#define REPL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"

// Énumérations pour les résultats des commandes et les types de statements
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

// Structure pour une commande SQL
typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;

// Structure pour l'entrée utilisateur
typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

// Prototypes des fonctions
InputBuffer* new_input_buffer();
void close_input_buffer(InputBuffer* input_buffer);
void print_prompt();
void read_input(InputBuffer* input_buffer);
MetaCommandResult do_meta_command(InputBuffer* input_buffer, BTree* tree);
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
void execute_statement(Statement* statement, BTree* tree);
void repl(BTree* tree);

#endif

