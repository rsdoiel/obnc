#!/bin/bash

#
# This file is used to setup for a RISC OS port and is run under Bash typically on a 
# Linux system (do to assumptions about path delimiters).
# It setups the symbolic links into a RISCOS sub-directory used for the port.
#
function symlink_for_riscos() {
	SDIR="$1"
	EXT="$2"
	
	# Symlink to the .c files
	P="${SDIR}/*${EXT}"
	for FNAME in $(ls -1 ${P}); do
		#FIXME: Need to handle mapping subfolders appropriately
        SRC_TYPE_DIR="RISCOS/${EXT:1}"
        if [ ! -d  "$SRC_TYPE_DIR" ]; then
	    	mkdir -p "${SRC_TYPE_DIR}"
		fi
	    LINK="$(basename "${FNAME}" "${EXT}")"
	    echo "ln ${FNAME} ${SRC_TYPE_DIR}/${LINK},fff"
	    ln ${FNAME} ${SRC_TYPE_DIR}/${LINK},fff
	done
}
rm -fR RISCOS
symlink_for_riscos 'src' '.c'
symlink_for_riscos 'src' '.h'
symlink_for_riscos 'src' '.l'
symlink_for_riscos 'src' '.y'
symlink_for_riscos 'src' '.env'
