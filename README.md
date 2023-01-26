# ParallelizedSnakeGame
Classic Snake-Game With Independent Grid-Updates For Efficient Parallelization And Constant Computation Time

There is only a for loop for iterating through grid points without brancing for efficient parallelization (and auto-vectorization). All computations are made on 16-bit integers (short) so an AVX2 supported CPU core can compute 16 elements at a time or an AVX512 CPU core can compute 32 elements at once.

Performance for 96*32 (Width X Height) grid (width has to be multiple of 32):

- 2.1GHz FX8150: 6.5 microseconds  (```-O3 -march=native -mavx```)
- Cascadelake in godbolt.org: 175 nanoseconds (```-O3 -march=native -mavx512f  -mprefer-vector-width=512```)
- beats 1D-array based snake game at snake length ~750
- beats linked-list based snake game at snake length ~100

128x50: 12.5 microseconds / 548 nanoseconds

192x64: 23 microseconds / 953 nanoseconds

256x256: 115 microseconds / 5.6 microseconds

## Dependencies

- ncurses (for console output)

- thread (for future multi-threading and benchmarking)

- chrono (for benchmarking)
