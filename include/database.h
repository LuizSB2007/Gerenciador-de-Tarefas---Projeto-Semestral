#ifndef DATABASE_H
#define DATABASE_H

#include "tarefas.h"

/* Interface de persistencia: carregar, consultar, salvar e exportar registros. */
int carregar_tarefas(const char *caminho, Tarefa tarefas[], int *quantidade);
int ler_tarefa(const char *caminho, int id, Tarefa *tarefa);
int salvar_tarefa(const char *caminho, const Tarefa *tarefa);
int exportar_agenda(const char *caminho, const Tarefa tarefas[], int quantidade, const char *data);

#endif
