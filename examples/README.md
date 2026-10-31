# Partida reproduzível

```sh
make demo
```

O exemplo usa seed 42, `partida.txt` e a saída `partida-esperada.txt`. O arquivo esperado foi gerado e inspecionado nesta edição; é uma referência de regressão, não o gabarito de uma disciplina.

| Símbolo | Ação |
| --- | --- |
| `a`, `d` | Mover |
| `w`, `z` | Girar |
| `s` | Descer manualmente |
| `x` | Queda imediata |
| `.` | Um passo de gravidade |
| `p`, `r`, `q` | Pausa, reinício, saída |
| `#` | Comentário até o fim da linha |

Espaços e quebras de linha são ignorados. Não use espaço como queda no replay; use `x`. O modo replay não avança o relógio automaticamente. Após fim de jogo, ações do motor são ignoradas; `r` e `q` continuam disponíveis na aplicação.
