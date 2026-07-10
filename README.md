<div align="center" >
  <h1>newcx</h1>
  <h4>pronounced: nukes (/njuːks/)</h4>
</div>

A command-line tool that scaffolds new C or CPP projects with a
`src/`, `include/`, Makefile, and CMake-based build setup. No manual directory
juggling or boilerplate typing.

## Install

You can use the installer script to install `newcx` on your system.

Linux and macOS:

```bash
curl -fsSL https://raw.githubusercontent.com/rivethorn/newcx/main/install.sh | bash
```

Windows PowerShell:

```powershell
irm https://raw.githubusercontent.com/rivethorn/newcx/main/install.ps1 | iex
```

This will download and install `newcx` and create `newc` and `newcpp` launchers, which can be used to generate C and CPP projects, respectively.

## Usage

Just provide the name of your project:

```bash
newc my-cool-app    # new C project
newcpp my-cool-app  # new CPP project
```

Then simply:

```bash
cd my-cool-app
cmake -S . -B out
cmake --build out
./out/my-cool-app
```

Or if you'd prefer Make:

```bash
cd my-cool-app
make
./out/my-cool-app
```

If for some reason you don't have symlinks, you can use `newcx link` to create them.

## Configuration

The app uses a simple TOML file at `~/.newcx.toml`, which will be created on first launch.

You can specify the C, and CPP standard version you'd like to use:

```toml
c-standard = "11"
cpp-standard = "11"
```

> [!NOTE]
Both are set to 11 by default.

## Build from source

First clone the repo:

```bash
git clone https://github.com/rivethorn/newcx.git && cd newcx
```

Then:

```bash
cmake -B out && cmake --build out
sudo cmake --install out
```

## Notes

- Project names with hyphens or other non-alphanumeric characters are
  automatically sanitized into valid C header guards
  (e.g. `password-vault` → `PASSWORD_VAULT_H`).
- `CMakeLists.txt` uses `file(GLOB ...)` to pick up source files, so
  adding new `.c`/`.cpp` files to `src/` later doesn't require editing the
  build config.
- Re-running `newc`/`newcpp` on a name that already exists will fail safely
  without overwriting anything.

## License

MIT — see `LICENSE`.
