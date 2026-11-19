#include "completude.h"

float calcular_progresso(const Tarefa tarefas[], int quantidade, int id) {
    float soma = 0;
    int filhos = 0;

    /* Passo recursivo: calcula o progresso de cada filho da tarefa atual. */
    for (int i = 0; i < quantidade; i++) {
        if (tarefas[i].id_pai == id) {
            soma += calcular_progresso(tarefas, quantidade, tarefas[i].id);
            filhos++;
        }
    }
    /* Cada filho direto tem o mesmo peso na media do pai. */
    if (filhos > 0)
        return soma / filhos;

    /* Caso base: folha pendente ou concluida. */
    return tarefas[id - 1].concluida ? 100.0f : 0.0f;
}
