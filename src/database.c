#include "database.h"
#include <stdio.h>
#include <string.h>

void init_database(){
    FILE *file = fopen("database.txt", "r");   

    if (file == NULL) file = fopen("database.txt", "w"); //verifica se o arquivo existe, caso não exista cria um novo
    else fclose(file);

    printf("Banco de dados inicializado com sucesso\n");
}

void add_tarefa(const Tarefa tarefa){
    FILE *file = fopen("database.txt", "a"); //abre o arquivo para escrita no final do arquivo
    if(file == NULL){
        printf("Erro ao abrir o arquivo de banco de dados. \n");
        return;
    }
    fprintf(file, "id:%d;descricao:%s;concluida:%d\n", tarefa.id, tarefa.descricao, tarefa.concluida);
    fclose(file);
}

void list_tarefas(){
    FILE *file = fopen("database.txt", "r"); //abre o arquivo para leitura
    if(file == NULL){
        printf("Erro ao abrir o arquivo de banco de dados. \n");
        return;
    }
    char line[256];
    while(fgets(line, sizeof(line), file)){
        printf("%s", line);
    }
    fclose(file);
}

void update_tarefa(const Tarefa tarefa){
    FILE *file = fopen("database.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if(file == NULL || temp == NULL){
        printf("Erro ao abrir o arquivo de banco de dados. \n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    char line[256];
    int id_lido;

    while (fgets(line, sizeof(line), file)) {
        // Extrai apenas o número do ID sem alterar a string 'line'
        if (sscanf(line, "id:%d", &id_lido) == 1 && id_lido == tarefa.id) {
            // Se for o is buscado, escreve a versao atualizada
            fprintf(temp, "id:%d;descricao:%s;concluida:%d\n", 
                    tarefa.id, tarefa.descricao, tarefa.concluida);
        } else {
            // Se nao for, mantem a linha original intacta
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove("database.txt");
    rename("temp.txt", "database.txt");
}

void delete_tarefa(const int id){
    FILE *file = fopen("database.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if(file == NULL || temp == NULL){
        printf("Erro ao abrir o arquivo de banco de dados. \n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    char line[256];
    int id_lido;

    while (fgets(line, sizeof(line), file)) {
        // verifica se é a linha para exclusao, caso sim, apenas ignora a linha
        if (sscanf(line, "id:%d", &id_lido) == 1 && id_lido == id) continue;
        else fputs(line, temp);
    }

    fclose(file);
    fclose(temp);

    remove("database.txt");
    rename("temp.txt", "database.txt");
}