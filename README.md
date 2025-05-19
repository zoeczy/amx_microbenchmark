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

## Note
### Frequency
We assumed that the CPU frequency is 3.126MHz, In Intel(R) Xeon(R) w3-2435, we need to shut down TurboBoost.
``` SHELL
# CHECK THE SUPPORT FOR INTEL_PSTATE
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_driver
# SHUT DOWN TEMPORARY
echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
# LOOK UP THE STATUS
cat /sys/devices/system/cpu/intel_pstate/no_turbo
```
use turbostat to check the frequency
``` SHELL
sudo turbostat
```

