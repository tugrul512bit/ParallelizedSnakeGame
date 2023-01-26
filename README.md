# ParallelizedSnakeGame
Classic Snake-Game With Independent Grid-Updates For Efficient Parallelization And Constant Computation Time

There is only a for loop for iterating through grid points without brancing for efficient parallelization (and auto-vectorization).

96*32 grid is computed in few microseconds regardless of length of snake.
