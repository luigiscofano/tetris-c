# Código-fonte

[Início](../README.md) · [Como executar](../README.md#como-executar) · [Aprendizagem](../README.md#o-que-desenvolvi-e-aprendi) · [Estado atual](../README.md#estado-atual)

> **Nesta área:** conteúdo da área “Código-fonte” dentro de Tetris em C · Motor, terminal e partidas reproduzíveis; use o guia inicial para localizar a execução e as relações entre módulos.

[Início](../README.md) · [Como executar](../README.md#como-executar) · [Aprendizagem](../README.md#o-que-desenvolvi-e-aprendi) · [Estado atual](../README.md#estado-atual)

> **Nesta área:** conteúdo da área “Código-fonte” dentro de Tetris em C · Motor, terminal e partidas reproduzíveis; use o guia inicial para localizar a execução e as relações entre módulos.

| Arquivo | Responsabilidade |
| --- | --- |
| [tetris.c](tetris.c) | Máscaras, PRNG, saco, colisões, rotação, fixação, linhas e score |
| [main.c](main.c) | Argumentos, comandos, leitura do replay e ciclo de gravidade |
| [terminal.c](terminal.c) | Teclado, tempo monotônico, desenho e restauração do terminal |

Para estudar, comece pelo tipo `Tetris` no header, depois `tetris_fits`, `tetris_move`, `lock_piece` e `clear_lines`. Detalhes e complexidade estão em [arquitetura](../docs/ARQUITETURA.md).

A interface Windows e a POSIX compartilham a mesma API. O motor pode ser compilado sozinho com os testes C; ele não precisa de janela nem terminal.

---

**Projeto:** [Tetris em C · Motor, terminal e partidas reproduzíveis](../README.md). Consulte a raiz para os requisitos e o primeiro comando; este guia aprofunda a área `src/`.

---

**Projeto:** [Tetris em C · Motor, terminal e partidas reproduzíveis](../README.md). Consulte a raiz para os requisitos e o primeiro comando; este guia aprofunda a área `src/`.
