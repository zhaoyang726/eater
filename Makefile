export
CC := g++
CPPFLAGS := -g
CPPFLAGS += -I$(CURDIR)/include/

OUTDIR := $(CURDIR)/out
objs := $(patsubst %.cpp, %.o, $(wildcard $(CURDIR)/src/*/*.cpp))

.PHONY: default
default: src clean

.PHONY: src
src:
	make -C src/
	make install -C src/

.PHONY: test
test: src
	make clean
	make -C test/
	rm -rf $(OUTDIR)
	mkdir -p $(OUTDIR)
	make install -C test/
	make clean

.PHONY: clean
clean:
	-find -name "*.o" | xargs rm -f
