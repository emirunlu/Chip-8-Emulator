# Chip-8-Emulator

A CHIP-8 emulator in C++

CHIP-8 is an interpretted programming language developed by Joseph Weisbecker in the mid 70s and was initally used on the COSMAC VIP and Telmac 1800 8-bit microcomputers to make game programming easier. CHIP-8 programs are run using a CHIP-8 virtual machine.

## Compiling and Running
Nothing special is needed other than SDL2.

You can run it with:
```
./chip8 <ROM file>
```

## TODO
*	Fix SDL implementation
*	Add audio
*	Add function keys
*	Add Super Chip-8 opcodes
* Use function pointers instead of switch case
* Improve graphics using filters

## References
Some helpful resources I used

- http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
- http://en.wikipedia.org/wiki/CHIP-8
- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
