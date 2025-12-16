// test_allocator.c
// Build (example):
//   cc -std=c11 -O1 -Wall -Wextra -Werror test_allocator.c allocator.c -o test_allocator
//
// Assumes you have a header that exposes:
//   - void *ft_alloc(size_t bytes);
//   - void  ft_free(void *ptr);
//   - BLOCK_SIZE, MEMORY_SIZE macros (or equivalent)

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ft_alloc.h"

#define RUN(t) do { t(); printf("[OK] %s\n", #t); } while (0)

void	ft_free(void *ptr);
void	*ft_alloc(size_t bytes);

static inline uintptr_t uptr(void *p) { return (uintptr_t)p; }

// Alloc n allocations of 1 byte; returns base pointer (first allocation)
static void *alloc_n_1b(size_t n, void **out)
{
    for (size_t i = 0; i < n; i++) {
        out[i] = ft_alloc(1);
        assert(out[i] != NULL);
    }
    return out[0];
}

static void free_all(void **ptrs, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        if (ptrs[i]) ft_free(ptrs[i]);
        ptrs[i] = NULL;
    }
}

static void test_alloc_zero_and_too_big(void)
{
    assert(ft_alloc(0) == NULL);
    assert(ft_alloc(MEMORY_SIZE) == NULL); // matches your current guard (bytes >= MEMORY_SIZE)
}

static void test_basic_spacing_and_reuse_first_fit(void)
{
    void *p[3] = {0};

    void *base = alloc_n_1b(3, p);

    // When fresh, allocator should hand out contiguous blocks in order.
    assert(uptr(p[1]) - uptr(base) == (uintptr_t)BLOCK_SIZE);
    assert(uptr(p[2]) - uptr(base) == (uintptr_t)2 * (uintptr_t)BLOCK_SIZE);

    // Free all; next alloc should reuse the first block (first-fit).
    free_all(p, 3);
    void *q = ft_alloc(1);
    assert(q == base);
    ft_free(q);
}

static void test_multi_block_allocation_layout(void)
{
    void *a = ft_alloc(BLOCK_SIZE + 1); // 2 blocks
    assert(a != NULL);

    void *b = ft_alloc(1); // should come immediately after 2 blocks when pool is fresh
    assert(b != NULL);
    assert(uptr(b) - uptr(a) == (uintptr_t)2 * (uintptr_t)BLOCK_SIZE);

    ft_free(a);
    void *c = ft_alloc(BLOCK_SIZE + 7); // 2 blocks again; should reuse a's spot (first-fit)
    assert(c == a);

    ft_free(b);
    ft_free(c);
}

static void test_fragmentation_first_fit_behavior(void)
{
    // A=2 blocks, B=2 blocks, C=2 blocks
    void *A = ft_alloc(BLOCK_SIZE + 1);
    void *B = ft_alloc(BLOCK_SIZE + 1);
    void *C = ft_alloc(BLOCK_SIZE + 1);
    assert(A && B && C);

    // Free middle chunk B
    ft_free(B);

    // Allocate 1 block: should land at start of B (first-fit)
    void *D = ft_alloc(1);
    assert(D == B);

    // Now only 1 block remains free inside old B region; ask for 2 blocks => must go after C
    void *E = ft_alloc(BLOCK_SIZE + 1);
    assert(E != NULL);
    assert(uptr(E) >= uptr(C) + (uintptr_t)2 * (uintptr_t)BLOCK_SIZE);

    ft_free(A);
    ft_free(C);
    ft_free(D);
    ft_free(E);
}

static void test_fill_pool_in_two_steps_and_reuse_tail(void)
{
    const size_t total_blocks = MEMORY_SIZE / BLOCK_SIZE;

    // Your ft_alloc disallows bytes >= MEMORY_SIZE, so allocate all-but-1 block first.
    void *big = ft_alloc(MEMORY_SIZE - BLOCK_SIZE);
    assert(big != NULL);

    // Allocate final block
    void *tail = ft_alloc(1);
    assert(tail != NULL);
    assert(uptr(tail) - uptr(big) == (uintptr_t)(total_blocks - 1) * (uintptr_t)BLOCK_SIZE);

    // Pool should now be full
    assert(ft_alloc(1) == NULL);

    // Free tail; should be allocatable again
    ft_free(tail);
    void *tail2 = ft_alloc(1);
    assert(tail2 == tail);
    ft_free(tail2);

    // Free big; should be allocatable again
    ft_free(big);
    void *big2 = ft_alloc(MEMORY_SIZE - BLOCK_SIZE);
    assert(big2 == big);
    ft_free(big2);
}

static uint32_t xorshift32(uint32_t *s)
{
    uint32_t x = *s;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *s = x;
    return x;
}

static void test_stress_alloc_free_valid_pointers(void)
{
    enum { MAX_PTRS = 4096 };
    void *ptrs[MAX_PTRS] = {0};
    size_t n = 0;

    // Fill with random sizes (1..5 blocks), until allocation fails.
    uint32_t seed = 0xC0FFEEu;
    while (n < MAX_PTRS) {
        size_t blocks = (xorshift32(&seed) % 5u) + 1u;
        size_t bytes = blocks * BLOCK_SIZE;
        if (bytes >= MEMORY_SIZE) bytes = MEMORY_SIZE - 1; // keep it valid for your guard
        void *p = ft_alloc(bytes);
        if (!p) break;
        ptrs[n++] = p;
    }
    assert(n > 0);

    // Shuffle pointers then free all (still all valid starts).
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = xorshift32(&seed) % (i + 1);
        void *tmp = ptrs[i];
        ptrs[i] = ptrs[j];
        ptrs[j] = tmp;
    }
    for (size_t i = 0; i < n; i++) ft_free(ptrs[i]);

    // After freeing everything, allocator should behave like fresh (first alloc gives lowest block).
    void *p0 = ft_alloc(1);
    void *p1 = ft_alloc(1);
    assert(p0 && p1);
    assert(uptr(p1) - uptr(p0) == (uintptr_t)BLOCK_SIZE);
    ft_free(p0);
    ft_free(p1);
}

int main(void)
{
    RUN(test_alloc_zero_and_too_big);
    RUN(test_basic_spacing_and_reuse_first_fit);
    RUN(test_multi_block_allocation_layout);
    RUN(test_fragmentation_first_fit_behavior);
    RUN(test_fill_pool_in_two_steps_and_reuse_tail);
    RUN(test_stress_alloc_free_valid_pointers);

    puts("All tests passed.");
    return 0;
}
