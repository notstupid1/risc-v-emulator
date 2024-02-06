gcc -Wall -pedantic -c ../main.c ../cpu/cpu.c ../asm_lib_func.c
gcc -o emu main.o cpu.o asm_lib_func.o