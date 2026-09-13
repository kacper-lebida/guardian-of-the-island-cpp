# Guardian of the Island

A console RPG built as a team project for an introductory C/C++ programming course. Explore an island, collect items, survive turn-based encounters and face its guardian.

## My contribution

The original project README credits **Kacper Lebida** with the inventory system, save/load system, and game loop/state work (`Game.h`). The supplied ZIP contains source code but no Git history, so this attribution follows the team's documentation.

- **Inventory:** polymorphic items owned by `std::unique_ptr` in a `std::vector`, numbered item selection, bounds checking and removal after use.
- **Persistence:** text serialization of player statistics, exploration progress and item type/name/value/power; reconstruction of concrete item classes on load.
- **Game state:** an exploration/combat/exit state model connecting the menu and gameplay systems.

## Gameplay

- 20 possible locations and three exploration outcomes: nothing, loot or combat.
- Six enemy types, with one to five enemies per combat location.
- Weapons increase attack; healing and armor items add HP. All items are consumed when used.
- Randomized attacks and probabilistic escape/item-use actions during combat.
- A final boss intended to appear after ten completed locations.
- Manual saving to `save.txt` and automatic loading at startup.

## Structure

| Module | Responsibility |
| --- | --- |
| `Game` | Main loop, exploration, encounters, loot and boss progression |
| `Character` | Character statistics and inventory ownership |
| `Inventory` | Item storage, use and serialization |
| `Item` | Abstract interface and weapon/healing/armor implementations |
| `SaveSystem` | Player and progress persistence |
| `MenuSystem` | Exploration menu and command dispatch |
| `CombatSystem` | An unused placeholder; actual combat is implemented in `Game.cpp` |

## Build and run

Use a C++17 compiler. From the repository root with CMake:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Run `./build/guardian` on Linux or `./build/Debug/guardian.exe` with a Visual Studio generator on Windows. Single-configuration Windows generators place the executable at `./build/guardian.exe`.

Alternatively, compile directly with GCC or Clang:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic src/*.cpp -o guardian
./guardian
```

On Windows with MinGW, use `-o guardian.exe`. The save file is written to the working directory. Python 3 is required for the optional CLI smoke tests.

Controls: **E** explore, **I** inventory, **U** use item, **S** save, **Q** quit.

## Review status

The portfolio version fixes boss progression, invalid damage ranges, malformed save handling, input recovery and restart state. Core regression tests and CLI smoke tests pass locally on Windows; direct C++17 builds with Zig 0.15.2 produce no compiler diagnostics. CMake configuration, build and CTest passed on both Windows and Ubuntu in [GitHub Actions run #1](https://github.com/kacper-lebida/guardian-of-the-island-cpp/actions/runs/34759763455).

See [portfolio preparation and validation](docs/portfolio-preparation.md) for changes, test coverage and remaining limitations. The original coursework and the later Codex-assisted improvements are documented separately.

## Team credits

Credits reproduced from the supplied project README:

| Contributor | Documented area |
| --- | --- |
| Kacper Lebida | Inventory, SaveSystem, game loop/state (`Game.h`) |
| Wiktor Kaczmarczyk | Turn-based combat (`Game.cpp`) |
| Szymon Baran | Item hierarchy and MenuSystem |
| Tomasz Kraska | Character, victory condition and loot |

No license was included in the supplied archive. This repository does not assign a new license to the team's code.
