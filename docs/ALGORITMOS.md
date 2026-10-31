# Algoritmos e decisões

## Rotação por consulta inversa

A máscara de cada peça é escrita uma vez em 16 caracteres. Para descobrir se uma célula está ocupada na orientação desejada, a consulta aplica `(linha, coluna) ← (3 − coluna, linha)` à coordenada de destino. Essa transformação inversa corresponde à rotação visual horária da máscara.

Quatro aplicações devolvem a mesma coordenada. O tipo O é tratado separadamente para permanecer no mesmo lugar. As tentativas de ajuste são horizontais e deliberadamente simples: 0, −1, +1, −2, +2. Isso pode rejeitar rotações no chão que outro sistema aceitaria.

## Movimento como proposta e validação

O motor copia a peça ativa, modifica a cópia e consulta colisão. Só substitui a peça real se a cópia for válida. Assim, uma tentativa contra parede ou bloco não exige desfazer uma mutação parcial.

As coordenadas das células vazias da máscara não causam colisão. As células ocupadas não podem ultrapassar o topo, as paredes ou o chão. Há uma verificação preliminar da origem para que coordenadas extremas não causem overflow antes da consulta de limites.

## Remoção de linhas

Dois índices começam no fim do tabuleiro: `source` lê as linhas e `destination` indica onde escrever a próxima linha preservada. Uma linha cheia aumenta o contador de removidas; uma linha incompleta é copiada para `destination`, que sobe. No fim, o topo restante é zerado.

Esse algoritmo trata linhas adjacentes e separadas sem saltar uma linha deslocada. Em um estado válido, cada fixação adiciona quatro células e pode completar até quatro linhas.

**Invariante após a fixação:** `ocupadas_depois + 10 × removidas = ocupadas_antes + 4`.

## Saco de sete e seed

O saco é preenchido com os sete tipos e embaralhado com Fisher–Yates. Um PRNG xorshift32 fornece valores de 32 bits; seed zero é convertida para uma constante não nula. O PRNG não é criptográfico. A redução por módulo no embaralhamento introduz pequeno viés, aceitável neste projeto didático; não se afirma uniformidade estatística exata.

Mesma seed **e mesma sequência de ações** produzem a mesma saída. No modo interativo, o relógio e o momento das teclas alteram as ações, por isso a seed sozinha não basta para repetir uma partida humana. O replay torna os passos de gravidade explícitos com `.`.

## Pontos e nível

O multiplicador de limpeza é o nível anterior à remoção. A quantidade total de linhas só é atualizada depois do cálculo dos pontos. O intervalo de gravidade é limitado a 100 ms antes da multiplicação para evitar underflow quando o nível cresce.

O estado usa contadores finitos: `uint64_t` para pontos e `unsigned` para linhas. Não há persistência, proteção contra edição externa do estado ou garantia para partidas artificiais que esgotem esses contadores.
