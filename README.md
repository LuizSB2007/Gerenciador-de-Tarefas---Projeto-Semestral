# Gerenciador de tarefas com subtarefas

Projeto semestral em C para a disciplina SI203. O tema escolhido é o **T5 — Gerenciador de tarefas com subtarefas**.

## Proposta

O programa deverá permitir organizar tarefas hierárquicas com prazo e prioridade, associar subtarefas e acompanhar o percentual de conclusão. O progresso de uma tarefa será calculado recursivamente a partir de suas subtarefas.

Os dados deverão persistir entre execuções: a hierarquia será armazenada em arquivo binário com acesso direto, e a agenda do dia poderá ser exportada para arquivo-texto.

## Organização prevista

- `src/main.c`: menu e interação com a pessoa usuária.
- `src/tarefas.c` e `include/tarefas.h`: operações sobre tarefas.
- `src/subtarefas.c` e `include/subtarefas.h`: operações sobre subtarefas.
- `src/completude.c` e `include/completude.h`: cálculo recursivo do progresso.
- `src/database.c` e `include/database.h`: persistência e acesso aos arquivos.

O projeto deverá usar structs e uma coleção de registros, separar os módulos em arquivos `.h` e `.c`, evitar variáveis globais e tratar entradas inválidas e falhas ao abrir arquivos.

## Compilação

Com GCC e GNU Make instalados, abra o terminal na pasta raiz do projeto e execute:

```sh
make
```

No ambiente MinGW configurado neste projeto, se `make` não for reconhecido, use no PowerShell:

```powershell
C:\mingw64\bin\mingw32-make.exe
```

O Makefile compila os arquivos `.c` de `src/` e gera `gerenciador.exe`. Para executar no Windows, use:

```powershell
.\gerenciador.exe
```

Para apagar o executável e os arquivos intermediários `.o`, execute:

```sh
make clean
```

Ou, no PowerShell com o MinGW acima:

```powershell
C:\mingw64\bin\mingw32-make.exe clean
```

Se o comando `gcc` não for encontrado, confira se a pasta do MinGW que contém `gcc.exe` está no `PATH`.

## Estado atual e teste rápido

O código ainda está em desenvolvimento. Por enquanto, o menu exibe as opções de adicionar, listar e concluir tarefas, mas essas operações ainda não estão implementadas. Para testar o menu, compile e execute o programa, digite `4` e pressione Enter para sair. As outras opções apenas exibem uma mensagem de seleção.