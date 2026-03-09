/*
 * Benchmark 2B — Unpredictable Branch (Data-Dependent)
 * ------------------------------------------------------
 * Demonstrates pipeline flushes due to branch misprediction.
 *
 * Branch outcomes are driven by a linear congruential generator (LCG)
 * whose period is long relative to any history buffer in LTAGE.
 * The predictor cannot learn the pattern and mispredicts frequently.
 *
 * Expected Konata view:
 *   Frequent pipeline squashes visible as instructions that reach
 *   decode/rename/issue but are then flushed before commit.
 *   Clear "restart" bubbles in the pipeline after each flush.
 *
 * Expected stats:
 *   branchPred.condIncorrect / condPredicted  ≈  30–45%
 *   IPC significantly lower than bench_2a despite identical work volume
 *   squashedInsts count will be high
 *
 */

#include <stdint.h>
#include <gem5/m5ops.h>

#define ITERATIONS 2000

volatile uint64_t sink;

/*
 * Linear Congruential Generator — Knuth parameters.
 * Period = 2^64, no discernible short pattern for a branch predictor.
 */
static inline uint64_t lcg_next(uint64_t state) {
    return state * 6364136223846793005ULL + 1442695040888963407ULL;
}

int main(void) {
    volatile uint64_t state = 0xDEADBEEFCAFEBABEULL;
    volatile uint64_t sum_taken    = 0;
    volatile uint64_t sum_nottaken = 0;

    #ifdef GEM5
        m5_reset_stats(0, 0);
    #endif


    for (int i = 0; i < ITERATIONS; i++) {
        state = lcg_next(state);

        /*
         * Branch depends on the MSB of the LCG output.
         * The MSB of a good LCG passes statistical randomness tests —
         * it is effectively unpredictable from the predictor's viewpoint.
         *
         * The O3 processor will speculate down one path, frequently
         * discover it was wrong, flush the pipeline, and restart.
         * Each flush costs (pipeline_depth) cycles of wasted work.
         */
        if (state & (1ULL << 63)) {
            sum_taken    += state;
            sum_taken    ^= (state >> 17);
        } else {
            sum_nottaken += state;
            sum_nottaken ^= (state >> 13);
        }
    }

    #ifdef GEM5
        m5_dump_stats(0, 0);
    #endif
    sink = sum_taken ^ sum_nottaken;
    return 0;
}
