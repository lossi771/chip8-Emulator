#include "utility.h"
#include <ncurses.h>

class OutputVideo{

private:

  WINDOW* win;
  uint8_t display [64][32];

public:

  OutputVideo(WINDOW* w);
  ~OutputVideo();

  void error(){
    endwin();
  }


  uint8_t drawPixels(uint8_t pixels,uint8_t x,uint8_t y);

  void clear();

};
