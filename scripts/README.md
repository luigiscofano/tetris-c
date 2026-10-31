# Ferramentas

Após `make`, execute `python3 scripts/gerar_preview.py` na raiz. O script executa o replay de exemplo e atualiza `assets/preview.svg`. Usa apenas a biblioteca padrão de Python.

O CI regenera o SVG e compara com o arquivo versionado. Uma mudança no motor que afete a demonstração deve ser examinada antes de atualizar tanto a referência de saída quanto o preview.
