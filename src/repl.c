#include "btree.h"
#include "repl.h"


// Créer un nouvel InputBuffer
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

// Libérer l'InputBuffer
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

// Afficher le prompt
void print_prompt() {
    printf("db > ");
}

// Lire l'entrée utilisateur
void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

// Gérer les commandes spéciales
MetaCommandResult do_meta_command(InputBuffer* input_buffer, BTree* tree) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    free_btree(tree);
    exit(EXIT_SUCCESS);
  } else if (strcmp(input_buffer->buffer, ".help") == 0) {
    printf("Available commands:\n");
    printf(".exit - Exit the program\n");
    printf(".help - Show help message\n");
    return META_COMMAND_SUCCESS;
  } else {
    //TODO  here implement handling of other input as .exit
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

// Préparer une commande SQL
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        
        Row* row = &statement->row_to_insert;
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %31s %254s", 
                                   &row->id, row->username, row->email);
        
        if (args_assigned < 3) {
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }
        return PREPARE_SUCCESS;
    }
    
    if (strcmp(input_buffer->buffer, "select *") == 0) {
        statement->type = STATEMENT_SELECT;
        statement->row_to_insert.id = 0; // ID par défaut pour SELECT *
        return PREPARE_SUCCESS;
    }
    
    if (strncmp(input_buffer->buffer, "select where id=", 16) == 0) {
        statement->type = STATEMENT_SELECT;

        // Extraire directement l'ID
        if (sscanf(input_buffer->buffer, "select where id=%d", &statement->row_to_insert.id) != 1) {
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }

        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

// Exécuter une commande SQL
void execute_statement(Statement* statement, BTree* tree) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            insert_btree(tree, statement->row_to_insert);
            printf("Row inserted successfully.\n");
            break;

        case STATEMENT_SELECT:
            if (statement->row_to_insert.id == 0) {
                // Commande SELECT * : afficher toute la table
                printf("Displaying rows:\n");
                print_btree(tree);
            } else {
                // Commande SELECT WHERE : rechercher une ligne spécifique
                Row result;
                if (search_btree(tree, statement->row_to_insert.id, &result)) {
                    printf("(%d, %s, %s)\n", result.id, result.username, result.email);
                } else {
                    printf("No row found with id=%d.\n", statement->row_to_insert.id);
                }
            }
            break;
    }
}

// Boucle REPL
void repl(BTree* tree) {
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer, tree)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }

        execute_statement(&statement, tree);
        printf("Executed.\n");
    }
}

