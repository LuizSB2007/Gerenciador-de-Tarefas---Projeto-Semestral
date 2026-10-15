#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char comando[1];

    printf("====================================\n");   
    printf("GERENCIADOR DE TAREFAS - PROJETO SEMESTRAL\n");   
    printf("Digite um dos comandos abaixo para executar uma ação:\n");   
    printf("1. Adicionar tarefa\n");   
    printf("2. Listar tarefas\n");   
    printf("3. Marcar tarefa como concluída \n");   
    printf("4. Sair\n");   

    comando[0] = getchar();

    switch (comando[0]) {
        case '1':
            printf("Adicionar tarefa selecionado.\n");
            // Chamar função para adicionar tarefa
            break;
        case '2':
            printf("Listar tarefas selecionado.\n");
            // Chamar função para listar tarefas
            break;
        case '3':
            printf("Marcar tarefa como concluída selecionado.\n");
            // Chamar função para marcar tarefa como concluída
            break;
        case '4':
            printf("Saindo do programa.\n");
            exit(0);
        default:
            printf("Comando inválido. Por favor, tente novamente.\n");
    }

    return 0;
}
