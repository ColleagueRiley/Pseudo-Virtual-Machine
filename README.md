# Psudeo VM
This is a very simple VM for my imaginary 8-bit machine. 

This project is heavily inspired by [Chip-8](https://en.wikipedia.org/wiki/CHIP-8)

# building 

## Linux
simply run `make buildLinux`

or\
`gcc source/main.c -I include -lX11 -o RVM.exe`

## Windows
simply run `make buildWindows`

or\
`gcc source/main.c -I include -lgdi -lshell32 -o RVM.exe`


then to run the VM simply run `RVM` or `RVM.exe` as per OS


# Machine Code
## key

V -> register [VX = register[x]]\
X -> register number (0 - F)\
N -> number, 0 - 99\
        numbers must be represented using two digits\
        ex. 9 -> 09\
M -> memory address from 0 - 4096\
        number bust be represented using 4 digets\
        ex. 9 -> 0009

I'd reccomend that you do not try to access memory lower than
1500 as to not overwrite the script which is stored in memory

## opcodes

**0000** : `}` close program or end while loop ( supported )\
**1FXN** : `if N == VX `[do next line] ( supported / untested)\
**1AXN** : `while N == VX` ( supported )\
**A000** : `rand() % 100` generates a random number [0 - 100] into VE ( supported )\
**C000** : `clearScrn()` clear screen ( supported )\
**DFXM** : `registers[X] = memory[M]` dumps value at RAM address M into VX ( supported )\
**DSXY** : `registers[S] = X; registers[S + 1] = Y; ` sets V(S) to X and V(S + 1) to Y ( supported )\
**DESX** : `drawRect(registers[S], registers[S + 1], registers[S + 2], registers[S + 3])` draws rectangle at (V(X), V(X + 1)) with w width of V(X + 3) and a height of V(X + 4)\
**EENM** : `memory[M] = N` sets the value at RAM address M to N ( supported )\
**EANM** : `memory[M] += N `adds N to the value at RAM address M\
**EFXM** : `memory[M] += registers[X]` adds the value at register X to the value at RAM address M\