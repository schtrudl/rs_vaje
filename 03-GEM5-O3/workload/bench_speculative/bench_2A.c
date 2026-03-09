/*
 * Benchmark 2A — Predictable Branch (Loop Counter)
 * --------------------------------------------------
 * Demonstrates near-perfect branch prediction with LTAGE.
 *
 * A simple counted loop with a strongly-taken branch on every
 * iteration except the last. LTAGE learns this pattern after
 * a few iterations and predicts with >99% accuracy.
 *
 * Expected Konata view:
 *   Instructions flow through the pipeline continuously.
 *   Almost no pipeline flushes visible.
 *   Speculative instructions after the branch are committed correctly.
 *
 * Expected stats:
 *   branchPred.condIncorrect  ≈ 1-2  (only the final exit misprediction)
 *   branchPred.condPredicted  ≈ OUTER_ITER * INNER_ITER
 *   IPC close to fetch width (2) — front-end limited, not branch-limited
 *
 * Compile:
 *   riscv64-unknown-elf-gcc -O1 -o bench_2a_predictable bench_2a_predictable.c
 */

#include <stdint.h>
#include <gem5/m5ops.h>


#define OUTER_ITER  100
#define INNER_ITER  500

volatile uint64_t sink;

int main(void) {

    volatile uint64_t sum = 0;

    #ifdef GEM5
        m5_reset_stats(0, 0);
    #endif
    for (int i = 0; i < OUTER_ITER; i++) {
        for (int j = 0; j < INNER_ITER; j++) {
            /*
             * Branch is: "is j < INNER_ITER?"
             * Taken 499 times, not-taken once per outer iteration.
             * LTAGE saturates its counters quickly — this is the
             * "always-taken" pattern it handles best.
             */
            sum += (uint64_t)(i * j + j);
        }
    }

    #ifdef GEM5
        m5_dump_stats(0, 0);
    #endif

    sink = sum;
    return 0;
}
