#ifndef DATABASE_H
#define DATABASE_H

#include "tarefas.h"

void init_database();
void add_tarefa(const Tarefa tarefa);
void list_tarefas();
void update_tarefa(const Tarefa tarefa);
void delete_tarefa(const int id);

#endif