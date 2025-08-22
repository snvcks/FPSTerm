#include <ncurses.h>

int main()
{	
	initscr();			/* Start curses mode 		  */
	
	int height, width, start_y, start_x;
    height = 10;
    width = 20;
    start_x = start_y = 10;

    //pointer to window 
    WINDOW * win = newwin(height, width, start_y, start_x);

    refresh();
    box(win,0,0);
    wrefresh(win);

    wprintw(win,"my box");

    int c = getch();



	endwin();			/* End curses mode		  */

	return 0;
}
