# Interfaces e contratos

[Início](../README.md) · [Como executar](../README.md#como-executar) · [Aprendizagem](../README.md#o-que-desenvolvi-e-aprendi) · [Estado atual](../README.md#estado-atual)

> **Nesta área:** tipos e interfaces públicas; o contrato deve ser lido junto da implementação correspondente.

[Início](../README.md) · [Como executar](../README.md#como-executar) · [Aprendizagem](../README.md#o-que-desenvolvi-e-aprendi) · [Estado atual](../README.md#estado-atual)

> **Nesta área:** tipos e interfaces públicas; o contrato deve ser lido junto da implementação correspondente.

`tetris.h` expõe os tipos e as ações do motor. `terminal.h` expõe operações da interface. O chamador inicializa `Tetris` com `tetris_init` e mantém um estado válido; o ponteiro passado às funções não pode ser nulo. As funções não oferecem uma API de importação de estado arbitrário.

| Função | Retorno / efeito |
| --- | --- |
| `tetris_cell` | Consulta uma célula; coordenada/tipo inválido retorna false |
| `tetris_fits` | Verifica células ocupadas e limites sem alterar estado |
| `tetris_move` | Aceita −1/+1; informa se moveu |
| `tetris_rotate` | Aceita −1/+1; informa se encontrou orientação válida |
| `tetris_step` | Informa se desceu; false pode significar bloqueio/pausa ou fixação seguida de spawn |
| `tetris_drop` | Retorna distância de queda; pode fixar mesmo retornando zero |
| `tetris_pause` | Alterna pausa, exceto no fim de jogo |
| `tetris_init` | Reinicia o estado e define seed determinística |

A estrutura fica pública para facilitar o estudo e os cenários de teste. Código de aplicação não deve escrever diretamente no tabuleiro. `score` e `lines` são cumulativos e de tamanho finito; não existe persistência ou proteção contra adulteração externa.

---

**Projeto:** [Tetris em C · Motor, terminal e partidas reproduzíveis](../README.md). Consulte a raiz para os requisitos e o primeiro comando; este guia aprofunda a área `include/`.

---

**Projeto:** [Tetris em C · Motor, terminal e partidas reproduzíveis](../README.md). Consulte a raiz para os requisitos e o primeiro comando; este guia aprofunda a área `include/`.
