#ifndef SUBTAREFAS_H
#define SUBTAREFAS_H

#include "tarefas.h"

// Vincula a nova tarefa a um pai existente//

int cadastrar_subtarefa(Tarefa tarefas[], int *quantidade, Tarefa nova, int id_pai, const char *caminho);

#endif
