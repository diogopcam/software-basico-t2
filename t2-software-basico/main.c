#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

int main() {
    const size_t POOL_SIZE = 1024;
    mymemory_t *memory_first = mymemory_init(POOL_SIZE, FIRST_FIT);
    void *blocks[100] = {NULL}; // até 100 alocações possíveis
    int next_index = 0;

    int opcao;
    do {
        printf("\n===== MENU - GERENCIADOR FIRST FIT =====\n");
        printf("1. Alocar bloco\n");
        printf("2. Liberar bloco\n");
        printf("3. Exibir memória\n");
        printf("4. Exibir estatísticas\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (next_index >= 100) {
                    printf("Limite de blocos atingido.\n");
                    break;
                }

                size_t tamanho;
                printf("Digite o tamanho do bloco a ser alocado: ");
                scanf("%zu", &tamanho);

                void *ptr = mymemory_alloc(memory_first, tamanho);
                if (ptr != NULL) {
                    blocks[next_index] = ptr;
                    printf("Bloco alocado no índice %d (%zu bytes em %p)\n", next_index, tamanho, ptr);
                    next_index++;
                } else {
                    printf("Falha ao alocar bloco de %zu bytes.\n", tamanho);
                }
                break;
            }
            case 2: {
                int idx;
                printf("Digite o índice do bloco a ser liberado: ");
                scanf("%d", &idx);

                if (idx >= 0 && idx < next_index && blocks[idx] != NULL) {
                    mymemory_free(memory_first, blocks[idx]);
                    printf("Bloco no índice %d liberado (%p).\n", idx, blocks[idx]);
                    blocks[idx] = NULL;
                } else {
                    printf("Índice inválido ou bloco já foi liberado.\n");
                }
                break;
            }
            case 3:
                mymemory_display(memory_first);
                break;
            case 4:
                mymemory_stats(memory_first);
                break;
            case 5:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 5);

    mymemory_cleanup(memory_first);
    return 0;
}

// Observação: O código abaixo foi a versão que desenvolvemos inicialmente, implementando as três estratégias de alocação


// int main() {
//     const size_t POOL_SIZE = 1000;

//     printf("\n===== TESTE COM WORST FIT =====\n");
//     mymemory_t *memory_worst = mymemory_init(POOL_SIZE, WORST_FIT);
    
//     // Definição do tamanho de cada bloco
//     size_t block_sizes[] = {
//         1, 2, 3, 5, 7, 8, 10, 12, 14, 16,
//         2, 3, 2, 3, 4, 1, 1, 1, 13, 16,
//         19, 20, 2, 3, 4, 5, 5, 6, 7, 8,
//         9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
//         19, 19, 20, 20, 20, 20, 20, 20, 20, 20
//     };

//     void *blocks[50];

//     printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");
//     for (int i = 0; i < 50; i++) {
//         blocks[i] = mymemory_alloc(memory_worst, block_sizes[i]);

//         if (blocks[i] != NULL) {
//             printf("Alocação %d: %zu bytes em %p\n", i, block_sizes[i], blocks[i]);
//         } else {
//             printf("Alocação %d: Falhou (%zu bytes)\n", i, block_sizes[i]);
//         }
//     }

//     mymemory_display(memory_worst);
//     mymemory_stats(memory_worst);

//     printf("\n[TESTE - LIBERAÇÃO DE BLOCOS DEFINIDOS NA MAIN]\n");
//     int indices_para_liberar_worst[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//     int total_indices = sizeof(indices_para_liberar_worst) / sizeof(indices_para_liberar_worst[0]);

//     for (int i = 0; i < total_indices; i++) {
//         int idx = indices_para_liberar_worst[i];
//         if (blocks[idx]) {
//             mymemory_free(memory_worst, blocks[idx]);
//             printf("Liberado bloco %d (%p)\n", idx, blocks[idx]);
//             blocks[idx] = NULL;
//         } else {
//             printf("Bloco %d já está vazio ou nunca foi alocado.\n", idx);
//         }
//     }

//     mymemory_display(memory_worst);
//     mymemory_stats(memory_worst);

//     printf("\n[TESTE - NOVAS ALOCAÇÕES APÓS LIBERAÇÃO]\n");
//     size_t new_block_sizes[] = {40, 80, 75, 15, 5};
//     void *new_blocks[5];
//     for (int i = 0; i < 5; i++) {
//         new_blocks[i] = mymemory_alloc(memory_worst, new_block_sizes[i]);
//         if (new_blocks[i] != NULL) {
//             printf("Nova alocação %d: %zu bytes em %p\n", i, new_block_sizes[i], new_blocks[i]);
//         } else {
//             printf("Nova alocação %d: Falhou (%zu bytes)\n", i, new_block_sizes[i]);
//         }
//     }

//     mymemory_display(memory_worst);
//     mymemory_stats(memory_worst);
//     mymemory_cleanup(memory_worst);

//     printf("\n===== TESTE COM BEST FIT =====\n");
//     mymemory_t *memory_best = mymemory_init(POOL_SIZE, BEST_FIT);
//     // Ponteiro para uma estrutura do tipo mymemory_t, 

//     mymemory_display(memory_best);
//     mymemory_stats(memory_best);

//     printf("\n[TESTE - LIBERAÇÃO DE ALGUNS BLOCOS]\n");
//     int indices_para_liberar_best[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//     int total_indices_best = sizeof(indices_para_liberar_best) / sizeof(indices_para_liberar_best[0]);
//     for (int i = 0; i < total_indices_best; i++) {
//         int idx = indices_para_liberar_best[i];
//         if (blocks[idx]) {
//             mymemory_free(memory_best, blocks[idx]);
//             printf("Liberado bloco %d (%p)\n", idx, blocks[idx]);
//             blocks[idx] = NULL;
//         } else {
//             printf("Bloco %d já está vazio ou nunca foi alocado.\n", idx);
//         }
//     }

//     mymemory_display(memory_best);
//     mymemory_stats(memory_best);

//     printf("\n[TESTE - NOVAS ALOCAÇÕES APÓS LIBERAÇÃO]\n");
//         for (int i = 0; i < total_indices; i++) {
//         int idx = indices_para_liberar_best[i];
//         if (blocks[idx]) {
//             mymemory_free(memory_worst, blocks[idx]);
//             printf("Liberado bloco %d (%p)\n", idx, blocks[idx]);
//             blocks[idx] = NULL;
//         } else {
//             printf("Bloco %d já está vazio ou nunca foi alocado.\n", idx);
//         }
//     }

//     mymemory_display(memory_best);
//     mymemory_stats(memory_best);
//     mymemory_cleanup(memory_best);

//     printf("\n===== TESTE COM FIRST FIT =====\n");
//     mymemory_t *memory_first = mymemory_init(POOL_SIZE, FIRST_FIT);
//     // Ponteiro para uma estrutura do tipo mymemory_t,

//     printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");
//     for (int i = 0; i < 50; i++) {
//         blocks[i] = mymemory_alloc(memory_first, block_sizes[i]);
//         if (blocks[i] != NULL) {
//             printf("Alocação %d: %zu bytes em %p\n", i, block_sizes[i], blocks[i]);
//         } else {
//             printf("Alocação %d: Falhou (%zu bytes)\n", i, block_sizes[i]);
//         }
//     }

//     mymemory_display(memory_first);
//     mymemory_stats(memory_first);

//     printf("\n[TESTE - LIBERAÇÃO DE ALGUNS BLOCOS]\n");
//     int indices_para_liberar_first[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//     int total_indices_first = sizeof(indices_para_liberar_best) / sizeof(indices_para_liberar_first[0]);
//     for (int i = 0; i < total_indices_first; i++) {
//         int idx = indices_para_liberar_first[i];
//         if (blocks[idx]) {
//             mymemory_free(memory_best, blocks[idx]);
//             printf("Liberado bloco %d (%p)\n", idx, blocks[idx]);
//             blocks[idx] = NULL;
//         } else {
//             printf("Bloco %d já está vazio ou nunca foi alocado.\n", idx);
//         }
//     }

//     mymemory_display(memory_first);
//     mymemory_stats(memory_first);

//     printf("\n[TESTE - NOVAS ALOCAÇÕES APÓS LIBERAÇÃO]\n");
//     for (int i = 0; i < 5; i++) {
//         new_blocks[i] = mymemory_alloc(memory_first, new_block_sizes[i]);
//         if (new_blocks[i] != NULL) {
//             printf("Nova alocação %d: %zu bytes em %p\n", i, new_block_sizes[i], new_blocks[i]);
//         } else {
//             printf("Nova alocação %d: Falhou (%zu bytes)\n", i, new_block_sizes[i]);
//         }
//     }

//     mymemory_display(memory_first);
//     mymemory_stats(memory_first);
//     mymemory_cleanup(memory_first);

//     return 0;
// }