# ParallelizedSnakeGame
Classic Snake-Game With Branchless Independent Grid-Updates For Efficient Parallelization And Constant Computation Time

There is only a for loop for iterating through grid points without branching for efficient parallelization (and auto-vectorization). All computations are made on 16-bit integers (short) so an AVX2 supported CPU core can compute 16 elements at a time or an AVX512 CPU core can compute 32 elements at once.

Performance for 96*32 (Width X Height) grid (width = multiple of 16 means higher performance due to SIMD capabilities of CPUs):

- 2.1GHz FX8150 CPU: **2.2 microseconds**  (```-O3 -march=native -mavx```)
- Cascadelake CPU in godbolt.org: **76 nanoseconds** (```-O3 -march=native -mavx512f  -mprefer-vector-width=512```)
- beats 1D-array based snake game at snake length ~200
- beats linked-list based snake game at snake length ~25

128x50: Bulldozer **4.1 microseconds** / Cascadelake **144 nanoseconds**

192x64: 10.6 microseconds / **277 nanoseconds (data in L1 cache)**

256x256: 47 microseconds / **1.9 microseconds (data in L2 cache)**

1024x1024: 543 microseconds / **89 microseconds (data in L3 cache)**

## Dependencies

- ncurses (for console output)

- thread (for future multi-threading and benchmarking)

- chrono (for benchmarking)
