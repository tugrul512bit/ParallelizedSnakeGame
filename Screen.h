/*
 * Screen.h
 *
 *  Created on: Jan 26, 2023
 *      Author: tugrul
 */
#include <ncurses.h>
#ifndef SCREEN_H_
#define SCREEN_H_


template<short W, short H>
struct Screen
{
    const int width,height;
    char data[W*H];
    Screen():width(W),height(H){ std::fill(data,data+(W*H),'#'); }


    void render(size_t nanoseconds)
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
        attroff(COLOR_PAIR(1));
        refresh();
    }
};


#endif /* SCREEN_H_ */
