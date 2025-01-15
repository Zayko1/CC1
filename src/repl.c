#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TABLE_MAX_ROWS 100

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

typedef struct {
  StatementType type;
  Row row_to_insert;
} Statement;

typedef struct{
  size_t id;
  char username[32];
  char email[255];
} Row;

typedef struct {
  Row rows[TABLE_MAX_ROWS];
  size_t num_row;
} Table;

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

Table* new_table() {
  Table* tale = (Table*)malloc(sizeof(Table)):
  table->num_rows = 0;
  return table;
}

void free_table(Table* table) {
    free(table);
}

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void close_inputbuffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    free_table(table);
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

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {

  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    
    Row* row = &statement->row_to_insert;
    intargs_assigned = sscanf(input_buffer->buffer, "insert %d %s %s",
    &row->id, row->username, row->email);
    
    if (args_assigned < 3) {
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }
    
    return PREPARE_SUCCESS;
  }
  
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
    //TODO Implement the command here
      break;
    case (STATEMENT_SELECT):
      //TODO implement the command here 
      break;
  }
}


void repl(void){
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);
    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }
    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case (PREPARE_SUCCESS):
        printf("recognized statement\n");
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n",
               input_buffer->buffer);
        continue;
    }
     execute_statement(&statement);
     printf("Executed.\n");
  }
}
