# Instalação e execução

## Requisitos

- Compilador C11: GCC, Clang ou compatível.
- GNU Make.
- Python 3.9+ para os testes de integração e ferramentas.
- Terminal com sequências ANSI; tamanho recomendado mínimo de 80 × 30.
- Não usa ncurses, SDL, bibliotecas de áudio ou pacotes Python externos.

## Linux

Com compilador, Make e Python instalados:

```sh
make
./build/tetris
./build/tetris --seed 42
make test
```

Para escolher compilador: `make clean` e depois `make test CC=clang`.

## macOS

Use as ferramentas de linha de comando do Xcode para obter compilador e Make e tenha Python 3 disponível. Execute os mesmos comandos de Linux. O job macOS do CI informa a validação automatizada; os executáveis não são binários universais distribuídos.

## Windows

Use o terminal **UCRT64 do MSYS2**, com GCC, Make e Python disponíveis:

```sh
make CC=gcc
./build/tetris.exe --seed 42
make test CC=gcc
```

A adaptação Windows usa console Win32 e `_getch`, em vez de termios. O executável precisa de um console com suporte a processamento de terminal virtual. O CI compila e testa motor e replay no Windows; o teste automatizado de pseudoterminal é POSIX e fica marcado como ignorado nessa plataforma. Uma sessão humana de console Windows ainda é uma verificação manual distinta.

## Compilar sem Make

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -Iinclude src/main.c src/terminal.c src/tetris.c -o tetris
./tetris --seed 42
```

No Windows, use GCC no UCRT64 e saída `tetris.exe`.

## Demonstração sem interação

```sh
make demo
./build/tetris --seed 42 --replay examples/partida.txt
```

O replay lê ações lógicas, sem consultar o relógio, imprime o tabuleiro final e um resumo. Ele funciona com stdout redirecionado e não emite cores ANSI. Para comparar a referência:

```sh
./build/tetris --seed 42 --replay examples/partida.txt > build/replay.txt
diff -u examples/partida-esperada.txt build/replay.txt
```

## Problemas frequentes

| Sintoma | Ação |
| --- | --- |
| Mensagem pedindo terminal interativo | Abra um terminal ou use `--replay`; não execute o modo interativo com saída capturada |
| Tabuleiro quebra de linha | Aumente o terminal para pelo menos 80 × 30 |
| Caracteres de escape visíveis | Use um terminal compatível com ANSI |
| Arquivo de replay não encontrado | Execute na raiz ou informe caminho válido |
| Mudou CFLAGS e o binário não recompilou | Execute `make clean` antes de recompilar |
| LeakSanitizer falha sob sandbox/ptrace | Para esse ambiente, `ASAN_OPTIONS=detect_leaks=0 make sanitize`; isso desliga apenas a análise de vazamentos |

`make sanitize` usa AddressSanitizer e UndefinedBehaviorSanitizer em ambientes compatíveis. Não é um requisito para jogar e não existe um alvo de sanitização MSVC.
