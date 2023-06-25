# Pseudo VM
![vm workflow linux](https://github.com/ColleagueRiley/psudeo-Virtual-Machine/actions/workflows/linux.yml/badge.svg)
![vm workflow windows](https://github.com/ColleagueRiley/psudeo-Virtual-Machine/actions/workflows/windows.yml/badge.svg)

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
number bust be represented using unicode value\
ex. 65 -> A\
ex. 

*memory lower than 1500 is locked for the RAM,
this means 1500 is added to whatever RAM address you're trying to access

## opcodes

**0000** : `}` close program or end while loop\
**1FXN** : `if N == VX `[do next line] ( supported / untested)\
**1AXN** : `while N == VX`\
**1BXN** : `if N >= VX` [do next line]\
**1CXN** : `while N >= VX` \
**1DXN** : `if N <= VX` [do next line]\
**1EXN** : `while N <= VX`\
**A1XN** : `registers[X] = rand() % N` generates a random number from 0 - N into VX\
**C000** : `clearScrn()` clear screen\
**DFXM** : `registers[X] = memory[M]` dumps value at RAM address M into VX\
**DSXY** : `registers[S] = X; registers[S + 1] = Y; ` sets V(S) to X and V(S + 1) to Y\
**DESX** : `drawRect(registers[S], registers[S + 1], registers[S + 2], registers[S + 3])` draws rectangle at (V(X), V(X + 1)) with w width of V(X + 3) and a height of V(X + 4)\
**EENM** : `memory[M] = N` sets the value at RAM address M to N\
**EANM** : `memory[M] += N `adds N to the value at RAM address M\
**EFXM** : `memory[M] += registers[X]` adds the value at register X to the value at RAM address M\