# Roteiro de apresentação — 6 a 8 minutos

## 1. Problema e contexto — 45 segundos

Criar um jogo de blocos em C oferece um problema pequeno, visual e cheio de casos de borda. Esta versão é uma implementação nova de portfólio; o trabalho acadêmico original não foi recuperado. O foco da apresentação é explicar as decisões verificáveis desta edição.

## 2. Objetivo pedagógico — 1 minuto

Mostrar como matrizes representam o campo, como uma transformação gera rotações e como uma regra se torna um teste. Apontar a diferença entre peça ativa e blocos fixados.

## 3. Demonstração — 1 minuto

```sh
make
./build/tetris --seed 42
```

Mover, girar, cair, pausar e reiniciar. Explicar que a seed repete a ordem das peças, mas o tempo das teclas também influencia a partida interativa.

## 4. Motor e algoritmos — 2 minutos

Abrir o diagrama em ARQUITETURA.md. Explicar proposta → validação → atualização. Mostrar a compactação de baixo para cima e o motivo de manter o motor separado do terminal.

## 5. Evidência — 1 minuto

```sh
make test
make demo
```

Mostrar um teste explícito de quatro linhas, o caso de rotação bloqueada e a conservação de células no stress. O replay e a saída de referência permitem reproduzir regressões.

## 6. Limites e próximos passos — 45 segundos

Reconhecer a rotação simplificada, ausência de hold/ghost e falta do fonte original. Propor uma extensão pequena com teste e contrato, como uma projeção de queda calculada sem modificar o motor.

## Perguntas para discussão

- Como adicionar outra interface sem reescrever as regras?
- Qual bug um teste de linhas não adjacentes encontra?
- Por que não alegar correção absoluta após o stress?

A demonstração deve usar o código e os resultados reais, sem apresentar avaliações acadêmicas ou revisões externas que não aconteceram.
