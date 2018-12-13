#include"Memory.h"

Memory::Memory(){
  std::fill(memory+0, memory+4096, 0);
}

Memory::~Memory(){}

uint8_t& Memory::operator[](uint16_t addr){
  if(addr<0 || addr>=4096)
    std::cerr << "Memory::opertor[] - out of bound index" << '\n';
  else return memory[addr];
}
