#
# This is a Makefile to automate generating a snap of obnc
# for use with snapcraft build.
#
HERE=$(pwd)

build: src/*.c src/*.h
	./build

test: .FORCE
	./test

install: .FORCE
	./install


clean:
	if [ -f obnc-snap_0.16.1_amd64.snap ]; then rm obnc-snap_0.16.1_amd64.snap; fi
	./build clean-all

.FORCE:
