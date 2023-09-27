#!/bin/sh

# Copyright 2017, 2018, 2019, 2023 Karl Landstrom <karl@miasap.se>
#
# This file is part of OBNC.
#
# obnc-libext is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# obnc-libext is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with obnc-libext.  If not, see <http://www.gnu.org/licenses/>.

set -e

expectedOutput="a
abc
-32768;-2147483648;-9223372036854775808
-1
 -1
0
1
37
32767;2147483647;9223372036854775807
 0000; 00000000; 0000000000000000
 0001; 00000001; 0000000000000001
 FFFF; FFFFFFFF; FFFFFFFFFFFFFFFF
-1.000000E+00;-1.000000E+000
0.000000E+00;0.000000E+000
  0.000000E+00;  0.000000E+000
1.000000E+00;1.000000E+000
3.700000E+01;3.700000E+001
3.700000E-01;3.700000E-001"

IFS='
'
i=1
for line in $(./ErrTest 2>&1); do
	IFS=";" read -r expectedRes1 expectedRes2 expectedRes3<<EOF
$(echo "$expectedOutput" | awk -v i=$i 'NR == i')
EOF
	if [ "$line" != "$expectedRes1" ] && [ "$line" != "$expectedRes2" ] && [ "$line" != "$expectedRes3" ]; then
		echo "test failed: output: \"$line\", expected output: \"$expectedRes1\" or \"$expectedRes2\" or \"$expectedRes3\"" >&2
		exit 1
	fi
	i="$((i + 1))"
done
