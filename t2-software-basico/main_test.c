// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include "mymemory.h"

// int main() {
//     mymemory_t *memory = mymemory_init(1000);
//     if (!memory) {
//         printf("Erro ao inicializar memória.\n");
//         return 1;
//     }

//     // Blocos de tamanhos variados (10 até 1000)
//     size_t block_sizes[] = {
//         10, 25, 50, 100, 150, 200, 250, 300, 350, 400,
//         450, 500, 30, 60, 90, 120, 180, 210, 270, 330,
//         390, 450, 30, 55, 75, 95, 105, 135, 165, 195,
//         225, 255, 285, 315, 345, 375, 405, 435, 465, 495,
//         520, 540, 560, 580, 600, 620, 640, 660, 680, 700
//     };

//     void *blocks[50];

//     printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");

//     for (int i = 0; i < 50; i++) {
//         blocks[i] = mymemory_alloc(memory, block_sizes[i]);
//         if (blocks[i] != NULL) {
//             printf("Alocação %d: %zu bytes em %p\n", i, block_sizes[i], blocks[i]);
//         } else {
//             printf("Alocação %d: Falhou (%zu bytes)\n", i, block_sizes[i]);
//         }
//     }

//     mymemory_display(memory);
//     mymemory_stats(memory);

//     printf("\n[TESTE - LIBERAÇÃO DE ALGUNS BLOCOS]\n");

//     // Libera alguns blocos (ex: todos os de índice par)
//     for (int i = 0; i < 50; i += 2) {
//         if (blocks[i]) {
//             mymemory_free(memory, blocks[i]);
//             printf("Liberado bloco %d (%p)\n", i, blocks[i]);
//         }
//     }

//     mymemory_display(memory);
//     mymemory_stats(memory);

//     // Finaliza
//     mymemory_cleanup(memory);

//     return 0;
// }