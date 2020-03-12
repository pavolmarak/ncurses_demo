// Author: Pavol Marak
// Description: ncurses demo

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <curses.h>

int STARTX = 0;
int STARTY = 0;
int ENDX = 79;
int ENDY = 24;

#define CELL_CHAR '*'
#define TIME_OUT  300

typedef struct _state {
	int oldstate;
	int newstate;
}state;

void cursesStart() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE); // support for F1, F2, F3, ...
}

void cursesStop() {
	getch();
	endwin();
}

void display(WINDOW* win, state** area, int startx, int starty, int endx, int endy)
{
	int i, j;
	wclear(win);
	for (i = startx; i <= endx; ++i)
		for (j = starty; j <= endy; ++j)
			if (area[i][j].newstate == TRUE)
				mvwaddch(win, j, i, CELL_CHAR);
	wrefresh(win);
}

void calc(state** area, int i, int j)
{
	int neighbours;
	int newstate;

	neighbours =
		area[(i - 1 + COLS) % COLS][j].oldstate +
		area[(i - 1 + COLS) % COLS][(j - 1 + LINES) % LINES].oldstate +
		area[(i - 1 + COLS) % COLS][(j + 1) % LINES].oldstate +
		area[(i + 1) % COLS][j].oldstate +
		area[(i + 1) % COLS][(j - 1 + LINES) % LINES].oldstate +
		area[(i + 1) % COLS][(j + 1) % LINES].oldstate +
		area[i][(j - 1 + LINES) % LINES].oldstate +
		area[i][(j + 1) % LINES].oldstate;

	newstate = FALSE;
	if (area[i][j].oldstate == TRUE && (neighbours == 2 || neighbours == 3))
		newstate = TRUE;
	else
		if (area[i][j].oldstate == FALSE && neighbours == 3)
			newstate = TRUE;
	area[i][j].newstate = newstate;
}

void update_state(state** area, int startx, int starty, int endx, int endy)
{
	int i, j;

	for (i = startx; i <= endx; ++i)
		for (j = starty; j <= endy; ++j)
			area[i][j].oldstate = area[i][j].newstate;
}

// *********************************************************************************

void cursesWindow() {
	int nlines = 20;
	int ncols = 80; refresh();
	WINDOW* win = newwin(nlines, ncols, 2, 5);
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	box(win, 0, 0);
	printw("%s", "Enter a number: ");
	int ch = getch();
	wmove(win, 10, 20);
	mvprintw(12, 13, "You entered ");
	waddch(win, ch);
	mvprintw(2, 70, "[ Window ]");
	move(23, 5);
	wrefresh(win);
	refresh();

}

void cursesGameOfLife() {
	state** workarea;
	int i, j;
	timeout(TIME_OUT);
	ENDX = COLS - 1;
	ENDY = LINES - 1;

	workarea = (state**)calloc(COLS, sizeof(state*));
	for (i = 0; i < COLS; ++i)
		workarea[i] = (state*)calloc(LINES, sizeof(state));

	/* For inverted U */
	workarea[39][15].newstate = TRUE;
	workarea[40][15].newstate = TRUE;
	workarea[41][15].newstate = TRUE;
	workarea[39][16].newstate = TRUE;
	workarea[39][17].newstate = TRUE;
	workarea[41][16].newstate = TRUE;
	workarea[41][17].newstate = TRUE;
	update_state(workarea, STARTX, STARTY, ENDX, ENDY);

	/* For block  */
	/*
	workarea[37][13].newstate = TRUE;
	workarea[37][14].newstate = TRUE;
	workarea[38][13].newstate = TRUE;
	workarea[38][14].newstate = TRUE;

	update_state(workarea, STARTX, STARTY, ENDX, ENDY);
	*/
	display(stdscr, workarea, STARTX, STARTY, ENDX, ENDY);
	while (getch() != KEY_F(1))
	{
		for (i = STARTX; i <= ENDX; ++i)
			for (j = STARTY; j <= ENDY; ++j)
				calc(workarea, i, j);
		update_state(workarea, STARTX, STARTY, ENDX, ENDY);
		display(stdscr, workarea, STARTX, STARTY, ENDX, ENDY);
	}
}


void printDot(int row, int col, char dot) {
	//clear();
	refresh();
	mvprintw(row,col,"%c",dot);
	refresh();
}
void cursesKeyboard() {
	int highlight = 1;
	int c, row = 0, col = 0;
	int dir = 1;
	int last_ent = 1;

	nodelay(stdscr, true);
	start_color();
	curs_set(0);

	init_pair(1, 14, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	printDot(row, col, '*');
	do {
		c = getch();
		switch (c) {
		case KEY_UP:
			attroff(COLOR_PAIR(1));
			init_pair(1, 12, COLOR_BLACK);
			attron(COLOR_PAIR(1));
			row--;
			last_ent = 0;
			break;
		case KEY_DOWN:
			attroff(COLOR_PAIR(1));
			init_pair(1,10, COLOR_BLACK);
			attron(COLOR_PAIR(1));
			row++;
			last_ent = 2;
			break;
		case KEY_LEFT:
			attroff(COLOR_PAIR(1));
			init_pair(1, 9, COLOR_BLACK);
			attron(COLOR_PAIR(1));
			col-=2;
			last_ent = 3;
			break;
		case KEY_RIGHT:
			attroff(COLOR_PAIR(1));
			init_pair(1, 14, COLOR_BLACK);
			attron(COLOR_PAIR(1));
			col+=2;
			last_ent = 1;
			break;
		default:
			switch (last_ent) {
			case 0:
				row--;
				break;
			case 1: 
				col+=2;
				break;
			case 2:
				row++;
				break;
			case 3:
				col-=2;
				break;
			}
			
		}
		if (row < 0) {
			row++;
		}
		if (row > 25) {
			row--;
		}
		if (col < 0) {
			col+=2;
		}
		if (col > 95) {
			col-=2;
		}
		printDot(row, col, '*');
		refresh();
		Sleep(80);
	
	} while (c!='q');
	
	refresh();
	//clrtoeol();

}

int main()
{
	cursesStart();
	//cursesWindow();
	//cursesGameOfLife();
	cursesKeyboard();
	cursesStop();
	return 0;
}
