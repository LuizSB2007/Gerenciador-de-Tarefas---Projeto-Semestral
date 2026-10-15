#include "database.h"
#include <stdio.h>

void init_database(){
    FILE *file = fopen("database.txt", "r");   
    if (file == NULL) file = fopen("database.txt", "w"); //verifica se o arquivo existe, caso não exista cria um novo
    else fclose(file);
    printf("Banco de dados inicializado com sucesso. \n");
    printf("Banco de dados inicializado com sucesso. \n");
}
