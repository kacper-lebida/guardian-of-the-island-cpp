"""Black-box checks. Each run uses an isolated temporary save directory."""
import pathlib
import subprocess
import sys
import tempfile

exe = str(pathlib.Path(sys.argv[1]).resolve())

def run(commands, save=None):
    with tempfile.TemporaryDirectory(prefix="guardian-test-") as d:
        if save is not None:
            pathlib.Path(d, "save.txt").write_text(save)
        result = subprocess.run([exe], input=commands, text=True, capture_output=True, cwd=d, timeout=5)
        assert result.returncode == 0, result.stderr
        return result.stdout

assert "=== MENU ===" in run("")
assert "Enter a whole number" in run("u\nabc\n1x\n1\nq\n")
assert "Attack +2" in run("u\nabc\n1\nq\n")
assert "Save loaded" not in run("q\n", "invalid")
for attack in (15, 92, 93, 1000000):
    output = run("1\n" * 100, f"Hero\n100000\n{attack}\n10\n10\n0\n")
    assert "Guardian of the Island" in output and "YOU WON" in output
    import re
    damage = [int(x) for x in re.findall(r"for (\d+) damage", output)]
    assert damage and all(5 <= x <= 50 for x in damage), damage
# Reaching the goal via a normal fight must retain the boss state.
# With goal 1, every exploration outcome reaches the boss after enough attacks.
output = run("e\n" + "1\n" * 150, "Hero\n100000\n1000000\n0\n1\n0\n")
assert "YOU WON" in output
assert "Choose 1, 2 or 3" in run("9\n", "Hero\n100\n15\n10\n10\n0\n")
print("CLI smoke tests passed.")
