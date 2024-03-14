# Notes on converting to Meson

Count lines of C code

    fd -e c -x wc -l '{}' | awk '{printf "%6.d %s\n", $1, $2}' | sort

