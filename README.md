# ParallelizedSnakeGame
Classic Snake-Game With Branchless Independent Grid-Updates For Efficient Parallelization And Constant Computation Time

There is only a for loop for iterating through grid points without branching for efficient parallelization (and auto-vectorization). All computations are made on 16-bit integers (short) so an AVX2 supported CPU core can compute 16 elements at a time or an AVX512 CPU core can compute 32 elements at once.

Performance for 96*32 (Width X Height) grid (width has to be multiple of 32):

- 2.1GHz FX8150 CPU: 6.5 microseconds  (```-O3 -march=native -mavx```)
- Cascadelake CPU in godbolt.org: 175 nanoseconds (```-O3 -march=native -mavx512f  -mprefer-vector-width=512```)
- beats 1D-array based snake game at snake length ~750
- beats linked-list based snake game at snake length ~100

128x50: 12.5 microseconds / 548 nanoseconds

192x64: 23 microseconds / 953 nanoseconds (data in L1 cache)

256x256: 115 microseconds / 5.6 microseconds (data in L2 cache)

1024x1024: 1.9 milliseconds / 210 microseconds (data in L3 cache)

## Dependencies

- ncurses (for console output)

- thread (for future multi-threading and benchmarking)

- chrono (for benchmarking)
