# Validação e limites

## Comandos reproduzíveis

```sh
make clean
make test
make sanitize
```

O build normal exige C11 e ativa `-Wall -Wextra -Wpedantic -Werror`. Python usa apenas a biblioteca padrão. `make sanitize` recompila com AddressSanitizer e UndefinedBehaviorSanitizer.

## O que os testes verificam

| Camada | Critérios |
| --- | --- |
| Motor: 12 grupos C | Quatro células por peça/rotação; seeds; sacos de sete; bordas; colisão atômica; rotações e ajustes; quedas; fixação; limpeza de 1–4 linhas; compactação; score/nível; pausa e top-out |
| Stress | 100 seeds × 2.000 ações; células 0–7; peça ativa válida; conservação de ocupação/linhas |
| CLI: 6 testes Python | Ajuda/versão; seeds inválidas e extremas; argumentos; erros de arquivo; fixture exata; comandos inválidos; saída sem ANSI |
| Terminal: 2 testes POSIX | Teclas, pausa, reinício e encerramento; restauração de termios, cursor e tela alternativa; encerramento por SIGTERM |
| Demonstração | Replay de seed 42 comparado byte a byte com saída versionada |
| Preview | SVG regenerado do replay; CI rejeita divergência com o arquivo versionado |

A saída de referência foi gerada da implementação desta edição e inspecionada; ela detecta regressões, mas não é um gabarito independente das regras. Os testes C usam cenários explícitos para fornecer essa verificação adicional.

## Resultados locais desta edição

Em 31/10/2026, passaram a compilação estrita, os 12 grupos C, as 200 mil ações, os seis testes CLI e os dois testes POSIX. ASan/UBSan foram executados; no ambiente local, a análise de vazamentos foi desativada porque LeakSanitizer não consegue inspecionar os processos sob a sandbox. Não houve desativação de ASan/UBSan.

## Automação no GitHub

O workflow [Verificacao C](https://github.com/luigiscofano/tetris-c/actions/workflows/ci.yml) executa:

- Linux e macOS com os comandos de compilador `gcc` e `clang` (no macOS, `gcc` pode ser o alias do Clang da Apple).
- ASan/UBSan nos jobs Linux.
- Build e testes de motor/CLI no Windows UCRT64; os testes de pseudoterminal são explicitamente ignorados nesse sistema.
- Exportação de logs, demonstração e SVG como artefatos da execução.

O status de cada execução é a evidência atual; uma lista de plataformas no workflow não significa aprovação antes de executar.

## Limites

- Não há teste humano de conforto, legibilidade ou jogabilidade em todas as plataformas.
- Console Windows interativo e todos os emuladores de terminal não foram verificados manualmente.
- Testes de teclado chamam a interface real por pseudoterminal; não cobrem todos os layouts, modificadores e sequências de escape.
- Não há verificação exaustiva de todos os tabuleiros possíveis nem prova formal de correção.
- Não há SRS nem conformidade com diretrizes comerciais.
- Não foi recuperado enunciado acadêmico para demonstrar equivalência com uma entrega da disciplina.
- Não foi realizada avaliação pedagógica com participantes.
