# ParallelizedSnakeGame
Classic Snake-Game With Independent Grid-Updates For Efficient Parallelization And Constant Computation Time

There is only a for loop for iterating through grid points without brancing for efficient parallelization (and auto-vectorization).

Performance for 96*32 grid:

- 2.1GHz FX8150: 8 microseconds
- Cascadelake in godbolt.org: 500 nanoseconds

## Dependencies

- ncurses (for console output)

- thread (for future multi-threading and benchmarking)

- chrono (for benchmarking)
