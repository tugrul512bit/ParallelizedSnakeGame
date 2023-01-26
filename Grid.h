/*
 * Bench.h
 *
 *  Created on: Jan 26, 2023
 *      Author: tugrul
 */

#include "Screen.h"
#include "Bench.h"
template<short W, short H>
struct Grid
{
    // snake + terrain data
    short data[W*H];
    short reduction[W*H];
    short age[W*H];
    short len;
    short x,y;


    const short EMPTY=0;
    const short SNAKE=1;
    const short UP = 2;
    const short DOWN = 3;
    const short LEFT = 4;
    const short RIGHT = 5;

    Grid() { x=W/2; y=H/2; len=1; std::fill(data,data+(W*H),EMPTY); std::fill(age,age+(W*H),0); data[W/2 + (H/2)*W]=SNAKE; age[W/2 + (H/2)*W]=1;  }

    // eat=1 means snake ate
    // eat=0 means snake did not eat
    inline bool compute(const short eat, const short dir, size_t & nanoseconds) noexcept
    {
    	Bench bench(&nanoseconds);

    	x += (dir == RIGHT);
    	x -= (dir == LEFT);
    	y += (dir == DOWN);
    	y -= (dir == UP);
    	len += eat;

    	x += (x<0)*(W);
    	x -= (x>W-1)*(W);
    	y += (y<0)*(H);
    	y -= (y>H-1)*(H);


        for(short j=0;j<H;j++)
        for(short i=0;i<W;i++)
        {
            const short index = i+j*W;
            const short val = data[index];
            const short snk = (val==SNAKE);
            const short ag = age[index]-1;
            const short alive = (ag>0);

            const short isX = (x==i);
            const short isY = (y==j);
            const short newHead = isX * isY;
            const short increaseAge = (eat>0);

            age[index] = (snk?(alive?(ag+increaseAge):0):0) + newHead * len;
            data[index] = alive*SNAKE + newHead * SNAKE;
            reduction[index] = newHead * alive;
        }

        // reduction for collision-checking
        short coll = 0;
        for(short j=0;j<H;j++)
        for(short i=0;i<W;i++)
        {
        	coll += (reduction[i+j*W]>0);
        }

        // if game ends
        return coll>0;

    }


    void updateScreen(Screen<W,H> & scr)
    {
        for(short j=0;j<H;j++)
        for(short i=0;i<W;i++)
        {
            scr.data[i+j*W] = (data[i+j*W] == EMPTY) ? '#':'O';
        }
    }
};
