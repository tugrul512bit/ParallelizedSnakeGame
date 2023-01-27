/*
 * Grid.h
 *
 *  Created on: Jan 26, 2023
 *      Author: tugrul
 */

#include "Screen.h"
#include "Bench.h"
template<int W, int H>
struct Grid
{

    // age information of each grid point (to simulate snake segments)
    short age[W*H];

    // snake length (used for deciding max age of snake segments)
    short len;

    // snake head position
    short x,y;


    // keyboard codes for snake head direction
    const short UP = 2;
    const short DOWN = 3;
    const short LEFT = 4;
    const short RIGHT = 5;

    Grid() { x=W/2; y=H/2; len=1;  std::fill(age,age+(W*H),0);  age[W/2 + (H/2)*W]=1;  }

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

    	// is head of snake colliding?
    	bool collided = (age[x+W*y]>0);

    	const short eating = eat>0;

        for(int j=0;j<H;j++)
        for(int i=0;i<W;i++)
        {
        	// current grid index
            const int index = i+j*W;

            // age of current point with decrement (benjamin button)
            const short ag = age[index]-1;

            // is this point alive? (0=dead)
            const short alive = (ag>0);

            // clamp new age to [0,n)
            const short newAge = ((ag<0)?0:ag);

            // if it is head, age = maximum, if it is tail and eating, age = inceased, if it is tail but not eating, age = decreased
            age[index] = alive * (newAge + eating);

        }

        // add snake head
        age[x+W*y]=len;

        // if head collides, game ends
        return collided;

    }


    void updateScreen(Screen<W,H> & scr)
    {
        for(short j=0;j<H;j++)
        for(short i=0;i<W;i++)
        {
            scr.data[i+j*W] = (age[i+j*W] == 0) ? '.':'O';
        }
    }
};
