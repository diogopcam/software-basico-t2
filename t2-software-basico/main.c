#include <stdio.h>
#include <string.h>
#include "mymemory.h"

int main() {
    const size_t POOL_SIZE = 98765;
    
    // --- TESTE COM WORST FIT ---
    printf("\n===== TESTE COM WORST FIT =====\n");
    mymemory_t *memory_worst = mymemory_init(POOL_SIZE, WORST_FIT);
    
    size_t block_sizes[] = {
        10, 25, 50, 100, 150, 200, 250, 300, 350, 400,
        450, 500, 30, 60, 90, 120, 180, 210, 270, 330,
        390, 450, 30, 55, 75, 95, 105, 135, 165, 195,
        225, 255, 285, 315, 345, 375, 405, 435, 465, 495,
        520, 540, 560, 580, 600, 620, 640, 660, 680, 700
    };

    void *blocks[50];

    printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");
    for (int i = 0; i < 50; i++) {
        blocks[i] = mymemory_alloc(memory_worst, block_sizes[i]);
        if (blocks[i] != NULL) {
            printf("Alocação %d: %zu bytes em %p\n", i, block_sizes[i], blocks[i]);
        } else {
            printf("Alocação %d: Falhou (%zu bytes)\n", i, block_sizes[i]);
        }
    }

    mymemory_display(memory_worst);
    mymemory_stats(memory_worst);

    printf("\n[TESTE - LIBERAÇÃO DE ALGUNS BLOCOS]\n");
    for (int i = 0; i < 50; i += 2) {
        if (blocks[i]) {
            mymemory_free(memory_worst, blocks[i]);
            printf("Liberado bloco %d (%p)\n", i, blocks[i]);
        }
    }

    mymemory_display(memory_worst);
    mymemory_stats(memory_worst);

    printf("\n[TESTE - NOVAS ALOCAÇÕES APÓS LIBERAÇÃO]\n");
    size_t new_block_sizes[] = {40, 80, 160, 320, 640};
    void *new_blocks[5];
    for (int i = 0; i < 5; i++) {
        new_blocks[i] = mymemory_alloc(memory_worst, new_block_sizes[i]);
        if (new_blocks[i] != NULL) {
            printf("Nova alocação %d: %zu bytes em %p\n", i, new_block_sizes[i], new_blocks[i]);
        } else {
            printf("Nova alocação %d: Falhou (%zu bytes)\n", i, new_block_sizes[i]);
        }
    }

    mymemory_display(memory_worst);
    mymemory_stats(memory_worst);
    mymemory_cleanup(memory_worst);

    // --- TESTE COM BEST FIT ---
    printf("\n===== TESTE COM BEST FIT =====\n");
    mymemory_t *memory_best = mymemory_init(POOL_SIZE, BEST_FIT);

    printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");
    for (int i = 0; i < 50; i++) {
        blocks[i] = mymemory_alloc(memory_best, block_sizes[i]);
        if (blocks[i] != NULL) {
            printf("Alocação %d: %zu bytes em %p\n", i, block_sizes[i], blocks[i]);
        } else {
            printf("Alocação %d: Falhou (%zu bytes)\n", i, block_sizes[i]);
        }
    }

    mymemory_display(memory_best);
    mymemory_stats(memory_best);

    printf("\n[TESTE - LIBERAÇÃO DE ALGUNS BLOCOS]\n");
    for (int i = 0; i < 50; i += 2) {
        if (blocks[i]) {
            mymemory_free(memory_best, blocks[i]);
            printf("Liberado bloco %d (%p)\n", i, blocks[i]);
        }
    }

    mymemory_display(memory_best);
    mymemory_stats(memory_best);

    printf("\n[TESTE - NOVAS ALOCAÇÕES APÓS LIBERAÇÃO]\n");
    for (int i = 0; i < 5; i++) {
        new_blocks[i] = mymemory_alloc(memory_best, new_block_sizes[i]);
        if (new_blocks[i] != NULL) {
            printf("Nova alocação %d: %zu bytes em %p\n", i, new_block_sizes[i], new_blocks[i]);
        } else {
            printf("Nova alocação %d: Falhou (%zu bytes)\n", i, new_block_sizes[i]);
        }
    }

    mymemory_display(memory_best);
    mymemory_stats(memory_best);
    mymemory_cleanup(memory_best);

    // --- TESTE COM FIRST FIT ---
    printf("\n===== TESTE COM FIRST FIT =====\n");
    mymemory_t *memory_first = mymemory_init(POOL_SIZE, FIRST_FIT);

    printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");
    for (int i = 0; i < 50; i++) {
        blocks[i] = mymemory_alloc(memory_first, block_sizes[i]);
        if (blocks[i] != NULL) {
            printf("Alocação %d: %zu bytes em %p\n", i, block_sizes[i], blocks[i]);
        } else {
            printf("Alocação %d: Falhou (%zu bytes)\n", i, block_sizes[i]);
        }
    }

    mymemory_display(memory_first);
    mymemory_stats(memory_first);

    printf("\n[TESTE - LIBERAÇÃO DE ALGUNS BLOCOS]\n");
    for (int i = 0; i < 50; i += 2) {
        if (blocks[i]) {
            mymemory_free(memory_first, blocks[i]);
            printf("Liberado bloco %d (%p)\n", i, blocks[i]);
        }
    }

    mymemory_display(memory_first);
    mymemory_stats(memory_first);

    printf("\n[TESTE - NOVAS ALOCAÇÕES APÓS LIBERAÇÃO]\n");
    for (int i = 0; i < 5; i++) {
        new_blocks[i] = mymemory_alloc(memory_first, new_block_sizes[i]);
        if (new_blocks[i] != NULL) {
            printf("Nova alocação %d: %zu bytes em %p\n", i, new_block_sizes[i], new_blocks[i]);
        } else {
            printf("Nova alocação %d: Falhou (%zu bytes)\n", i, new_block_sizes[i]);
        }
    }

    mymemory_display(memory_first);
    mymemory_stats(memory_first);
    mymemory_cleanup(memory_first);

    return 0;
}