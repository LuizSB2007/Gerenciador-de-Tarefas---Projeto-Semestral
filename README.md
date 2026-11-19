# Gerenciador de tarefas com subtarefas — SI203

MVP em C do tema **T5**. Usa terminal, vetor de até 100 registros e os módulos já existentes, sem dependências externas.

## Compilar e executar

No PowerShell, entre na pasta APP:

```powershell
cd "C:\Users\Luiz\Desktop\UNICAMP\SI203\TRABALHO_SEMESTRAL\APP"
& "C:\mingw64\bin\mingw32-make.exe" "CC=C:/mingw64/bin/gcc.exe"
.\gerenciador.exe
```

Se GCC e GNU Make estiverem no PATH, também funciona com `make`.
A compilação usa `-Wall -Wextra -std=c11 -g`.

No VS Code, abra **a pasta APP**. Ctrl+Shift+B compila todos os módulos; F5 executa com o depurador GDB do MinGW já instalado.

Execute sempre com APP como diretório atual. A pasta `database` já existe e precisa permitir gravação. O programa não depende do antigo `database/database.txt`; esse arquivo foi preservado.

## Operações

| Opção | Operação |
|---|---|
| 1 | Adicionar tarefa principal |
| 2 | Adicionar subtarefa, informando o ID do pai |
| 3 | Listar a hierarquia com prazo, prioridade e progresso |
| 4 | Concluir uma tarefa ou subtarefa sem filhos |
| 5 | Exportar a agenda da data local do computador |
| 0 | Sair |

Descrição: até 99 bytes (letras acentuadas podem ocupar mais de um byte).
Prazo: `DD/MM/AAAA` ou `DD MM AAAA` (por exemplo `20/11/2026` ou `20 11 2026`), com dia e mês válidos; datas passadas são aceitas.
Prioridade: 1 baixa, 2 média ou 3 alta.

IDs são sequenciais. Tarefas e subtarefas compartilham o limite de 100 registros.
Não há edição geral, exclusão, mudança de pai nem ordenação por prazo/prioridade.
O menu informa entradas inválidas e volta à seleção. Fim da entrada encerra o programa.

## Progresso e recursividade

Uma folha, isto é, uma tarefa sem filhos, vale 0% ou 100%.
O progresso de uma tarefa com filhos é a média dos percentuais de seus filhos diretos.
Cada filho direto tem o mesmo peso, independentemente da quantidade de descendentes.
A listagem mostra o percentual arredondado, mas o cálculo usa `float`.

Exemplo: dois filhos com 100% e 0% deixam o pai em 50%.
A opção 4 conclui apenas folhas; pais chegam a 100% quando seus filhos chegam a 100%.
Se uma folha concluída receber uma subtarefa, sua conclusão manual é zerada e seu progresso passa a depender dos filhos.

A função `calcular_progresso` é recursiva porque cada subtarefa pode ter sua própria hierarquia.
O caso base é a folha. Para um pai, a função chama a si mesma para cada filho e calcula a média.
Uma solução iterativa precisaria controlar a ordem de visita ou uma pilha explicitamente.
Com no máximo 100 registros e pais sempre anteriores aos filhos, a recursão é simples e limitada.
A listagem da hierarquia também utiliza recursão.

## Módulos e fluxo de dados

| Módulo (.h/.c) | Responsabilidade |
|---|---|
| main | Menu, leitura e validação das entradas |
| tarefas | Struct Tarefa, datas, cadastro, listagem e conclusão |
| subtarefas | Validar pai e reaproveitar o cadastro para vincular filhos |
| completude | Calcular o progresso recursivamente |
| database | Carregar registros, consultar/gravar por ID e exportar agenda |

Fluxo: entrada no main → operações de tarefas/subtarefas → database → arquivo.
Listagem e exportação consultam completude para calcular o progresso.

O vetor e sua quantidade ficam em `main` e são passados por parâmetros. Não há variáveis globais.
A struct única contém `id`, `id_pai`, `descricao[100]`, `prazo[11]`, `prioridade` e `concluida`.
Pai 0 indica tarefa principal; outros pais precisam existir antes do filho, impedindo ciclos.

## Persistência

- **database/tarefas.bin**: registros fixos de tamanho `sizeof(Tarefa)`, lidos com `fread` e escritos com `fwrite`.
- **Acesso direto**: `fseek` posiciona a consulta e a gravação no deslocamento `(id - 1) * sizeof(Tarefa)`. Concluir uma folha atualiza somente seu registro.
- **database/agenda_hoje.txt**: exportação legível das tarefas e subtarefas com prazo igual à data local, incluindo as concluídas. Mostra ID, pai, descrição, prioridade e progresso. Cada exportação substitui o relatório anterior.
- Cadastros e conclusões são salvos imediatamente. Ao reabrir, o programa recupera o vetor do binário.
- Banco inexistente inicia vazio; é criado na primeira gravação. Banco com tamanho, campos ou vínculos inválidos causa encerramento sem gravação.
- Falhas de leitura/gravação são informadas; em erro de persistência de uma operação, o programa encerra para evitar continuar com estado divergente. Uma falha durante uma escrita pode exigir recuperação do arquivo; não há transações nem recuperação automática.
- O formato binário usa a representação da struct deste compilador. Não é um formato para intercâmbio entre plataformas.

## Roteiro curto de demonstração

Faça o roteiro em um banco inicialmente vazio. Para testar sem mexer nos dados de uso, copie o executável para outra pasta e crie uma subpasta `database` nela.

1. Opção 1: descrição `Trabalho`, prazo `2026-11-19`, prioridade `3`. Esperado: tarefa 1 cadastrada.
2. Opção 2: pai `1`, descrição `Implementar`, prazo `2026-11-05`, prioridade `3`. Esperado: tarefa 2 cadastrada.
3. Opção 2: pai `1`, descrição `Apresentar`, prazo `2026-11-19`, prioridade `2`. Esperado: tarefa 3 cadastrada.
4. Opção 3: pai e filhos aparecem com 0%.
5. Opção 4, ID `2`; opção 3: Implementar aparece com 100% e Trabalho com 50%.
6. Opção 0; execute novamente e escolha 3: os mesmos dados e percentuais devem reaparecer.
7. Opção 4, ID `3`; opção 3: Trabalho chega a 100%.
8. Opção 5: confira `database/agenda_hoje.txt`. Em 05/11/2026, inclui Implementar; em outras datas, inclui somente os prazos correspondentes ou informa agenda vazia.
9. Digite `abc` ou opção `99`: mensagem de entrada/opção inválida. Tente pai `999`, prioridade `4` e prazo `2026-02-29`: devem ser recusados.
10. Adicione uma subtarefa da tarefa 2: o programa avisa que o progresso dela passa a depender da nova folha. Concluir essa nova folha restaura os percentuais anteriores.

## Verificação realizada em 05/11/2026

Compilação concluída sem avisos com as opções acima.
25 verificações passaram em pastas temporárias separadas: cadastro, hierarquia com três níveis, 0/50/100%, persistência ao reabrir, atualização de um registro sem modificar os demais, exportação filtrada, agenda vazia, validação de entradas/datas, limite de 100, fim da entrada, banco corrompido/cíclico e falhas de abertura/gravação/exportação.
Os dados da pasta de uso não receberam tarefas de teste.

## Entrega acadêmica

Este MVP e o README cobrem o programa e suas instruções.
O material da disciplina também pede o documento de escopo da E1 e o cartaz de arquitetura para a apresentação final. Esses itens e os dados do grupo ainda precisam ser preparados para a entrega.
