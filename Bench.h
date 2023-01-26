/*
 * Bench.h
 *
 *  Created on: Jan 26, 2023
 *      Author: tugrul
 */

#ifndef BENCH_H_
#define BENCH_H_
#include <chrono>
#include <thread>
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
