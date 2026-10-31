# Testes

[Início](../README.md) · [Como executar](../README.md#como-executar) · [Aprendizagem](../README.md#o-que-desenvolvi-e-aprendi) · [Estado atual](../README.md#estado-atual)

> **Nesta área:** testes e seus critérios de aprovação; um resultado deve ser interpretado dentro da cobertura documentada.

[Início](../README.md) · [Como executar](../README.md#como-executar) · [Aprendizagem](../README.md#o-que-desenvolvi-e-aprendi) · [Estado atual](../README.md#estado-atual)

> **Nesta área:** testes e seus critérios de aprovação; um resultado deve ser interpretado dentro da cobertura documentada.

Execute `make test` na raiz. Não compile os testes C com `-DNDEBUG`, pois seus critérios usam `assert`.

- `test_tetris.c`: 12 grupos de cenários explícitos e stress de 200 mil ações. Inclui linhas adjacentes/separadas, fronteiras, rotação com ajuste, fixação, score e top-out.
- `test_cli.py`: seis testes da aplicação real, erros e reprodução exata de uma partida.
- `test_terminal.py`: dois testes da interface real em pseudoterminal POSIX; são ignorados no Windows.

`make sanitize` recompila e repete a suíte com sanitizadores. Os testes não dependem de rede, arquivos de usuários, bibliotecas Python extras ou intervenção humana. Arquivos temporários de teste ficam em diretórios descartáveis.

Para mudar uma regra, atualize a especificação e escreva primeiro um cenário concreto. Não regenere a saída esperada apenas para esconder uma regressão. Consulte [VALIDACAO.md](../docs/VALIDACAO.md) para os limites.

---

**Projeto:** [Tetris em C · Motor, terminal e partidas reproduzíveis](../README.md). Consulte a raiz para os requisitos e o primeiro comando; este guia aprofunda a área `tests/`.

---

**Projeto:** [Tetris em C · Motor, terminal e partidas reproduzíveis](../README.md). Consulte a raiz para os requisitos e o primeiro comando; este guia aprofunda a área `tests/`.
