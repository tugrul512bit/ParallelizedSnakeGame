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

    // memory for doing reduction for collision checking
    short reduction[W*H];

    // age information of each grid point (to simulate snake segments)
    short age[W*H];

    // snake length (used for deciding max age of snake segments)
    short len;

    // snake head position
    short x,y;

    // empty grid point
    const short EMPTY=0;

    // filled grid point
    const short SNAKE=1;

    // keyboard codes for snake head direction
    const short UP = 2;
    const short DOWN = 3;
    const short LEFT = 4;
    const short RIGHT = 5;

    Grid() { x=W/2; y=H/2; len=1; std::fill(data,data+(W*H),EMPTY); std::fill(age,age+(W*H),0); data[W/2 + (H/2)*W]=SNAKE; age[W/2 + (H/2)*W]=1;  }

    // eat=1 means snake ate
    // eat=0 means snake did not eat
    inline bool compute(const short eat, const short dir, size_t & nanoseconds) noexcept
    {
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


        for(short j=0;j<H;j++)
        for(short i=0;i<W;i++)
        {
        	// current grid index
            const short index = i+j*W;

            // current grid point data
            const short val = data[index];

            // is this point a snake?
            const short snk = (val==SNAKE);

            // age of current point with decrement (benjamin button)
            const short ag = age[index]-1;

            // is this point alive? (0=dead)
            const short alive = (ag>0);

            // is this point on snake-head position?
            const short isX = (x==i);
            const short isY = (y==j);
            const short newHead = isX * isY;

            // if eating is enabled, age is increased (1 more frame life)
            const short increaseAge = (eat>0);

            // if it is head, age = maximum, if it is tail and eating, age = inceased, if it is tail but not eating, age = decreased
            age[index] = (snk*(alive*(ag+increaseAge))) + newHead * len;

            // if snake is alive, then this is a snake point, if it is head of snake, it is also a snake point
            data[index] = alive*SNAKE + newHead * SNAKE;

            // if this is collision with head
            reduction[index] = newHead * alive;
        }

        // accumulator for collision checking
        alignas(32)
        short coll[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        // zero register
        alignas(32)
        const short zero[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        // reduction for collision checking
        for(short j=0;j<H;j++)
        for(short i=0;i<W;i+=32)
        {
        	for(short lane=0;lane<32;lane++)
        	{
        		coll[lane] += (reduction[i+lane+j*W]>zero[lane]);
        	}
        }

        short res = 0;
        for(short lane=0;lane<32;lane++)
        	res += coll[lane];

        // if game ends
        return res>0;

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
