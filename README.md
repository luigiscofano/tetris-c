# Tetris em C · Motor, terminal e partidas reproduzíveis

> Um jogo de blocos em C11 com sete peças, pontuação, níveis, pausa e replays determinísticos, acompanhado de testes e guias para estudar programação.

![Visão geral de Tetris em C · Motor, terminal e partidas reproduzíveis](assets/preview.svg)

[![Verificação](https://github.com/luigiscofano/tetris-c/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/luigiscofano/tetris-c/actions/workflows/ci.yml)

[O que faz](#o-que-este-projeto-faz) · [Módulos](#funcionalidades-e-módulos) · [Executar](#como-executar) · [Aprendizagem](#o-que-desenvolvi-e-aprendi) · [Estado atual](#estado-atual) · [Documentação](#documentação-e-navegação)

## O que este projeto faz

Implementa um jogo de blocos em um tabuleiro de **10 colunas por 20 linhas**. O jogador move e gira a peça que cai, completa linhas e acumula pontos. Há próxima peça, avanço de nível, pausa, reinício e um modo de reprodução por arquivo. O programa roda no terminal; não exige navegador, servidor, ncurses ou uma biblioteca gráfica externa.

## Qual problema resolve

O desafio de engenharia é transformar regras de jogo em um estado coerente e verificável: impedir colisões, distinguir a peça ativa dos blocos fixos, girar coordenadas e remover linhas sem perder outras células. Separar o motor da interface permite testar essas regras sem teclado nem relógio real.

## Funcionalidades e módulos

### 1. Motor de regras

O motor em [tetris.c](src/tetris.c) e seu [contrato](include/tetris.h) cuidam das sete peças, sorteio em sacos de sete, movimentos, rotações, colisões, fixação, remoção de linhas, pontuação e nível. Uma seed permite repetir a sequência. A rotação usa uma matriz 4×4 e tentativas horizontais simples; não implementa SRS ou todas as regras das versões comerciais.

### 2. Aplicação de terminal

[main.c](src/main.c) organiza o loop de jogo; [terminal.c](src/terminal.c) adapta entrada e saída ao sistema operacional. O modo interativo lê teclas, avança a gravidade e desenha o tabuleiro com sequências ANSI. Ele precisa de terminal real; um painel que apenas captura stdout não oferece a mesma interação.

### 3. Replay independente da interface

O mesmo motor recebe comandos de um arquivo, sem depender do tempo humano entre teclas. Uma seed e um replay permitem reproduzir uma falha e comparar a saída com uma referência. Esse modo é a primeira opção para conhecer o programa sem abrir uma sessão interativa. Ele não avança o relógio automaticamente: `.` representa um passo de gravidade e `x`, a queda imediata.

### 4. Verificação e visualização

Os testes exercitam regras, entradas inválidas, invariantes e restauração do terminal. O preview SVG da capa é produzido a partir do replay real. Essas ferramentas apoiam o jogo e não são aplicativos independentes.

## Como executar

**Requisitos:** compilador C11 (GCC/Clang), GNU Make, Python 3.9+ para testes e terminal ANSI com pelo menos 80×30. O código Python de apoio usa a biblioteca padrão.

```sh
git clone https://github.com/luigiscofano/tetris-c.git
cd tetris-c
```

### Primeiro contato, sem interação

```sh
make
make demo
make test
```

`make demo` reproduz `examples/partida.txt` com seed 42. `make test` compila e executa os testes do motor e da CLI; os testes de pseudoterminal são POSIX.

### Jogar

```sh
./build/tetris --seed 42
```

| Tecla | Ação |
| --- | --- |
| A / D ou ← / → | Mover à esquerda / direita |
| W ou ↑ / Z | Girar no sentido horário / anti-horário |
| S ou ↓ | Descer uma linha |
| Espaço | Queda imediata |
| P / R / Q | Pausar / reiniciar / sair |

### Reproduzir um arquivo e consultar a CLI

```sh
./build/tetris --seed 42 --replay examples/partida.txt
./build/tetris --help
```

No replay, use `a,d,w,z,s,x,.,p,r,q`; `#` inicia um comentário. O espaço da interface interativa não é o comando de queda do arquivo.

### Windows e compilação direta

No Windows, use o ambiente **MSYS2 UCRT64**, com GCC, Make e Python, e execute `make CC=gcc`; o binário pode ser iniciado como `./build/tetris.exe --seed 42`. O PowerShell puro não fornece GNU Make/GCC por padrão.

Sem Make, em um terminal com compilador disponível:

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -Iinclude src/main.c src/terminal.c src/tetris.c -o tetris
./tetris --seed 42
```

## O que desenvolvi e aprendi

A implementação desta edição reúne modelagem por matrizes, transformações geométricas, validação antes da mutação, compactação estável de linhas, geração pseudoaleatória reproduzível e separação entre domínio e plataforma. Os [objetivos pedagógicos](docs/OBJETIVOS_PEDAGOGICOS.md) propõem atividades para explicar e alterar cada mecanismo.

O projeto se relaciona ao Tetris em C relatado durante a graduação. O fonte original não foi recuperado; esta é uma implementação nova de portfólio, com [procedência explícita](docs/PROCEDENCIA.md).

## Estado atual

| Situação | Recursos |
| --- | --- |
| Implementado | Motor, sete peças, próxima peça, gravidade, pontuação, níveis, pausa/reinício, terminal e replay |
| Verificado | 12 grupos de testes C, stress de 200 mil ações, testes CLI e testes POSIX do terminal; detalhes na validação |
| Fora desta versão | Hold, ghost, SRS, T-spin, combos, multiplayer e ranking persistente |
| Verificação manual adicional | Conforto de uso e sessão humana no console Windows e em diferentes emuladores de terminal |

## Validação e limites

`make test` é o ponto de entrada. Em ambiente compatível, `make sanitize` recompila com AddressSanitizer e UndefinedBehaviorSanitizer. O [workflow](.github/workflows/ci.yml) inclui Linux, macOS e Windows; o resultado de cada execução informa o estado real. O replay detecta regressões, mas sua saída de referência não é um gabarito acadêmico independente.

## Documentação e navegação

| Pasta / guia | Conteúdo |
| --- | --- |
| [src/](src/README.md) e [include/](include/README.md) | Motor, terminal, aplicação e contratos |
| [examples/](examples/README.md) | Replay comentado e saída esperada |
| [tests/](tests/README.md) | Regras, stress, CLI e terminal |
| [scripts/](scripts/README.md) e [assets/](assets/README.md) | Ferramentas e preview |
| [docs/](docs/README.md) | [Instalação](docs/INSTALACAO.md), [regras](docs/REGRAS.md), [arquitetura](docs/ARQUITETURA.md), [algoritmos](docs/ALGORITMOS.md) e [validação](docs/VALIDACAO.md) |
| [historico/](historico/README.md) | Situação da recuperação do trabalho antigo |

Projeto independente de estudo, sem afiliação a produtos comerciais ou comprovação de equivalência com um enunciado antigo.


## Autoria, referências e licença

**Luigi Scofano de Araujo · Engenharia de Computação, Poli-USP · USP 13680334.** Fontes recuperados, componentes de terceiros e trabalhos em grupo mantêm os créditos indicados nos arquivos e na documentação.

[Procedência](docs/PROCEDENCIA.md) · [Como contribuir](CONTRIBUTING.md) · [Mudanças](CHANGELOG.md)

## Tópicos do repositório

`c` · `c11` · `tetris` · `terminal-game` · `game-development` · `algorithms` · `deterministic-replay` · `unit-testing` · `educational`
