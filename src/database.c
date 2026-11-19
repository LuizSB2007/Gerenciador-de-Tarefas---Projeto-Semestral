#include "database.h"
#include "completude.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

int carregar_tarefas(const char *caminho, Tarefa tarefas[], int *quantidade) {
    FILE *arquivo = fopen(caminho, "rb");
    long tamanho;
    int valido = 1;
    *quantidade = 0;
    if (arquivo == NULL)
        return errno == ENOENT;
    if (fseek(arquivo, 0, SEEK_END) != 0) {
        fclose(arquivo);
        return 0;
    }
    tamanho = ftell(arquivo);
    if (tamanho < 0 || tamanho % (long)sizeof(Tarefa) != 0 ||
        tamanho / (long)sizeof(Tarefa) > MAX_TAREFAS) {
        fclose(arquivo);
        return 0;
    }
    rewind(arquivo);
    *quantidade = (int)(tamanho / (long)sizeof(Tarefa));
    for (int i = 0; i < *quantidade; i++) {
        Tarefa *t = &tarefas[i];
        if (fread(t, sizeof(Tarefa), 1, arquivo) != 1) {
            valido = 0;
            break;
        }
        /* Pais sempre precedem filhos: isso tambem impede ciclos. */
        if (t->id != i + 1 || t->id_pai < 0 || t->id_pai >= t->id ||
            memchr(t->descricao, '\0', sizeof(t->descricao)) == NULL ||
            t->descricao[0] == '\0' ||
            memchr(t->prazo, '\0', sizeof(t->prazo)) == NULL ||
            !data_valida(t->prazo) || t->prioridade < 1 || t->prioridade > 3 ||
            (t->concluida != 0 && t->concluida != 1)) {
            valido = 0;
            break;
        }
    }
    if (fclose(arquivo) != 0)
        valido = 0;
    return valido;
}

int ler_tarefa(const char *caminho, int id, Tarefa *tarefa) {
    FILE *arquivo = fopen(caminho, "rb");
    int sucesso;
    if (arquivo == NULL || id < 1 || id > MAX_TAREFAS) {
        if (arquivo != NULL)
            fclose(arquivo);
        return 0;
    }
    sucesso = fseek(arquivo, (long)(id - 1) * sizeof(Tarefa), SEEK_SET) == 0 &&
              fread(tarefa, sizeof(Tarefa), 1, arquivo) == 1;
    if (fclose(arquivo) != 0)
        sucesso = 0;
    return sucesso && tarefa->id == id;
}

int salvar_tarefa(const char *caminho, const Tarefa *tarefa) {
    FILE *arquivo;
    int sucesso;
    if (tarefa->id < 1 || tarefa->id > MAX_TAREFAS)
        return 0;
    arquivo = fopen(caminho, "r+b");
    if (arquivo == NULL && errno == ENOENT)
        arquivo = fopen(caminho, "w+b");
    if (arquivo == NULL)
        return 0;
    sucesso = fseek(arquivo, (long)(tarefa->id - 1) * sizeof(Tarefa),
                    SEEK_SET) == 0 &&
              fwrite(tarefa, sizeof(Tarefa), 1, arquivo) == 1;
    if (fclose(arquivo) != 0)
        sucesso = 0;
    return sucesso;
}

int exportar_agenda(const char *caminho, const Tarefa tarefas[],
                    int quantidade, const char *data) {
    FILE *arquivo = fopen(caminho, "w");
    int itens = 0;
    int sucesso;
    if (arquivo == NULL)
        return 0;
    fprintf(arquivo, "Agenda de %s\n", data);
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(tarefas[i].prazo, data) == 0) {
            fprintf(arquivo, "ID %d | pai %d | %s | prioridade %d | %.0f%%\n",
                    tarefas[i].id, tarefas[i].id_pai, tarefas[i].descricao,
                    tarefas[i].prioridade,
                    calcular_progresso(tarefas, quantidade, tarefas[i].id));
            itens++;
        }
    }
    if (itens == 0)
        fprintf(arquivo, "Nenhuma tarefa para hoje.\n");
    sucesso = !ferror(arquivo);
    if (fclose(arquivo) != 0)
        sucesso = 0;
    return sucesso;
}
