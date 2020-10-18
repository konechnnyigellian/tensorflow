;# TENSERflow
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;# Copyright 2020
;#
;#   This program is free software: you can redistribute it and/or modify
;#   it under the terms of the GNU General Public License as published by
;#   the Free Software Foundation, either version 3 of the License, or
;#   (at your option) any later version.
;#
;#   This program is distributed in the hope that it will be useful,
;#   but WITHOUT ANY WARRANTY; without even the implied warranty of
;#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;#   GNU General Public License for more details.
;#
;#   You should have received a copy of the GNU General Public License
;#   along with this program. If not, see <http://www.gnu.org/licenses/>.
;#

_SHA3_256_AVX2_ASM SEGMENT PAGE READ EXECUTE
PUBLIC SHA3_256_AVX2_ASM

ALIGN 64
SHA3_256_AVX2_ASM:

include sha3_256_avx2.inc

KeccakF1600_AVX2_ASM:
	lea r8,[rot_left+96]
	lea r9,[rot_right+96]
	lea r10,[rndc]

include sha3_256_keccakf1600_avx2.inc

_SHA3_256_AVX2_ASM ENDS
END
