#include<cstdint>
#include<algorithm>
#include<iostream>

#define MAX_ROM_SIZE 3584

#define MEM_SIZE 4096

#define _nnn (opcode&0x0FFF)
#define _x (opcode&0x0F00) >> 8
#define _kk (opcode & 0x00FF)
#define _y (opcode & 0x00F0) >> 4
#define _n (opcode&0x000F)
