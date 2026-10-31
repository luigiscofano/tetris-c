# Objetivos pedagógicos

**Aprender a decompor um problema visual em dados, regras, transições e evidências de correção.** O jogo oferece um contexto concreto para fundamentos de C e engenharia de software.

## Competências e evidências

| Objetivo de aprendizagem | Onde observar | Atividade proposta | Evidência esperada |
| --- | --- | --- | --- |
| Usar matrizes e índices com segurança | `Tetris.board`, `tetris_fits` | Desenhar uma peça na borda e justificar os índices válidos | Explicar por que não há acesso fora do tabuleiro |
| Distinguir representação de apresentação | `tetris.c` e `terminal.c` | Executar uma partida sem abrir terminal interativo | Mesmo motor usado pelo replay e pelo jogo |
| Aplicar transformações discretas | `tetris_cell` | Traçar quatro rotações de T e I | Quatro células preservadas e retorno à orientação inicial |
| Modelar estados e transições | pausa, fim de jogo, reinício | Definir quais ações são permitidas em cada estado | Teste mostrando pausa e top-out bloqueando mudanças |
| Atualizar dados de forma atômica | movimento e rotação | Tentar uma jogada que colide | Estado anterior preservado |
| Projetar algoritmos de compactação | `clear_lines` | Remover linhas adjacentes e separadas | Ordem preservada e linhas novas vazias no topo |
| Entender aleatoriedade reproduzível | PRNG e saco de sete | Repetir seed e comandos | Saída final idêntica |
| Formular invariantes | teste de stress | Contar células antes e depois de fixar uma peça | `depois + 10 × linhas_removidas = antes + 4` |
| Escrever testes por comportamento | testes C e Python | Criar um caso de borda e demonstrar falha antes da correção | Teste pequeno, reproduzível e com critério explícito |
| Integrar programa e sistema operacional | terminal POSIX | Pausar, reiniciar e encerrar por sinal | Cursor e configuração do terminal restaurados |

## Sequência sugerida de estudo

1. **Representação:** ler `include/tetris.h`, identificar peça ativa, células fixadas e próxima peça.
2. **Movimento:** acompanhar uma translação de uma coluna e a consulta de colisão.
3. **Rotação:** calcular a transformação em papel para uma matriz 4 × 4.
4. **Fixação:** explicar por que só se grava a peça após validar sua posição.
5. **Compactação:** simular a cópia de linhas com os índices `source` e `destination`.
6. **Estados:** escrever uma tabela com ações durante jogo, pausa e fim de jogo.
7. **Teste:** executar `make test` e acrescentar um caso sem alterar a interface.
8. **Extensão:** propor uma regra, escrever seu contrato e só depois implementar.

## Perguntas para apresentação ou avaliação

- Por que a peça ativa não fica gravada no tabuleiro a cada movimento?
- O que acontece se a compactação percorrer as linhas na direção errada?
- A mesma seed garante a mesma partida se os comandos forem diferentes?
- Por que um gerador determinístico ajuda a investigar um bug?
- Qual diferença existe entre um teste unitário, um replay e um teste do terminal?
- Passar em 200 mil ações prova que o programa não tem nenhum bug? Por quê?

## Critérios de avaliação sugeridos

| Dimensão | O que avaliar |
| --- | --- |
| Correção | Regras e invariantes preservadas em casos normais e de borda |
| Explicação | Capacidade de justificar coordenadas, rotação e compactação |
| Projeto | Separação entre motor e terminal; contratos claros |
| Reprodutibilidade | Seed, replay e instruções suficientes para repetir um resultado |
| Comunicação | Documentação coerente com o código e reconhecimento de limites |

Esses são objetivos e propostas de estudo. Não foi feito experimento com participantes para medir ganhos de aprendizagem. O repositório não atribui ao usuário uma nota acadêmica nem afirma equivalência com um enunciado não recuperado.
