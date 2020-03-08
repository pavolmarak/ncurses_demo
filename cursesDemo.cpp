// Author: Pavol Marak
// Description: ncurses demo

#include <stdio.h>
#include <curses.h>

void cursesInit() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
}

int main()
{
	cursesInit();
	int nlines = 20;
	int ncols = 80; refresh();
	WINDOW* win = newwin(nlines, ncols, 2, 5);
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	box(win,0 , 0);
	printw("%s", "Enter a number: ");
	int ch = getch();
	wmove(win,10,20);
	mvprintw(12,13,"You entered ");
	waddch(win, ch);	
	mvprintw(2, 65, "~  Window  ~");
	move(23, 5);
	wrefresh(win);
	getch();
	endwin();

	return 0;
}
