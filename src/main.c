#include "main.h"
#include "tarefas.h"
#include "subtarefas.h"
#include "database.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 1 = leitura valida, 0 = entrada invalida, -1 = fim da entrada. */
static int ler_texto(const char *mensagem, char texto[], int capacidade) {
    int caractere;
    int tem_texto = 0;
    printf("%s", mensagem);
    fflush(stdout);
    if (fgets(texto, capacidade, stdin) == NULL)
        return -1;
    if (strchr(texto, '\n') == NULL) {
        caractere = getchar();
        if (caractere != '\n' && caractere != EOF) {
            while ((caractere = getchar()) != '\n' && caractere != EOF) {}
            puts("Texto muito longo.");
            return 0;
        }
    }
    texto[strcspn(texto, "\r\n")] = '\0';
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char)texto[i]))
            tem_texto = 1;
    }
    if (!tem_texto) {
        puts("Entrada vazia.");
        return 0;
    }
    return 1;
}

static int ler_numero(const char *mensagem, int *numero) {
    char texto[40];
    char *fim;
    long valor;
    int resultado = ler_texto(mensagem, texto, sizeof(texto));
    if (resultado != 1)
        return resultado;
    errno = 0;
    valor = strtol(texto, &fim, 10);
    while (isspace((unsigned char)*fim))
        fim++;
    if (errno == ERANGE || fim == texto || *fim != '\0' ||
        valor < INT_MIN || valor > INT_MAX) {
        puts("Digite um numero inteiro valido.");
        return 0;
    }
    *numero = (int)valor;
    return 1;
}

static int ler_nova_tarefa(Tarefa *nova) {
    char prazo_entrada[32];
    int resultado = ler_texto("Descricao (ate 99 caracteres): ",
                              nova->descricao, sizeof(nova->descricao));
    if (resultado != 1)
        return resultado;
    resultado = ler_texto("Prazo (DD/MM/AAAA ou DD MM AAAA): ", prazo_entrada,
                         sizeof(prazo_entrada));
    if (resultado != 1)
        return resultado;
    if (!normalizar_data_prazo(prazo_entrada, nova->prazo,
                              sizeof(nova->prazo))) {
        puts("Data invalida.");
        return 0;
    }
    resultado = ler_numero("Prioridade (1 baixa, 2 media, 3 alta): ",
                           &nova->prioridade);
    if (resultado != 1)
        return resultado;
    if (nova->prioridade < 1 || nova->prioridade > 3) {
        puts("Prioridade invalida.");
        return 0;
    }
    return 1;
}

int main(void) {
    Tarefa tarefas[MAX_TAREFAS];
    int quantidade, opcao, id, resultado;
    const char *banco = "database/tarefas.bin";

    if (!carregar_tarefas(banco, tarefas, &quantidade)) {
        puts("Erro: banco invalido ou sem acesso. Nenhum dado foi alterado.");
        return 1;
    }
    for (;;) {
        Tarefa nova = {0};
        puts("\nGERENCIADOR DE TAREFAS");
        puts("1. Adicionar tarefa\n2. Adicionar subtarefa");
        puts("3. Listar tarefas e progresso\n4. Concluir tarefa sem filhos");
        puts("5. Exportar agenda de hoje\n0. Sair");
        resultado = ler_numero("Opcao: ", &opcao);
        if (resultado == -1 || (resultado == 1 && opcao == 0))
            break;
        if (resultado == 0)
            continue;

        if (opcao == 1 || opcao == 2) {
            if (quantidade >= MAX_TAREFAS) {
                puts("Limite de 100 tarefas atingido.");
                continue;
            }
            id = 0;
            if (opcao == 2) {
                resultado = ler_numero("ID do pai: ", &id);
                if (resultado == -1)
                    break;
                if (resultado == 0)
                    continue;
                if (id < 1 || id > quantidade) {
                    puts("ID do pai inexistente.");
                    continue;
                }
                if (tarefas[id - 1].concluida)
                    puts("Aviso: o pai passara a depender das subtarefas.");
            }
            resultado = ler_nova_tarefa(&nova);
            if (resultado == -1)
                break;
            if (resultado == 0)
                continue;
            if (opcao == 1)
                resultado = cadastrar_tarefa(tarefas, &quantidade, nova, banco);
            else
                resultado = cadastrar_subtarefa(tarefas, &quantidade, nova,
                                                id, banco);
            if (resultado == -1) {
                puts("Erro ao salvar. Verifique a pasta database e as permissoes.");
                return 1;
            }
            if (resultado == 1)
                printf("Tarefa %d cadastrada.\n", quantidade);
        } else if (opcao == 3) {
            listar_tarefas(tarefas, quantidade);
        } else if (opcao == 4) {
            resultado = ler_numero("ID a concluir: ", &id);
            if (resultado == -1)
                break;
            if (resultado == 1 &&
                concluir_tarefa(tarefas, quantidade, id, banco) == -1) {
                puts("Erro ao consultar/salvar o banco. Programa encerrado.");
                return 1;
            }
        } else if (opcao == 5) {
            char hoje[11];
            time_t agora = time(NULL);
            struct tm *data = localtime(&agora);
            if (data == NULL || strftime(hoje, sizeof(hoje), "%Y-%m-%d", data) == 0) {
                puts("Erro ao obter a data local.");
            } else if (exportar_agenda("database/agenda_hoje.txt", tarefas,
                                      quantidade, hoje)) {
                puts("Agenda salva em database/agenda_hoje.txt.");
            } else {
                puts("Erro ao exportar. Verifique a pasta database e as permissoes.");
            }
        } else {
            puts("Opcao invalida.");
        }
    }
    puts("Saindo.");
    return 0;
}
