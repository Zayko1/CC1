#include <stdio.h>

typedef struct{
	int id;
	char nom[32];
	char prenom[32];
	char email[255];
}Row;

typedef struct{
	Row rows[1000];
	int num_rows;
}Table;


