# amx_microbenchmark

## Build

```
$ make build
```

If the build step succeeds, the binary `amx_microbenchmark.out` will be generated in `bin`, and the objects will be generated in `build`.

There is a config file named `config.mk`, and user can modify or add build time config variables in the `bulid configs` section.

## Run

```
$ make run
```

It will execute `amx_microbenchmark.out` according to the runtime configs defined in the `runtime configs` section also in `config.mk`. User can also run `./bin/amx_microbenchmark.out` directly given runtime arguments as follows:

```
# number of iterations must no less than 10^9 to fully utilize AMX, useless args MNK in tmm level benchmark
./bin/amx_microbenchmark.out l1 10000 64 64 64
```
