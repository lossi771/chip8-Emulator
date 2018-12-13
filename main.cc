#include "chip8.h"
#include <unistd.h>
#include <ncurses.h>

using namespace std;
int main(int argc, char const *argv[]) {
  initscr();
  curs_set(0);
  noecho();
  cbreak();
  start_color();
  use_default_colors();

  WINDOW* win=newwin(34,132,0,0);
  wrefresh(win);


  refresh();
  wrefresh(win);
  chip8 _chip8=chip8(win);
  _chip8.inizialize();
  _chip8.loadROM(argv[1]);
  for(;;){

    _chip8.fetch();
    _chip8.execute();

    usleep(1800);
  }
  endwin();
  return 0;
}
