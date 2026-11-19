#include "subtarefas.h"
#include "database.h"
#include <stdio.h>

int cadastrar_subtarefa(Tarefa tarefas[], int *quantidade, Tarefa nova,
                        int id_pai, const char *caminho) {
    int resultado;
    /* Uma subtarefa e uma Tarefa comum ligada ao ID de outra tarefa. */
    if (id_pai < 1 || id_pai > *quantidade) {
        puts("ID do pai inexistente.");
        return 0;
    }
    nova.id_pai = id_pai;
    resultado = cadastrar_tarefa(tarefas, quantidade, nova, caminho);
    /* Se uma folha concluida ganha filhos, seu progresso passa a depender deles. */
    if (resultado == 1 && tarefas[id_pai - 1].concluida) {
        Tarefa pai = tarefas[id_pai - 1];
        pai.concluida = 0;
        if (!salvar_tarefa(caminho, &pai))
            return -1;
        tarefas[id_pai - 1] = pai;
        puts("O progresso do pai agora depende das subtarefas.");
    }
    return resultado;
}
