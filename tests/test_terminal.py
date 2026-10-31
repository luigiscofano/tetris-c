"""Executa a interface em um pseudoterminal POSIX, sem depender de ncurses."""
import os
import select
import signal
import subprocess
import time
import unittest
from pathlib import Path

if os.name != 'nt':
    import pty
    import termios

ROOT = Path(__file__).resolve().parents[1]


@unittest.skipIf(os.name == 'nt', 'Pseudoterminal deste teste e POSIX')
class TerminalTests(unittest.TestCase):
    def exercise(self, stop_signal=False):
        master, slave = pty.openpty()
        original = termios.tcgetattr(slave)
        process = subprocess.Popen([str(ROOT / 'build/tetris'), '--seed', '42'],
                                   stdin=slave, stdout=slave, stderr=slave, cwd=ROOT)
        output = b''

        def await_text(text):
            nonlocal output
            deadline = time.monotonic() + 5
            while text not in output:
                if time.monotonic() >= deadline:
                    self.fail(f'Tela nao encontrada: {text!r}; saida={output[-300:]!r}')
                if select.select([master], [], [], 0.1)[0]:
                    output += os.read(master, 65536)

        try:
            await_text(b'JOGANDO')
            os.write(master, b'p')
            await_text(b'PAUSADO')
            os.write(master, b'r\x1b[Dwx')
            if stop_signal:
                process.send_signal(signal.SIGTERM)
            else:
                os.write(master, b'q')
            # Continue drenando a saida enquanto aguarda o encerramento.
            # O buffer PTY menor no macOS pode bloquear o desenho/limpeza.
            deadline = time.monotonic() + 5
            while process.poll() is None:
                if time.monotonic() >= deadline:
                    self.fail('Aplicacao nao encerrou no prazo')
                if select.select([master], [], [], 0.05)[0]:
                    output += os.read(master, 65536)
            self.assertEqual(process.returncode, 0)
            restored = termios.tcgetattr(slave)
            # BSD/macOS pode marcar PENDIN ao voltar ao modo canonico.
            # E estado transitorio do driver, nao configuracao alterada pelo jogo.
            pending = getattr(termios, 'PENDIN', 0)
            restored[3] &= ~pending
            expected = original.copy()
            expected[3] &= ~pending
            self.assertEqual(restored, expected, 'Restauracao da configuracao do terminal')
            while select.select([master], [], [], 0.05)[0]:
                output += os.read(master, 65536)
            self.assertIn(b'\x1b[?25h', output, 'Cursor restaurado')
            self.assertIn(b'\x1b[?1049l', output, 'Tela alternativa encerrada')
        finally:
            if process.poll() is None:
                process.kill()
                process.wait()
            os.close(master)
            os.close(slave)

    def test_keyboard_pause_restart_and_exit(self):
        self.exercise()

    def test_sigterm_restores_terminal(self):
        self.exercise(stop_signal=True)


if __name__ == '__main__':
    unittest.main(verbosity=2)
