# Portfolio preparation

The starting point was the supplied academic project archive, `RPG.zip`. The team attribution in the original README is retained. No Git history was included, so original branch names, reviews and individual commits cannot be verified from this archive.

The following changes were made during portfolio preparation on 13 September 2026. They are separate from the original coursework contribution:

- Validate a complete save before replacing the player's state; reject malformed items, invalid counts, unknown item types and invalid progress.
- Report save failures and support names containing spaces.
- Keep both damage bounds at or below 50, including attack values 92 and 93 that previously produced a zero divisor.
- Preserve the boss state when the final regular fight ends and restore boss encounters from a save at the goal.
- Recover from nonnumeric input and exit cleanly on end-of-file.
- Reset progress and starter inventory consistently.
- Bound HP and attack accumulation to prevent signed integer overflow.
- Keep terminal output visible and remove the forced 20-second end screen delay.
- Add CMake configuration, core regression tests, CLI tests and a GitHub Actions workflow.

## Validation

On Windows, the application and regression executable were compiled with Zig 0.15.2's C++ frontend, using C++17 and `-Wall -Wextra -Wpedantic`. Both compiled without diagnostics.

The core suite checks typed-item save/load round trips, invalid indices, eight malformed save cases, preservation of existing state, missing/unwritable saves, bounded statistics, repeated restart and a deterministic normal-fight-to-boss transition.

The CLI suite checks startup/EOF, recovery from invalid numeric input, rejection of corrupt saves, boss encounters at attack 15/92/93/1,000,000, damage bounds, exploration-to-finale progression and invalid combat choices.

On 13 September 2026, [GitHub Actions run #1](https://github.com/kacper-lebida/guardian-of-the-island-cpp/actions/runs/34759763455) passed CMake configuration, compilation and CTest on both windows-latest and ubuntu-latest for commit e280cae3b70f042b6152073f5a59ee954e9974a8. Both core regression and CLI smoke tests are included in CTest. The initial local checks used direct compilation.

## Remaining limitations

This remains a small educational game. It uses `rand()`, has no difficulty selector, and treats weapons and armor as consumable stat bonuses. Saves are plain text, are not tamper-proof and are not written atomically. A failed write is reported but may leave a partial file. Character copy operations copy statistics, not inventory; persistence transfers inventory explicitly. The supplied `CombatSystem` is an unused placeholder.

The archive's prebuilt `start.exe` and missing `docs/damage_graph.png` reference are not included in this source package. No new license has been assigned to the team's code.
