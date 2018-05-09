//
//  main.cpp
//  conways-life-ncurses
//
//  Created by Michael Simanski on 11/15/17.
//  Copyright © 2017 Michael Simanski. All rights reserved.
//

#include <iostream>
#include <functional>
#include <chrono>
#include <future>
#include <cstdio>
#include <ncurses.h>

using namespace std;

class callBackTimer //no idea how this works, got it from Stack Overflow thread
{

public:
    callBackTimer()
    :_execute(false)
    {}

    void setup(int interval, std::function<void(void)> func)
    {
        _execute = true;
        thread = std::thread([=]()
                             {
                                 while (_execute)
                                 for (int steps = 0; steps < 100; ++steps)
                                 {
                                     func();
                                     std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                                 }
                             });
    }

    void stop()
    {
        _execute = false;
    }

    void run()
    {
        thread.join();
    }

private:
    bool _execute;
    std::thread thread;

};

class world //class representing the world
{

public:

	int worldstate[300][300]; //this is the world, 300x300 spaces

    world(int (&world)[300][300])
    {

        srand(time(NULL)); //seed the rand with current system time
        int randone, randtwo; //the x and y for the first "spaceship"
        randone = (rand() % 40) + 1; //random x
        randtwo = (rand() % 40) + 1; //random y

        for (int i = 0; i < 300; i++) // build random movers
        {
            world[randone][randtwo] = 1;
            world[randone + 1][randtwo] = 1;
            world[randone + 2][randtwo] = 1;
            world[randone + 2][randtwo - 1] = 1;
            world[randone + 1][randtwo - 1] = 1;
        }

        world[100][100] = 1; // build center "spaceship"
        world[99][100] = 1;
        world[101][100] = 1;
        world[100][101] = 1;
        world[100][99] = 1;

    }

    int getWorldStateAt (int i, int j, int (&world)[300][300]) //see if living or dead at (i,j)
    {
        int temp = world[i][j];
        return temp;
    }

    void placeCell(int x, int y)
    {
      this->worldstate[x][y] = 1;
    }

    int updateWordState () //main algorithm
    {

		int n1,n2,n3,n4,n5,n6,n7,n8,total; // all possible buddies of a cell

		for (int i = 1; i < 299; i++) //go to each cell
		{
			for (int j = 1; j < 299; j++)
			{

				n1 = this->worldstate[i-1][j]; // look up the neighborhood
				n2 = this->worldstate[i][j-1];
				n3 = this->worldstate[i-1][j-1];
				n4 = this->worldstate[i+1][j];
				n5 = this->worldstate[i][j+1];
				n6 = this->worldstate[i+1][j+1];
				n7 = this->worldstate[i+1][j-1];
				n8 = this->worldstate[i-1][j+1];

				total = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8; // add them up

				if (total < 2 && (this.worldstate[i][j] == 1)) //rule one
				{
					this.worldstate[i][j] = 0;
				}
				if ((total == 2 || total == 3) && (this.worldstate[i][j] == 1)) //rule two
				{
					this.worldstate[i][j] = 1;
				}
				if (total > 3 && (this.worldstate[i][j] == 1)) //rule three
				{
					this.worldstate[i][j] = 0;
				}
				if ((total == 3) && (this.worldstate[i][j] == 0)) //rule four
				{
					this.worldstate[i][j] = 1;
				}
			}
		}

	}

};

void timerExec() //where it all goes down when the timer ticks
{
    cout << "SNAFU" << endl;
}

int main(int argc, const char * argv[])
{

    initscr(); //create stdscr
    raw(); //just keep going

    callBackTimer timer; //declare the timer
    std::function<void(void)> exec = timerExec; //declare a pointer to timerExec
    timer.setup(25, std::bind(exec)); //start the timer
    timer.run(); //start the timer

    endwin(); //kill the window, and stdwin, as of now wont ever execute

    return 0;
}
