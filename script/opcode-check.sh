#!/bin/sh

# Get the full path to this script while handling spaces and symlinks correctly
scriptPath="$(cd -P -- "$(dirname -- "$0")" && pwd -P)"
cd "$scriptPath/.." || exit 1

opcode_check() {
	path="$1"
	file="$(basename "$path")"

	max=255
	for i in $(seq 1 "$max")
	do
		opcode=$(printf "0x%.2x\n" "$i")
		# Pattern: case 0x??:
		if ! grep -q "case $opcode:" "$path"; then
			echo "missing opcode: $opcode in $file"
		fi
	done
}

opcode_check "src/cpu.cpp"
opcode_check "src/cpu-prefix.cpp"
