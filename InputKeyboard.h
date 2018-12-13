#include "utility.h"
#include <ncurses.h>
#include<chrono>


class InputKeyboard{

private:

  WINDOW* win;
  int character;
  uint8_t keys[16];
  std::chrono::time_point<std::chrono::steady_clock> key_time[16];
  
public:

  InputKeyboard(WINDOW* w){
    win=w;
    character=0;
    std::fill(keys,keys+16,0);
    nodelay(win,true);
  };

  ~InputKeyboard(){};

  uint8_t getBlockingCharacter(){
    while(character=getch()){
      if(character>=48 && character<=57){
        return character-48;
      }
      if(character>=97 && character<=102){
        return character-87;
      }
    }
  };

  bool checkCharacter(uint8_t index){
    return keys[index]!=0 ? true : false;
  };

  void getKeys(){
    check_time();
    while(character=wgetch(win)){
      switch(character){
        case 48:
          keys[0]=1;
          key_time[0] = std::chrono::steady_clock::now();
          break;

        case 49:
          keys[1]=1;
          key_time[1] = std::chrono::steady_clock::now();
          break;

        case 50:
          keys[2]=1;
          key_time[2] = std::chrono::steady_clock::now();
          break;

        case 51:
          keys[3]=1;
          key_time[3] = std::chrono::steady_clock::now();
          break;

        case 52:
          keys[4]=1;
          key_time[4] = std::chrono::steady_clock::now();
          break;

        case 53:
          keys[5]=1;
          key_time[5] = std::chrono::steady_clock::now();
          break;

        case 54:
          keys[6]=1;
          key_time[6] = std::chrono::steady_clock::now();
          break;

        case 55:
          keys[7]=1;
          key_time[7] = std::chrono::steady_clock::now();
          break;

        case 56:
          keys[8]=1;
          key_time[8] = std::chrono::steady_clock::now();
          break;

        case 57:
          keys[9]=1;
          key_time[9] = std::chrono::steady_clock::now();
          break;

        case 97:
          keys[10]=1;
          key_time[10] = std::chrono::steady_clock::now();
          break;

        case 98:
          keys[11]=1;
          key_time[11] = std::chrono::steady_clock::now();
          break;

        case 99:
          keys[12]=1;
          key_time[12] = std::chrono::steady_clock::now();
          break;

        case 100:
          keys[13]=1;
          key_time[13] = std::chrono::steady_clock::now();
          break;

        case 101:
          keys[14]=1;
          key_time[14] = std::chrono::steady_clock::now();
          break;

        case 102:
          keys[15]=1;
          key_time[15] = std::chrono::steady_clock::now();
          break;

        case ERR:
          return;

        default:
          break;

      }
    }
  };

  void check_time(){
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    for(int index=0;index<15;index++){
      if(keys[index]!=0){
        long us = std::chrono::duration_cast<std::chrono::milliseconds>(now - key_time[index]).count();
        if(us>300)
          keys[index]=0;
      }
    }

  }

};
