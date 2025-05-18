CASE_LIST = tmm_4ld4tdpb_with_dep l1

include config.mk

default: build

build:
	cd src && make TILE_SHAPE=$(TILE_SHAPE) ENABLE_TRACE=$(ENABLE_TRACE)

 # TODO [wxc/20250518]: call sde tracing facilities on each case
trace:
	@for case in $(CASE_LIST); do \
		echo $$case; \
	done

clean:
	cd src && make clean

run:
	./bin/amx_microbenchmark.out $(CASE) $(NITERS) $(M) $(N) $(K) 