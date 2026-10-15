#ifndef SUBTAREFAS_H
#define SUBTAREFAS_H

typedef struct {
    int id;
    int tarefa_id;  // ID da tarefa à qual pertence
    char descricao[100];
    int concluida;
} Subtarefa;

#endif