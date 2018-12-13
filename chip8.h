#include"Memory.h"
#include"utility.h"
#include"OutputVideo.h"
#include"InputKeyboard.h"
#include <ncurses.h>

class chip8{
private:
  /*

  Chip-8 has 16 general purpose 8-bit registers, usually referred to as Vx, where x is a hexadecimal digit
  (0 through F). There is also a 16-bit register called I. This register is generally used to store memory
  addresses, so only the lowest (rightmost) 12 bits are usually used.
  The VF register should not be used by any program, as it is used as a flag by some instructions. See
  section 3.0, Instructions for details.
  Chip-8 also has two special purpose 8-bit registers, for the delay and sound timers. When these registers
  are non-zero, they are automatically decremented at a rate of 60Hz. See the section 2.5, Timers & Sound,
  for more information on these.
  There are also some "pseudo-registers" which are not accessable from Chip-8 programs. The program counter
  (PC) should be 16-bit, and is used to store the currently executing address. The stack pointer (SP) can
  be 8-bit, it is used to point to the topmost level of the stack.
  The stack is an array of 16 16-bit values, used to store the address that the interpreter shoud return to
  when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.
  */
  Memory memory;

  uint16_t stack[16];
  uint8_t stack_pointer;

  uint8_t V[16];                      //general porpuse

  uint16_t program_counter;                        // Program counter
  uint16_t opcode;                    // Current op code
  uint16_t I;                         // Index register

  uint8_t delay_timer;                // Delay timer
  uint8_t sound_timer;                // Sound timer

  OutputVideo Display;
  InputKeyboard Keyboard;

public:
  chip8(WINDOW* win);
  void inizialize();
  bool loadROM(const char* file);
  void fetch();
  void execute();
};
