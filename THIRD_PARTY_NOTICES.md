# Third-Party Notices

This file summarizes third-party code and assets included in or referenced by Hextris Advance. Individual upstream license files should be kept with their corresponding source files when redistributing the repository or a source release.

The lower-case `credits/` and `licenses/` directories, when present, are Butano distribution collateral. They are kept separate from this project's public-facing notices to avoid presenting Butano sample credits as Hextris Advance's own license.

## Project Code

- Hextris Advance source code: MIT License, see `LICENSE`.

## Engine And Libraries

- Butano: zlib License. Upstream: https://github.com/GValiente/butano. Local mirror: `legal/BUTANO.txt`.
- Butano third-party components: see `butano/hw/3rd_party/` and the license files already present in those subdirectories.
- Additional Butano license summaries may be present in the local lower-case `licenses/` directory when the full Butano distribution is included.

Known bundled third-party components include, but are not limited to:

- AAS
- agbabi
- cult-of-gba-bios
- gba-link-connection
- gba-modern
- gbt-player
- libtonc
- libugba
- maxmod
- posprintf

## Music

The active BGM build input is `src/Bgm/`.

The module files listed in `src/Bgm/modarchive.md` were selected from The Mod Archive as public-domain tracker modules and are stored directly under `src/Bgm/`. Keep `src/Bgm/modarchive.md` with releases so each module can be traced to its title, Mod Archive ID, format, intended use, and source URL.

Older / sample music credit notes may exist in `credits/music.txt` if the full Butano sample asset directories are present. Do not assume those older notes apply to the active BGM set unless the corresponding files are included in a release.

## Graphics

Game graphics are stored under `src/Image/` and `src/Res/`.

Some directories copied from Butano or its examples include their own credit files, such as `credits/graphics.txt`, when the full Butano sample distribution is present. Preserve those credit files if redistributing the corresponding example assets.

## Sound Effects

Sound effects are stored under `src/Se/`.

If the local `credits/sound_effects.txt` file is included, it identifies Superpowers asset-pack sound effects by Sparklin Labs / Pixel-boy as Creative Commons Zero (CC0). Preserve that notice when redistributing those sound effects.

## Release Checklist

Before publishing a binary or source archive:

- Keep `LICENSE`, `README.md`, `THIRD_PARTY_NOTICES.md`, `legal/`, and `src/Bgm/modarchive.md` in the release.
- Keep third-party license files inside `butano/hw/3rd_party/`.
- Verify that every distributed asset is covered by the notices above or by a license file distributed next to it.
- Avoid publishing generated build artifacts unless the release intentionally includes a prebuilt ROM.
