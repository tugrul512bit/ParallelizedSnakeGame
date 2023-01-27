
#include <string>
#include <iostream>

#include "Grid.h"


int main(int argc, char *argv[])
{

	// x-dimension to be multiples of 16 for performance
	const int W=112;

	// can be anything
	const int H=32;

	int ch;

	resizeterm(W+2,H+2);
	initscr();
	clear();

	cbreak();
	keypad(stdscr, TRUE);



	// ncurses console screen
	Screen<W,H> screen;

	// snake game grid [ compute method calculates the main game-logic ]
	Grid<W,H> grid;

	// star snake facing upwards
	char dir=grid.UP;

	// game exits when snake head collides with a snake
    bool exit = false;

    // no delay for keyboard input
    nodelay(stdscr, TRUE);

    // eating latch (game gives apple automatically in every second frame)
    bool eat = false;

    // benchmark variables
    size_t avgTime = 0;
    int ctFrame = 0;

    // game loop
	while(true)
	{
		ctFrame++;
	    std::this_thread::sleep_for(std::chrono::nanoseconds(100000000));

		switch(ch=getch())
		{	case KEY_LEFT:
				dir=grid.LEFT;
				break;
			case KEY_RIGHT:
				dir=grid.RIGHT;
				break;
			case KEY_UP:
				dir=grid.UP;
				break;
			case KEY_DOWN:
				dir=grid.DOWN;
				break;
			case 27:
				exit = true;
				break;
		}

		if(exit)
		    break;

		size_t nanoseconds;

		// if head collides tail, then exit game
		if(grid.compute( eat=!eat,dir,nanoseconds))
		{
			avgTime+=nanoseconds;

			// ncurses finish
			endwin();

			// output performance of player and computer
			std::cout<<"score="<<grid.len<<std::endl;
			std::cout<<"average compute time = "<<avgTime/ctFrame<<" nanoseconds"<<std::endl;
			return 0;
		}

		// update screen and accumulate average frame time
		avgTime+=nanoseconds;
		grid.updateScreen(screen);
        screen.render(nanoseconds);
	}

	// ncurses finish
	endwin();

	// output performance of player and computer
	std::cout<<"score="<<grid.len<<std::endl;
	std::cout<<"average compute time = "<<avgTime/ctFrame<<" nanoseconds"<<std::endl;
	return 0;
}
