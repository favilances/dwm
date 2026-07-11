# Repository Guidelines

## Project Structure & Module Organization

This repository is a customized `dwm` tree. Core window-manager code lives at the top level: `dwm.c`, `drw.c`, `util.c`, and their headers. Edit defaults in `config.def.h`; `make` copies it to `config.h` when needed. Build settings are in `config.mk`, install/package rules are in `Makefile`, and the manual page is `dwm.1`.

Runtime helpers live in `scripts/`, with shared shell logic in `scripts/lib/noir-common.sh`. Desktop/session assets are under `packaging/`, and UI configuration for rofi, picom, dunst, and GTK lives in `config/`. `install-cachyos.sh` provisions packages and system configuration for CachyOS.

## Build, Test, and Development Commands

- `make` builds the `dwm` binary from local C sources.
- `make clean all` removes generated objects and rebuilds from scratch.
- `sudo make install` installs `dwm` and the man page under the paths from `config.mk`.
- `./install-cachyos.sh` installs the full CachyOS desktop setup; review it before running because it can change packages, LightDM, and system services.
- `git diff --check` catches whitespace errors before commit.
- `bash -n scripts/noir-* scripts/lib/noir-common.sh install-cachyos.sh` checks shell syntax.

## Coding Style & Naming Conventions

Follow upstream `dwm` style for C: C99, tabs for indentation, compact functions, K&R-style braces, and small static helpers close to their use. Keep configuration data in `config.def.h` unless behavior belongs in `dwm.c`.

For shell scripts, use Bash, quote variable expansions, prefer `snake_case` names, and place shared behavior in `scripts/lib/noir-common.sh`. Keep source text ASCII unless user-facing labels or status glyphs require UTF-8.

## Testing Guidelines

There is no automated test framework in this repo. Minimum validation is a clean C build plus shell syntax checks. For window-manager changes, also test manually in an X11 session before merging. Name any future tests after the behavior they cover, for example `status-clicks` or `network-menu`.

## Commit & Pull Request Guidelines

Use small, focused commits with terse imperative subjects, preferably scoped like `bar: fix status click target` or `scripts: harden network menu`. Pull requests should describe visible behavior changes, list validation commands run, link related issues when applicable, and include screenshots for bar, menu, theme, or session changes. Call out package, LightDM, or service changes explicitly.

## Security & Configuration Notes

Do not commit Wi-Fi passwords, tokens, or machine-specific secrets. Scripts that need privileges should make `sudo` boundaries clear and avoid hardcoded user home paths.
