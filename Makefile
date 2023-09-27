#
# This is a Makefile to automate generating a snap of obnc
# for use with snapcraft build.
#
HERE=$(pwd)

PREFIX_OPT =
prefix =
ifneq ($(prefix), )
  PREFIX_OPT = --prefix=$(prefix)
endif

build: src/*.c src/*.h
	./build $(PREFIX_OPT)

test: .FORCE
	./test

install: .FORCE
	./install


clean:
	if [ -f obnc-snap_0.16.1_amd64.snap ]; then rm obnc-snap_0.16.1_amd64.snap; fi
	if [ -f obnc-snap_0.17.0_amd64.snap ]; then rm obnc-snap_0.17.0_amd64.snap; fi
	./build clean-all

.FORCE:
