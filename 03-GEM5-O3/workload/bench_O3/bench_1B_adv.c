/*
 * Benchmark 1B-IND — Independent version of the same computation
 * ---------------------------------------------------------------
 * Structurally identical work volume to bench_1b_dependent.c,
 * but operations are distributed across independent register chains.
 *
 * Run this alongside bench_1b_dependent.c and compare:
 *   - IPC  (expect significantly higher here)
 *   - Committed instructions (identical — same work done)
 *   - numCycles (expect significantly fewer here)
 *
 * This is the key "aha moment" for students:
 * same program, same result, dramatically different pipeline behaviour.
 *
 * Compile:
 *   riscv64-unknown-elf-gcc -O1 -o bench_1b_ind bench_1b_independent.c
 */

#include <stdint.h>
#include <gem5/m5ops.h>
#define ITERATIONS 1000

volatile uint64_t sink;

int main(void) {
    /* Four independent chains — O3 can work on all four simultaneously */
    volatile uint64_t v0 = 1, v1 = 2, v2 = 3, v3 = 4;


    #ifdef GEM5
        m5_reset_stats(0, 0);
    #endif

    for (int i = 0; i < ITERATIONS; i++) {
        /* Chain 0 */
        v0 = v0 + 1;
        v0 = v0 * 3;

        /* Chain 1 — independent of chain 0 */
        v1 = v1 + 7;
        v1 = v1 * 2;

        /* Chain 2 — independent of chains 0 and 1 */
        v2 = v2 - 4;
        v2 = v2 + v2;

        /* Chain 3 — independent of all above */
        v3 = v3 - 1;
        v3 = v3 * 5;
    }

    #ifdef GEM5
        m5_dump_stats(0, 0);
    #endif
    sink = v0 ^ v1 ^ v2 ^ v3;
    return 0;
}
