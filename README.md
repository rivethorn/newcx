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
git clone https://github.com/rivethorn/newcx.git && cd newcx
```

Then:

```bash
cmake -B out && cmake --build out
sudo cmake --install out
```

## License

MIT — see `LICENSE`.
