#include <ncurses.h>

int main()
{	
	initscr();			/* Start curses mode 		  */
	
    printw("Hello World !!!");	/* Print Hello World		  */
	
    refresh();			/* Print it on to the real screen */


	int c = getch();			/* Wait for user input */


    int y = 10;
    int x = 10;
    //move
    move(y,x);

    printw("Meow %d",c);

    getch();

	endwin();			/* End curses mode		  */

	return 0;
}
