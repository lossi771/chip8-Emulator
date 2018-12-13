#include"chip8.h"
#include<stdio.h>
#include<time.h>
#include<chrono>

uint8_t fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    /*font 0*/
    0x20, 0x60, 0x20, 0x20, 0x70,
    /*font 1*/
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    /*font 2*/
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    /*font 3*/
    0x90, 0x90, 0xF0, 0x10, 0x10,
    /*font 4*/
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    /*font 5*/
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    /*font 6*/
    0xF0, 0x10, 0x20, 0x40, 0x40,
    /*font 7*/
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    /*font 8*/
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    /*font 9*/
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    /*font A*/
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    /*font B*/
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    /*font C*/
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    /*font D*/
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    /*font E*/
    0xF0, 0x80, 0xF0, 0x80, 0x80
    /*font F*/
};

chip8::chip8(WINDOW* win):memory(),Display(win),Keyboard(win){
  std::fill(stack, stack+16, 0);
  std::fill(V,V+16,0);
  stack_pointer=0;
}

void chip8::inizialize(){

/* Programs may also refer to a group of sprites representing the hexadecimal digits 0
through F. These sprites are 5 bytes long, or 8x5 pixels.
The data should be stored in the interpreter area of Chip-8 memory (0x000 to 0x1FF)*/

  for (int addr = 0; addr < 80; addr++) {
      memory[addr] = fontset[addr];
  }
  program_counter=0x200;
  srand(time(NULL));
}

bool chip8::loadROM(const char* file){

  FILE* ROM=fopen(file,"rb"); //read and binary
  if(ROM==NULL){
    std::cerr<<"Error in loadROM - can't find ROM path..."<<std::endl;
    return false;
  }
  fseek(ROM,0,SEEK_END); //posizione l'indicatore del file a 0
  unsigned int ROM_size=ftell(ROM); //per gli stream binary ritorna la dimensione dall'inizio dell'indicatore
  rewind(ROM);
  /*http://www.cplusplus.com/reference/cstdio/ftell/*/
  /*adesso devo verificare se ho abbastanza spazio*/
  if(ROM_size>MAX_ROM_SIZE){
    std::cerr<<"Error in loadROM - ROM size greater than available memory"<<std::endl;
    return false;
  }
  size_t parsedByte=fread(&memory[512],1,(size_t)ROM_size,ROM);
  if(parsedByte=!(size_t)ROM_size){
    std::cerr<<"Error in loadROM - can't copy ROM in memory..."<<std::endl;
    return false;
  }
  fclose(ROM);
  return true;
}

void chip8::fetch(){
  opcode=(memory[program_counter] << 8) | memory[program_counter+1];
}

/*
Abbiamo utilizzato questa notazione per il prelievo dei valori dall' opcode (DOCUMENTAZIONE):

_nnn - A 12-bit value, the lowest 12 bits of the instruction
_n  - A 4-bit value, the lowest 4 bits of the instruction
_x - A 4-bit value, the lower 4 bits of the high byte of the instruction
_y - A 4-bit value, the upper 4 bits of the low byte of the instruction
_kk - An 8-bit value, the lowest 8 bits of the instruction

*/

void chip8::execute(){

  Keyboard.getKeys();

  switch(opcode & 0xF000){

    /*istruzione 0x0... */
    case (0x0000):
      switch _n {
        case 0x0000:
          Display.clear();
          break;

        case 0x000E:
          program_counter = stack[--stack_pointer];
          break;

        default:
          Display.error();
          std::cout<<"unplemented operation: "<<std::hex<<unsigned(opcode)<<std::endl;
          exit(0);
      }
      break;

      /*istruzione 0x1... */
    case(0x1000):
      program_counter = _nnn - 2;
      break;

      /*istruzione 0x2... */
    case(0x2000):
      stack[stack_pointer++]=program_counter;
      program_counter = _nnn - 2;
      break;

        /*istruzione 0x3...*/
    case (0x3000):
      if(V[_x]==_kk)
        program_counter += 2;
      break;
      /*istruzione 0x4 - contrario 0x3...*/
    case(0x4000):
      if(V[_x]!=_kk)
        program_counter += 2;
      break;

      /*istruzione 0x5...*/
    case(0x5000):
      if (V[_x] == V[_y])
        program_counter += 2;
      break;

      /*istruzione 0x6...*/
      case(0x6000):
        V[_x]=_kk;
        break;

      case (0x7000):
        V[_x] += _kk;
        break;

      case(0x8000):

        switch _n {
          case 0x0000:
            V[_x] = V[_y];
            break;

          case 0x0001:
            V[_x] = V[_x] | V[_y];
            break;

          case 0x0002:
            V[_x] = V[_x] & V[_y];
            break;

          case 0x0003:
            V[_x] = V[_x] ^ V[_y];
            break;

          case 0x0004:
            if((255 - V[_y]) < V[_x])
              V[15]=1;
            else
              V[15]=0;
            V[_x] = V[_x] + V[_y];
            break;

          case 0x0005:
            if(V[_y] > V[_x])
              V[15] = 0; // nella documentazione dice di settarlo quando non borrow
            else
              V[15] = 1;
            V[_x] = V[_x] - V[_y];
            break;

          case 0x0006:
            if((V[_x] & 0x01)==1)
              V[15]=1;
            else
              V[15]=0;
            V[_x] = V[_x] >> 1;
            break;

          case 0x0007: //uguale a 5 solo invertito
            if(V[_x] > V[_y])
              V[15] = 0; // nella documentazione dice di settarlo quando non borrow
            else
              V[15] = 1;
            V[_x] = V[_y] - V[_x];
            break;

          case 0x000E: //uguale a 6 solo shift a sinistra
            if((V[_x] & 0x80)==0x80)
              V[15]=1;
            else
              V[15]=0;
            V[_x] = V[_x] << 1;
            break;

          default:
            Display.error();
            std::cout<<"unplemented operation: "<<std::hex<<unsigned(opcode)<<std::endl;
            exit(0);
            break;

        }

        break;



    case 0x9000:
      if (V[_x] != V[_y])
          program_counter += 2;
      break;

    /*istruzione 0xA...*/
    case(0xA000):
      I=_nnn;
      break;

    case 0xB000:
      program_counter = _nnn + V[0] - 2;
      break;

    /*istruzione 0xC...*/
    case(0xC000):
      V[_x] = ((uint8_t)rand()%255+1) & _kk;
      break;

    case(0xD000):
      V[15]=0;
      for(uint8_t index=0;index<_n;index++){
        V[15]+=Display.drawPixels(memory[I+index],V[_x],V[_y]+index);
      }
      if(V[15]>0) V[15]=1;
      break;


      /*istruzioni 0xF...*/
    case(0xE000):
      switch _kk {

          case (0x009E):
            if (Keyboard.checkCharacter(V[_x]))
                program_counter += 2;
            break;

          case (0x00A1):
            if (!Keyboard.checkCharacter(V[_x]))
                program_counter += 2;
            break;

          default:
            Display.error();
            std::cout<<"unplemented operation: "<<std::hex<<unsigned(opcode)<<std::endl;
            exit(0);
            break;
      }
      break;

    case(0xF000):

        switch _kk{

          case(0x0007):
            V[_x] = delay_timer;
            break;

          case (0x000A):
            V[_x]=Keyboard.getBlockingCharacter();
            break;

          case (0x0015):
            delay_timer = V[_x];
            break;

          case (0x0018):
            /* registro sound incrementato*/
            break;

          case (0x001E):
          if(I + V[_x] > 0xFFF)
            V[0xF] = 1;
          else
            V[0xF] = 0;
          I+=V[_x];
          break;

          /*I punta allo sprite del valore presente in Vx,
          abbiamo inserito gli sprite predefiniti in indirizzi
          contigui da 0x000 a 0x1FF. Basta prendere il valore e
          moltiplicarlo per 5 in quanto ogni singolo sprite è
          composto da 5 byte*/

          case(0x0029):
            I=V[_x]*5;
            break;

          case (0x0033):
            memory[I] = V[_x] / 100;
            memory[I + 1] = (V[_x] / 10) % 10;
            memory[I + 2] = (V[_x] % 100) % 10;
            break;

          case(0x0055):
            for(int i=0;i<=(_x);I++,i++)
              memory[I]=V[i];
            break;

          case(0x0065):
            for(int i=0;i<=(_x);I++,i++)
              V[i]=memory[I];
            break;

          default:
            Display.error();
            std::cout<<"unplemented operation: "<<std::hex<<unsigned(opcode)<<std::endl;
            exit(0);
            break;
        }
        break;



     default:

          Display.error();
          std::cout<<"unplemented operation: "<<std::hex<<unsigned(opcode)<<std::endl;
          exit(0);
          break;
  }

  program_counter+=2;

  /* controllo dei registri sound e delay*/

  if(delay_timer>0)
    delay_timer--;

  }
