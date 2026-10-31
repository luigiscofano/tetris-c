"""Testa a CLI real: saídas, erros e reprodução de partidas."""
from pathlib import Path
import os
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
BIN = ROOT / 'build' / ('tetris.exe' if os.name == 'nt' else 'tetris')


def run(*args):
    return subprocess.run([str(BIN), *args], cwd=ROOT, capture_output=True, text=True, timeout=10)


class CommandLineTests(unittest.TestCase):
    def test_help_and_version(self):
        self.assertIn('Uso:', run('--help').stdout)
        self.assertEqual(run('--version').stdout.strip(), 'tetris-c 1.0.0')

    def test_seed_validation(self):
        for seed in ['-1', '4294967296', '', 'abc', '42x', ' 1', '+1']:
            self.assertEqual(run('--seed', seed).returncode, 2, seed)
        for seed in ['0', '4294967295']:
            self.assertEqual(run('--seed', seed, '--replay', 'examples/partida.txt').returncode, 0)

    def test_missing_arguments_and_files(self):
        for option in ['--seed', '--replay', '--unknown']:
            self.assertEqual(run(option).returncode, 2)
        self.assertEqual(run('--replay', 'arquivo-inexistente').returncode, 1)

    def test_replay_matches_committed_fixture(self):
        first = run('--seed', '42', '--replay', 'examples/partida.txt')
        self.assertEqual(first.returncode, 0, first.stderr)
        self.assertEqual(first.stdout, (ROOT / 'examples/partida-esperada.txt').read_text())
        self.assertEqual(first.stdout, run('--seed', '42', '--replay', 'examples/partida.txt').stdout)
        self.assertNotIn('\x1b', first.stdout)

    def test_replay_rejects_invalid_command(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / 'invalido.txt'
            path.write_text('a?x')
            result = run('--replay', str(path))
            self.assertEqual(result.returncode, 1)
            self.assertIn('invalido', result.stderr)

    def test_noninteractive_requires_replay(self):
        self.assertEqual(run().returncode, 1)
        self.assertIn('--replay', run().stderr)


if __name__ == '__main__':
    unittest.main(verbosity=2)
