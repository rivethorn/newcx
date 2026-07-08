<div align="center" >
  <h1>newcx</h1>
  <h4>pronounced: nukes (/njuːks/)</h4>
</div>
A command-line tool that scaffolds new C or CPP projects with a
`src/`, `include/`, Makefile, and CMake-based build setup. No manual directory
juggling or boilerplate typing.

## Build from source

First clone the repo:

```bash
git clone https://github.com/rivethorn/newc.git && cd newcx
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
  adding new `.c` files to `src/` later doesn't require editing the
  build config.
- Re-running `newc` or `newcpp` on a name that already exists will fail safely
  without overwriting anything.

## License

MIT — see `LICENSE`.
