#include "tarefas.h"
#include "completude.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int data_iso_valida(const char *data) {
    int ano, mes, dia;
    int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (data == NULL)
        return 0;
    if (strlen(data) != 10 || data[4] != '-' || data[7] != '-')
        return 0;
    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7 && (data[i] < '0' || data[i] > '9'))
            return 0;
    }
    if (sscanf(data, "%4d-%2d-%2d", &ano, &mes, &dia) != 3 ||
        ano < 1 || mes < 1 || mes > 12)
        return 0;
    /* Considera anos bissextos para validar o dia de fevereiro. */
    if (ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0))
        dias[1] = 29;
    return dia >= 1 && dia <= dias[mes - 1];
}

int data_valida(const char *data) {
    char normalizada[11];

    if (data == NULL)
        return 0;
    if (data_iso_valida(data))
        return 1;
    return normalizar_data_prazo(data, normalizada, sizeof(normalizada));
}

int normalizar_data_prazo(const char *entrada, char saida[], size_t tamanho) {
    /* Converte a data digitada para AAAA-MM-DD*/
    char copia[32];
    int dia, mes, ano;

    if (entrada == NULL || saida == NULL || tamanho < 11)
        return 0;

    strncpy(copia, entrada, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';

    if (strchr(copia, '/') != NULL || strchr(copia, ' ') != NULL) {
        if (sscanf(copia, "%d/%d/%d", &dia, &mes, &ano) == 3 ||
            sscanf(copia, "%d %d %d", &dia, &mes, &ano) == 3) {
            /* aceita o padrao comum DD/MM/AAAA ou DD MM AAAA*/
        } else {
            return 0;
        }
    } else {
        return 0;
    }

    if (ano < 100) {
        if (ano <= 30)
            ano += 2000;
        else
            ano += 1900;
    }

    if (dia < 1 || mes < 1 || mes > 12)
        return 0;

    if (ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0)) {
        if (mes == 2 && dia > 29)
            return 0;
    } else if (mes == 2 && dia > 28) {
        return 0;
    }

    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30)
        return 0;
    if ((mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 ||
         mes == 10 || mes == 12) && dia > 31)
        return 0;

    snprintf(saida, tamanho, "%04d-%02d-%02d", ano, mes, dia);
    return data_iso_valida(saida);
}

int tem_filhos(const Tarefa tarefas[], int quantidade, int id) {
    /* Uma tarefa e pai quando outro registro aponta para seu ID. */
    for (int i = 0; i < quantidade; i++) {
        if (tarefas[i].id_pai == id)
            return 1;
    }
    return 0;
}

/* Retorno: 1 = sucesso, 0 = entrada invalida, -1 = falha de arquivo. */
int cadastrar_tarefa(Tarefa tarefas[], int *quantidade, Tarefa nova, const char *caminho) {
    if (*quantidade >= MAX_TAREFAS) {
        puts("Limite de 100 tarefas atingido.");
        return 0;
    }
    /* IDs seguem a ordem de cadastro; primeiro salva, depois atualiza o vetor. */
    nova.id = *quantidade + 1;
    nova.concluida = 0;
    if (!salvar_tarefa(caminho, &nova))
        return -1;
    tarefas[*quantidade] = nova;
    (*quantidade)++;
    return 1;
}

static void listar_nivel(const Tarefa tarefas[], int quantidade, int id_pai, int nivel) {
    /* Recursao: imprime cada filho e depois visita suas subtarefas. */
    for (int i = 0; i < quantidade; i++) {
        if (tarefas[i].id_pai == id_pai) {
            printf("%*s%d. %s | %s | prioridade %d | %.0f%%\n",
                   nivel * 2, "", tarefas[i].id, tarefas[i].descricao,
                   tarefas[i].prazo, tarefas[i].prioridade,
                   calcular_progresso(tarefas, quantidade, tarefas[i].id));
            listar_nivel(tarefas, quantidade, tarefas[i].id, nivel + 1);
        }
    }
}

void listar_tarefas(const Tarefa tarefas[], int quantidade) {
    if (quantidade == 0)
        puts("Nenhuma tarefa cadastrada.");
    else
        listar_nivel(tarefas, quantidade, 0, 0);
}

int concluir_tarefa(Tarefa tarefas[], int quantidade, int id, const char *caminho) {
    Tarefa tarefa;
    if (id < 1 || id > quantidade) {
        puts("ID inexistente.");
        return 0;
    }
    /* So folhas podem ser concluidas manualmente; pais usam o progresso medio. */
    if (tem_filhos(tarefas, quantidade, id)) {
        puts("Conclua as subtarefas: o progresso do pai e calculado.");
        return 0;
    }
    if (tarefas[id - 1].concluida) {
        puts("Tarefa ja concluida.");
        return 0;
    }
    /* Consulta e atualizacao usam a posicao do ID no binario. */
    if (!ler_tarefa(caminho, id, &tarefa))
        return -1;
    tarefa.concluida = 1;
    if (!salvar_tarefa(caminho, &tarefa))
        return -1;
    tarefas[id - 1] = tarefa;
    puts("Tarefa concluida.");
    return 1;
}
