# ParallelizedSnakeGame
Classic Snake-Game With Independent Grid-Updates For Efficient Parallelization And Constant Computation Time

There is only a for loop for iterating through grid points without brancing for efficient parallelization (and auto-vectorization). All computations are made on 16-bit integers (short) so an AVX2 supported CPU core can compute 16 elements at a time or an AVX512 CPU core can compute 32 elements at once. The grid data possibly fits inside L1 cache and vector-operations (and are faster than moving linked-list nodes or their datas (when snake length  > ~100) on the classic memory-efficient approach).

Performance for 96*32 (x-dimension has to be multiple of 32) grid:

- 2.1GHz FX8150: 6.5 microseconds  (```-O3 -march=native -mavx```)
- Cascadelake in godbolt.org: 175 nanoseconds (```-O3 -march=native -mavx512f  -mprefer-vector-width=512```)
- beats 1D-array based snake game at snake length ~750
- beats linked-list based snake game at snake length ~100

128x50: 13 microseconds / 641 nanoseconds

192x64: 26 microseconds / 1.5 microseconds

256x256: 160 microseconds / 8.8 microseconds

## Dependencies

- ncurses (for console output)

- thread (for future multi-threading and benchmarking)

- chrono (for benchmarking)
