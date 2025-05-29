#include <stdio.h>
#include <string.h>
#include "mymemory.h"

int main() {
    // --- TESTE COM BLOCOS VARIADOS ---
    const size_t POOL_SIZE = 98765;
    
    // Teste com Worst Fit
    printf("\n===== TESTE COM WORST FIT =====\n");
    mymemory_t *memory_worst = mymemory_init(POOL_SIZE, WORST_FIT);
    
    // Blocos de tamanhos variados (10 até 700)
    size_t block_sizes[] = {
        10, 25, 50, 100, 150, 200, 250, 300, 350, 400,
        450, 500, 30, 60, 90, 120, 180, 210, 270, 330,
        390, 450, 30, 55, 75, 95, 105, 135, 165, 195,
        225, 255, 285, 315, 345, 375, 405, 435, 465, 495,
        520, 540, 560, 580, 600, 620, 640, 660, 680, 700
    };

    void *blocks[50];

    printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");

    // Aloca todos os blocos iniciais
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

    // Libera blocos de índice par (0, 2, 4, ..., 48)
    for (int i = 0; i < 50; i += 2) {
        if (blocks[i]) {
            mymemory_free(memory_worst, blocks[i]);
            printf("Liberado bloco %d (%p)\n", i, blocks[i]);
        }
    }

    mymemory_display(memory_worst);
    mymemory_stats(memory_worst);

    printf("\n[TESTE - NOVAS ALOCAÇÕES APÓS LIBERAÇÃO]\n");

    // Novos tamanhos para testar Best Fit vs Worst Fit
    size_t new_block_sizes[] = {40, 80, 160, 320, 640};
    void *new_blocks[5];

    // Aloca novos blocos nos espaços liberados
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

    // --- Repete o teste com Best Fit ---
    printf("\n===== TESTE COM BEST FIT =====\n");
    mymemory_t *memory_best = mymemory_init(POOL_SIZE, BEST_FIT);

    printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");

    // Aloca os mesmos 50 blocos iniciais
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

    // Libera os mesmos blocos de índice par
    for (int i = 0; i < 50; i += 2) {
        if (blocks[i]) {
            mymemory_free(memory_best, blocks[i]);
            printf("Liberado bloco %d (%p)\n", i, blocks[i]);
        }
    }

    mymemory_display(memory_best);
    mymemory_stats(memory_best);

    printf("\n[TESTE - NOVAS ALOCAÇÕES APÓS LIBERAÇÃO]\n");

    // Aloca os mesmos novos blocos
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

    return 0;
}