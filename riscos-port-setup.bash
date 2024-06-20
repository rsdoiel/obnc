#!/bin/bash

#
# This file is used to setup for a RISC OS port and is run under Bash typically on a 
# Linux system (do to assumptions about path delimiters).
# It setups the symbolic links into a RISCOS sub-directory used for the port.
#
function symlink_for_riscos() {
	EXT="$1"
	mkdir -p "RISCOS/${EXT:1}"
	
	# Symlink to the .c files
	P="src/*${EXT}"
	for FNAME in $(ls -1 ${P}); do
	    LINK="$(basename "${FNAME}" "${EXT}")"
	    echo "ln ${FNAME} RISCOS/${EXT:1}/${LINK},fff"
	    ln ${FNAME} RISCOS/${EXT:1}/${LINK},fff
	done
}
rm -fR RISCOS
symlink_for_riscos '.c'
symlink_for_riscos '.h'
symlink_for_riscos '.l'
symlink_for_riscos '.y'
symlink_for_riscos '.env'
