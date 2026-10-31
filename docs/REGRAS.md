# Regras e controles

## Tabuleiro e peças

O campo tem 10 colunas e 20 linhas visíveis. Existem sete formatos de quatro células: I, O, T, S, Z, J e L. Cada saco contém os sete tipos uma vez, embaralhados antes de sua retirada. O painel mostra a próxima peça.

A peça nasce com origem `(3, 0)`, orientação zero. As células vazias da máscara 4 × 4 podem ficar fora da área; as quatro células ocupadas precisam estar dentro do tabuleiro e livres. Não existem linhas ocultas acima do topo.

## Movimento e rotação

A/D ou setas laterais deslocam uma coluna. W/↑ gira no sentido horário e Z no anti-horário. O quadrado O mantém sua posição ocupada. As demais peças giram na matriz 4 × 4.

Se uma rotação colidir, o motor tenta deslocamentos horizontais de 0, −1, +1, −2 e +2 colunas. Sem posição válida, a peça permanece como estava. Não existem tentativas verticais nem tabelas SRS.

S/↓ desce uma linha; se não puder descer, fixa a peça. Espaço desce até o último local válido e fixa imediatamente. A gravidade também fixa imediatamente quando o próximo passo é bloqueado: não existe atraso de fixação.

## Pontuação e velocidade

| Ação | Pontos |
| --- | --- |
| Descida manual de uma célula | 1 |
| Queda imediata de uma célula | 2 |
| Remover 1 / 2 / 3 / 4 linhas | 100 / 300 / 500 / 800 × nível anterior à remoção |
| Descida pela gravidade | 0 |

O nível é `linhas / 10 + 1`, com divisão inteira. O intervalo de gravidade começa em 800 ms e diminui 50 ms por nível até o piso de 100 ms, atingido no nível 15. A interface avança um passo por intervalo observado; atrasos do sistema não são compensados por uma sequência de quedas acumuladas.

## Pausa, fim e reinício

P alterna a pausa. Durante a pausa, movimentos, rotações, gravidade e quedas não alteram o motor. O jogo termina quando uma nova peça não cabe na posição inicial. Nesse estado, só reinício ou saída têm efeito na aplicação.

R reinicia com a mesma seed e zera tabuleiro, score e linhas. Q encerra. Ctrl+C e SIGTERM também solicitam encerramento com restauração do terminal em POSIX. SIGKILL ou interrupção abrupta do sistema não permitem limpeza pelo programa; em POSIX, `stty sane` pode recuperar um terminal afetado.

## Limites intencionais

Não há hold, ghost, SRS, T-spin, combos, som, multiplayer ou persistência de recordes. O projeto privilegia regras pequenas e explícitas para estudo. Não reivindica conformidade com diretrizes de jogos comerciais.
