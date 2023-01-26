
#include <string>
#include <iostream>

#include "Grid.h"





int main(int argc, char *argv[])
{


	const int W=96;
	const int H=32;

	int ch;

	initscr();
	resizeterm(W+2,H+2);
	cbreak();
	keypad(stdscr, TRUE);




	Screen<W,H> screen;
	Grid<W,H> grid;
	char dir=grid.UP;
    bool exit = false;
    nodelay(stdscr, TRUE);
    bool eat = false;
    size_t avgTime = 0;
    int ctFrame = 0;
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

		// if head collides tail
		if(grid.compute( eat=!eat,dir,nanoseconds))
		{
			avgTime+=nanoseconds;
			endwin();
			std::cout<<"score="<<grid.len<<std::endl;
			std::cout<<"average compute time = "<<avgTime/ctFrame<<" nanoseconds"<<std::endl;
			return 0;
		}
		avgTime+=nanoseconds;
		grid.updateScreen(screen);
        screen.render(nanoseconds);
	}

	endwin();			/* End curses mode		  */
	std::cout<<"score="<<grid.len<<std::endl;
	std::cout<<"average compute time = "<<avgTime/ctFrame<<" nanoseconds"<<std::endl;
	return 0;
}

