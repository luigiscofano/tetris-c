"""Gera uma ilustração SVG a partir da saída real do replay de referência."""
from pathlib import Path
import html
import subprocess

ROOT = Path(__file__).resolve().parents[1]
output = subprocess.check_output([str(ROOT/'build/tetris'), '--seed', '42', '--replay',
                                  str(ROOT/'examples/partida.txt')], text=True)
rows = [line[1:-1] for line in output.splitlines() if line.startswith('|')]
assert len(rows) == 20
palette = dict(zip('IOTSZJL', ['#63dbe5', '#ffd66e', '#bc98ff', '#86dfab', '#ff7d94', '#8baaff', '#ffa66e']))
parts = ['<svg xmlns="http://www.w3.org/2000/svg" width="1100" height="720" viewBox="0 0 1100 720">',
         '<rect width="1100" height="720" rx="24" fill="#101827"/>',
         '<g font-family="monospace" fill="#e8eef8">',
         '<text x="64" y="95" fill="#86dfab" font-size="17">C11 · TERMINAL · MOTOR DETERMINISTICO</text>',
         '<text x="60" y="165" font-size="66" font-weight="bold">TETRIS EM C</text>',
         '<text x="64" y="212" fill="#aebbd0" font-size="19">Matrizes. Estados. Algoritmos. Testes.</text>',
         '<text x="64" y="300" font-size="25">Aprender construindo.</text>',
         '<text x="64" y="342" fill="#aebbd0" font-size="17">Um motor pequeno, separado do terminal.</text>',
         '<text x="64" y="380" fill="#aebbd0" font-size="17">Sete pecas e partidas reproduziveis.</text>',
         '<text x="64" y="418" fill="#aebbd0" font-size="17">Testes de regras, CLI e pseudoterminal.</text>',
         '<rect x="64" y="480" width="490" height="76" rx="12" fill="#1d2c42"/>',
         '<text x="84" y="512" fill="#86dfab" font-size="16">make test</text>',
         '<text x="84" y="540" fill="#aebbd0" font-size="14">./build/tetris --seed 42</text>',
         '<text x="64" y="632" fill="#aebbd0" font-size="16">Luigi Scofano de Araujo · Poli-USP</text>',
         '<rect x="668" y="55" width="304" height="574" rx="12" fill="#1d2c42"/>']
for r, row in enumerate(rows):
    for c in range(10):
        char = row[c*2]
        parts.append(f'<rect x="{680+c*28}" y="{67+r*28}" width="25" height="25" rx="4" fill="{palette.get(char, "#142034")}"/>')
summary = output.splitlines()[-1]
parts += [f'<text x="668" y="664" fill="#aebbd0" font-size="12">{html.escape(summary)}</text>',
          '<text x="668" y="688" fill="#aebbd0" font-size="12">Visualizacao do replay real; interface nativa em terminal.</text>',
          '</g></svg>']
(ROOT/'assets/preview.svg').write_text('\n'.join(parts)+'\n')
print('assets/preview.svg')
