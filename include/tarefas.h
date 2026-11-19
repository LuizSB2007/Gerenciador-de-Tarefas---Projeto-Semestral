#ifndef TAREFAS_H
#define TAREFAS_H

#include <stddef.h>

/* Limite compartilhado por tarefas principais e subtarefas. */
#define MAX_TAREFAS 100

/* Um registro representa tanto uma tarefa quanto uma subtarefa.
   id_pai = 0 indica tarefa principal; outro valor indica seu pai. */
typedef struct {
    int id;
    int id_pai;
    char descricao[100];
    char prazo[11];
    int prioridade;
    int concluida;
} Tarefa;

/* Declaracoes das operacoes implementadas em src/tarefas.c. */
int data_valida(const char *data);
int tem_filhos(const Tarefa tarefas[], int quantidade, int id);
int cadastrar_tarefa(Tarefa tarefas[], int *quantidade, Tarefa nova, const char *caminho);
void listar_tarefas(const Tarefa tarefas[], int quantidade);
int concluir_tarefa(Tarefa tarefas[], int quantidade, int id, const char *caminho);
int normalizar_data_prazo(const char *entrada, char saida[], size_t tamanho);

#endif
