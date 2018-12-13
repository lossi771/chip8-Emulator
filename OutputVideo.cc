#include"OutputVideo.h"



OutputVideo::OutputVideo(WINDOW* w){
  init_pair(1,COLOR_WHITE,COLOR_WHITE);
  init_pair(2,-1,-1);
  win=w;
  wattron(win,COLOR_PAIR(1));
  for(int x=0;x<64;x++)
    for(int y=0;y<32;y++)
      display[x][y]=0;
}

OutputVideo::~OutputVideo(){}

uint8_t OutputVideo::drawPixels(uint8_t pixels,uint8_t x,uint8_t y){
  uint8_t cancelled=0;
  /*SISTEMA CHE ANCHE I SUCCESSIVI PIXEL POSSONO SCAVALLARE*/
  y=y%32;
  x=x%64;
  for(int index=0;index<8;index++){
    if(x+index>63)
      x=x+index-64;
    if((pixels & (0x80 >> index)) != 0){
      if(display[x+index][y]==1){
        cancelled=1;
        display[x+index][y]=0;
        wattron(win,COLOR_PAIR(2));
        mvwprintw(win,(y+1),((x*2)+2)+2*index,"  ");
      }
      else{
        wattron(win,COLOR_PAIR(1));
        mvwprintw(win,(y+1),((x*2)+2)+2*index,"XX");
        display[x+index][y]=1;
      }
    }
  }
  refresh();
  wrefresh(win);
  return cancelled;
}


void OutputVideo::clear(){
  wattron(win,COLOR_PAIR(2));
  for (int y=1;y<34;y++)
    for(int x=2;x<132;x+=2){
      mvwprintw(win,y,x,"  ");
  }
  for(int x=0;x<64;x++)
    for(int y=0;y<32;y++)
      display[x][y]=0;
  refresh();
  wrefresh(win);
}
