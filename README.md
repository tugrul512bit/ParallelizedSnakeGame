# ParallelizedSnakeGame
Classic Snake-Game With Independent Grid-Updates For Efficient Parallelization And Constant Computation Time

There is only a for loop for iterating through grid points without brancing for efficient parallelization (and auto-vectorization). All computations are made on 16-bit integers (short) so an AVX2 supported CPU core can compute 16 elements at a time or an AVX512 CPU core canc ompute 32 elements at once. The grid data possibly fits inside L1 cache and vector-operations are made efficiently.

Performance for 96*32 grid:

- 2.1GHz FX8150: 13.5 microseconds  (```-O3 -march=native -mavx```)
- Cascadelake in godbolt.org: 175 nanoseconds (```-O3 -march=native -mavx512f  -mprefer-vector-width=512```)

## Dependencies

- ncurses (for console output)

- thread (for future multi-threading and benchmarking)

- chrono (for benchmarking)
