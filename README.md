# Hextris Advance

Hextris Advance is a Game Boy Advance port / remake project of the hexagonal falling-block puzzle game Hextris. It is built with [Butano](https://github.com/GValiente/butano), a modern C++ engine for Game Boy Advance development.

The current build focuses on a single-player GBA experience with multiple difficulty modes, score ranking saved to SRAM, bitmap-style presentation, sound effects, and tracker-module BGM.

## Features

- Game Boy Advance ROM output (`Hextris-Advance.gba`)
- Solo play with `EASY`, `NORMAL`, `MASTER`, and `DEATH` modes
- Score ranking and save data support
- Butano-based graphics, input, audio, and SRAM handling
- Public-domain tracker-module BGM candidates from The Mod Archive

## Requirements

- A working GBA development environment compatible with Butano
- `make`
- Python available as `python`
- A GBA emulator or flash cart for running the generated ROM

The `butano/` directory is included in this repository. If you replace it with another Butano version, check the Butano setup notes and update `LIBBUTANO` in `Makefile` if needed.

## Build

From the repository root:

```sh
make
```

The build outputs are generated under `build/` and as root-level ROM / ELF files such as:

- `Hextris-Advance.gba`
- `Hextris-Advance.elf`

To remove generated build files:

```sh
make clean
```

## Controls

Default GBA controls:

- D-pad: move / menu navigation
- A / B / L / R: game actions
- Start: confirm / pause-related action depending on scene

## Repository Notes

This repository includes game source code, game assets, Butano engine files, and third-party components used by Butano. Generated files such as `build/`, `*.gba`, `*.elf`, save files, and emulator configuration are ignored by Git.

Before publishing a release, verify that generated build artifacts are not committed unless you intentionally want to distribute a prebuilt ROM.

The lower-case `credits/` and `licenses/` directories, when present locally, are copied from the Butano distribution and are not the primary license notice for Hextris Advance. Public-facing project notices live in `LICENSE`, `README.md`, `THIRD_PARTY_NOTICES.md`, and `legal/`.

## Licenses And Credits

Hextris Advance project source code is licensed under the MIT License. See [LICENSE](LICENSE).

Third-party engine, library, music, graphics, and sound-effect notices are summarized in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md). Butano's zlib license is mirrored at [legal/BUTANO.txt](legal/BUTANO.txt). The tracker-module BGM list is maintained in [src/Bgm/modarchive.md](src/Bgm/modarchive.md), and the module files live directly under `src/Bgm/`.

Hextris Advance is an unofficial project and is not affiliated with Nintendo.
