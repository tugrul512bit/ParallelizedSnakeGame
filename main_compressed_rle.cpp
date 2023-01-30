// this version is too slow because RLE is not optimized much, just hastily applied to every row of 2D grid of ages

#ifndef BENCH_H_
#define BENCH_H_
#include <chrono>
#include <thread>
#include <iostream>
class Bench
{
public:
	Bench(size_t * targetPtr)
	{
		target=targetPtr;
		t1 =  std::chrono::duration_cast< std::chrono::nanoseconds >(std::chrono::high_resolution_clock::now().time_since_epoch());
	}

	~Bench()
	{
		t2 =  std::chrono::duration_cast< std::chrono::nanoseconds >(std::chrono::high_resolution_clock::now().time_since_epoch());
		if(target)
		{
			*target= t2.count() - t1.count();
		}
		else
		{
			std::cout << (t2.count() - t1.count())/1000000000.0 << " seconds" << std::endl;
		}
	}
private:
	size_t * target;
	std::chrono::nanoseconds t1,t2;
};




#endif /* BENCH_H_ */

/*
 * Screen.h
 *
 *  Created on: Jan 26, 2023
 *      Author: tugrul
 */
#include <ncurses.h>
#ifndef SCREEN_H_
#define SCREEN_H_


template<int W, int H>
struct Screen
{
    char data[W*H];
    Screen(){ std::fill(data,data+(W*H),'#'); }


    void render(size_t nanoseconds,float compressionRatio)
    {
        start_color();
        init_pair(1, COLOR_RED, COLOR_GREEN);
        attron(COLOR_PAIR(1));

        for(int j=0;j<H;j++)
        for(int i=0;i<W;i++)
        {
            move(j,i);
            printw("%c",data[i+j*W]);
        }
        move(H+1,0);
        printw("%lu nanoseconds",nanoseconds);
        move(H+2,0);
		printw("compression ratio = %fx",1.0f/compressionRatio);

        attroff(COLOR_PAIR(1));
        refresh();
    }
};


#endif /* SCREEN_H_ */


#include<vector>

template<typename DT=unsigned short>
    class RunLengthEncoderString
    {
    public:
    	RunLengthEncoderString(){}
    	RunLengthEncoderString(DT * str, int size)
    	{
    		const int szBlock = 8;
    		const size_t sz = size;
    		const size_t sz8 = sz-(sz%szBlock);
    		DT last = ((size>0)?str[0]:0);
    		DT count = 0;
    		counts.resize(sz);
    		chars.resize(sz);


    		alignas(64)
    		DT tmp[szBlock];

    		size_t nsz = 0;
    		for(size_t i=0;i<sz8;i+=szBlock)
    		{
    			for(int j=0;j<szBlock;j++)
    			{
    				if(i+j<sz)
    				{
    					tmp[j]=str[i+j];
    				}
    			}

    			for(int j=0;j<szBlock;j++)
    			{
    				if(i+j<sz)
    				{
						DT cur = tmp[j];
						auto cond = (count==(256*256-1) || (last!=cur));

						counts[nsz]=(cond?count:counts[nsz]);
						chars[nsz]=(cond?last:chars[nsz]);
						nsz+=(cond?1:0);
						count=(cond?1:count);



						count+=((!cond) ? 1: 0);

						last=cur;
    				}
    			}
    		}

    		for(size_t i=sz8;i<sz;i++)
    		{
    			DT cur = str[i];
    			if(count==(256*256-1) || (last!=cur))
    			{

    				counts[nsz]=count;
    				chars[nsz]=last;
    				nsz++;
    				count=1;
    			}
    			else
    			{
    				count++;
    			}
    			last=cur;
    		}



    		if(count>0)
    		{
    			counts[nsz]=count;
    			chars[nsz]=last;
    			nsz++;
    		}

    		counts.resize(nsz);
    		chars.resize(nsz);
    		counts.shrink_to_fit();
    		chars.shrink_to_fit();

    	}


    	void compressedData(std::vector<DT> & str)
    	{
    		if(str.size()!=chars.size())
    			str.resize(chars.size());
    		std::copy(chars.begin(),chars.end(),str.begin());

    	}

    	void compressedPrefix(std::vector<DT> & str)
    	{
    		if(str.size()!=counts.size())
    			str.resize(counts.size());
    		std::copy(counts.begin(),counts.end(),str.begin());

    	}

    	void clearCompressedData()
    	{
    		chars=std::vector<DT>();
    	}

    	void clearCompressedPrefix()
    	{
    		counts=std::vector<DT>();
    	}

    	void setCompressedData(std::vector<DT> & str)
    	{
    		chars = str;
    	}

    	void setCompressedPrefix(std::vector<DT> & str)
    	{
    		counts = str;
    	}

        template<typename DT2>
    	void string(DT2 * ptr)
    	{
    		const size_t sz = counts.size();

    		int tot = 0;
    		for(size_t i=0; i<sz; i++)
    		{
    		    std::fill(ptr+tot,ptr+tot+counts[i],chars[i]);
                tot += counts[i];
    		}

    	}

    	size_t compressedSize()
    	{
    		return (counts.size() + counts.size())*sizeof(DT);
    	}
    private:
    	std::vector<DT> chars;
    	std::vector<DT> counts;
    };


template<int W, int H>
struct Grid
{

    // age information of each grid point (to simulate snake segments)
	alignas(64)
    short age[W*H];


    std::vector<unsigned short> compressedRowsData[H];
    std::vector<unsigned short> compressedRowsPrefix[H];

    // snake length (used for deciding max age of snake segments)
	alignas(64)
    short len;

    // snake head position
	alignas(64)
    short x,y;


    // keyboard codes for snake head direction
    const short UP = 2;
    const short DOWN = 3;
    const short LEFT = 4;
    const short RIGHT = 5;

    Grid() { x=W/2; y=H/2; len=1;  std::fill(age,age+(W*H),0);  age[W/2 + (H/2)*W]=1; initRleEncode();  }


    void initRleEncode()
    {
        for(int j=0;j<H;j++)
        {
            RunLengthEncoderString<unsigned short> rle((unsigned short *)&age[W*j],W);
            rle.compressedData(compressedRowsData[j]);
            rle.compressedPrefix(compressedRowsPrefix[j]);
        }
    }

    // eat=1 means snake ate
    // eat=0 means snake did not eat
    inline bool compute(const short eat, const short dir, size_t & nanoseconds, float & compressionRatio) noexcept
    {
    	float compressionTotal = W*H;
    	float compressionCurrent = 0;
    	// RAII benchmarking utility to measure running-time of compute(..)
    	Bench bench(&nanoseconds);

    	// move snake head position
    	x += (dir == RIGHT);
    	x -= (dir == LEFT);
    	y += (dir == DOWN);
    	y -= (dir == UP);

    	// eating = snake increases length
    	len += eat;

    	// out-of-bounds = wrapped-back
    	x += (x<0)*(W);
    	x -= (x>W-1)*(W);
    	y += (y<0)*(H);
    	y -= (y>H-1)*(H);

    	// is head of snake colliding?
    	bool collided = false;

    	const short eating = eat>0;

        for(int j=0;j<H;j++)
        {
        	// decompress row
            RunLengthEncoderString<unsigned short> rleRead;
            rleRead.setCompressedData(compressedRowsData[j]);
            rleRead.setCompressedPrefix(compressedRowsPrefix[j]);
            rleRead.string<short>(age);

    		#pragma GCC ivdep
            for(int i=0;i<W;i++)
            {


                // age of current point with decrement (benjamin button)
                const short ag = age[i]-1;

                // is this point alive? (0=dead)
                const short alive = (ag>0);

                // clamp new age to [0,n)
                const short newAge = ((ag<0)?0:ag);

                // if it is head, age = maximum, if it is tail and eating, age = inceased, if it is tail but not eating, age = decreased
                age[i] = alive * (newAge + eating);

            }


            if(y==j)
                collided=(age[x]>0);

            if(j==y)
                age[x]=len;

            // compress row
            RunLengthEncoderString<unsigned short> rleWrite((unsigned short *)age,W);
            rleWrite.compressedData(compressedRowsData[j]);
            rleWrite.compressedPrefix(compressedRowsPrefix[j]);
            compressionCurrent += compressedRowsData[j].size() + compressedRowsPrefix[j].size();

        }
        // add snake head
        compressionRatio = compressionCurrent / compressionTotal;

        // if head collides, game ends
        return collided;

    }


    void updateScreen(Screen<W,H> & scr)
    {
        for(short j=0;j<H;j++)
        {
            RunLengthEncoderString<unsigned short> rleRead;
            rleRead.setCompressedData(compressedRowsData[j]);
            rleRead.setCompressedPrefix(compressedRowsPrefix[j]);
            rleRead.string<short>(age);
            for(short i=0;i<W;i++)
            {
                scr.data[i+j*W] = (age[i] == 0) ? '.':'O';
            }
        }
    }
};

#include <string>
#include <iostream>



// changed name to not do redeclaration of main
int main_compressed(int argc, char *argv[])
{

	// x-dimension to be multiples of 16 for performance
	const int W=64;

	// can be anything
	const int H=32;

	int ch;

	resizeterm(W+2,H+3);
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
		float compressionRatio;
		// if head collides tail, then exit game
		if(grid.compute( eat=!eat,dir,nanoseconds,compressionRatio))
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
        screen.render(nanoseconds,compressionRatio);
	}

	// ncurses finish
	endwin();

	// output performance of player and computer
	std::cout<<"score="<<grid.len<<std::endl;
	std::cout<<"average compute time = "<<avgTime/ctFrame<<" nanoseconds"<<std::endl;
	return 0;
}
