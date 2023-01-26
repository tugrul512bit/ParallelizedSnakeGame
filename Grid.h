

#include "Screen.h"
#include "Bench.h"
template<int W, int H>
struct Grid
{
    // snake + terrain data
    char data[W*H];
    char reduction[W*H];
    short age[W*H];
    short len;
    char x,y;


    const char EMPTY=0;
    const char SNAKE=1;
    const char UP = 2;
    const char DOWN = 3;
    const char LEFT = 4;
    const char RIGHT = 5;

    Grid() { x=W/2; y=H/2; len=1; std::fill(data,data+(W*H),EMPTY); std::fill(age,age+(W*H),0); data[W/2 + (H/2)*W]=SNAKE; age[W/2 + (H/2)*W]=1;  }

    // eat=1 means snake ate
    // eat=0 means snake did not eat
    inline bool compute(const char eat, const char dir, size_t & nanoseconds) noexcept
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


        for(int j=0;j<H;j++)
        for(int i=0;i<W;i++)
        {
            const int index = i+j*W;
            const char val = data[index];
            const char snk = (val==SNAKE);
            const short ag = age[index]-1;
            const char alive = (ag>0);

            const char isX = (x==i);
            const char isY = (y==j);
            const char newHead = isX * isY;
            const char increaseAge = (eat>0);

            age[index] = (snk?(alive?(ag+increaseAge):0):0) + newHead * len;
            data[index] = alive*SNAKE + newHead * SNAKE;
            reduction[index] = newHead * alive;
        }

        // reduction for collision-checking
        int coll = 0;
        for(int j=0;j<H;j++)
        for(int i=0;i<W;i++)
        {
        	coll += (reduction[i+j*W]>0);
        }

        // if game ends
        return coll>0;

    }

    void updateScreen(Screen<W,H> & scr)
    {
        for(int j=0;j<H;j++)
        for(int i=0;i<W;i++)
        {
            scr.data[i+j*W] = (data[i+j*W] == EMPTY) ? '#':'O';
        }
    }
};
