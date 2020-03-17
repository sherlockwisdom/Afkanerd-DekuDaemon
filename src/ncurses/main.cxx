#include <ncurses.h>

int main() {
	// Initializes the start of the curses mode
	initscr();

	// Seems other = printw(stream, y, x) y = line number \n, x = tab | space
	// This data is printed to a buffer, and in other to see what the buffer has - refresh is called to create a dump
	
	// This
	char ch = 'A';
	addch(ch | A_BOLD | A_UNDERLINE);
	addch(ch | A_UNDERLINE);
	refresh();

	/*
	// Like is says, don't echo what doesn't need some echo

	noecho();

	// Line buffering is disabled here
	// raw();

	// Allow capturing of the F1 and Arrow keys
	keypad(stdscr, TRUE);

	// Line buffering is enabled here
	cbreak();

	// This clears the buffer and dumpsthe data to stdscr (data structure holding the current stream buffer)
	refresh();


	//This ends curses
	*/
	getch();
	endwin();

	return 0;
}
