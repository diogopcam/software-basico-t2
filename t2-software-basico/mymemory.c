#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t align_size(size_t size) {
    const size_t ALIGNMENT = 8; // Alinhamento de 8 bytes
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
} 

mymemory_t* mymemory_init(size_t size, Strategy strategy) {
    mymemory_t *memory = (mymemory_t*)malloc(sizeof(mymemory_t));
    if (!memory) return NULL;
    
    memory->pool = malloc(size);
    if (!memory->pool) {
        free(memory);
        return NULL;
    }
    
    memory->total_size = size;
    memory->head = NULL;
    memory->strategy = strategy;
    return memory;
}

// Função que implementa a estratégia First Fit para alocação de memória
static void* find_first_fit(mymemory_t *memory, size_t size, allocation_t **prev) {
    // Começa pelo primeiro bloco alocado na memória
    allocation_t *current = memory->head;
    // Inicializa o ponteiro para o bloco anterior como NULL
    *prev = NULL;
    
    // Define os limites do pool de memória
    void *pool_start = memory->pool;
    void *pool_end = (char*)memory->pool + memory->total_size;

    // Verifica se há espaço entre o início do pool e o primeiro bloco alocado
    if (current && (char*)current->start - (char*)pool_start >= (ptrdiff_t)size) {
        // Se houver espaço suficiente antes do primeiro bloco, retorna o primeiro bloco
        // (indicando que o espaço livre está antes dele)
        return current;
    }

    // Percorre a lista de blocos alocados
    while (current) {
        void *block_end = (char*)current->start + current->size;
        allocation_t *next = current->next;
        void *next_start = next ? next->start : pool_end;
        
        // Verifica se o espaço entre o bloco atual e o próximo é suficiente
        if ((char*)next_start - (char*)block_end >= (ptrdiff_t)size) {
            // Se for, armazena o bloco atual como 'prev' e retorna o próximo bloco
            *prev = current;
            return next;
        }
        
        current = next;
    }

    if (!memory->head) {
         // Se não há blocos alocados, verifica se o pool inteiro está livre
        if (memory->total_size >= size) {
            return NULL;
        }
    } else {
         // Se houver blocos alocados, verifica o espaço após o último bloco
        allocation_t *last = memory->head;
        while (last->next) last = last->next;
        // Calcula o fim do último bloco
        void *last_end = (char*)last->start + last->size;
        // Verifica se há espaço após o último bloco
        if ((char*)pool_end - (char*)last_end >= (ptrdiff_t)size) {
            // Se houver, armazena o último bloco como 'prev' e retorna NULL
            // (indicando que o espaço livre está após o último bloco)
            *prev = last;
            return NULL;
        }
    }

    return NULL;
}

// Realiza a alocacao na menor lacuna suficientemente grande
static void* find_best_fit(mymemory_t *memory, size_t size, allocation_t **prev) {
    allocation_t *current = memory->head;
    *prev = NULL;
    void *best_block = NULL;
    allocation_t *best_prev = NULL;
    size_t best_size = memory->total_size + 1;

    void *pool_start = memory->pool;
    void *pool_end = (char*)memory->pool + memory->total_size;

    if (current) {
        size_t available = (char*)current->start - (char*)pool_start;
        if (available >= size && available < best_size) {
            best_size = available;
            best_block = current;
            best_prev = NULL;
        }
    }

    while (current) {
        void *block_end = (char*)current->start + current->size;
        allocation_t *next = current->next;
        void *next_start = next ? next->start : pool_end;
        size_t available = (char*)next_start - (char*)block_end;

        if (available >= size && available < best_size) {
            best_size = available;
            best_block = next;
            best_prev = current;
        }

        current = next;
    }

    if (memory->head) {
        allocation_t *last = memory->head;
        while (last->next) last = last->next;
        void *last_end = (char*)last->start + last->size;
        size_t available = (char*)pool_end - (char*)last_end;

        if (available >= size && available < best_size) {
            best_size = available;
            best_block = NULL;
            best_prev = last;
        }
    } else {
        if (memory->total_size >= size && memory->total_size < best_size) {
            best_size = memory->total_size;
            best_block = NULL;
            best_prev = NULL;
        }
    }

    *prev = best_prev;
    return best_block;
}

static void* find_worst_fit(mymemory_t *memory, size_t size, allocation_t **prev) {
    allocation_t *current = memory->head;
    *prev = NULL;
    void *worst_block = NULL;
    allocation_t *worst_prev = NULL;
    size_t worst_size = 0;

    void *pool_start = memory->pool;
    void *pool_end = (char*)memory->pool + memory->total_size;

    if (current) {
        size_t available = (char*)current->start - (char*)pool_start;
        if (available >= size && available > worst_size) {
            worst_size = available;
            worst_block = current;
            worst_prev = NULL;
        }
    }

    while (current) {
        void *block_end = (char*)current->start + current->size;
        allocation_t *next = current->next;
        void *next_start = next ? next->start : pool_end;
        size_t available = (char*)next_start - (char*)block_end;

        if (available >= size && available > worst_size) {
            worst_size = available;
            worst_block = next;
            worst_prev = current;
        }

        current = next;
    }

    if (memory->head) {
        allocation_t *last = memory->head;
        while (last->next) last = last->next;
        void *last_end = (char*)last->start + last->size;
        size_t available = (char*)pool_end - (char*)last_end;

        if (available >= size && available > worst_size) {
            worst_size = available;
            worst_block = NULL;
            worst_prev = last;
        }
    } else {
        if (memory->total_size >= size) {
            worst_block = NULL;
            worst_prev = NULL;
        }
    }

    *prev = worst_prev;
    return worst_block;
}

void* mymemory_alloc(mymemory_t *memory, size_t size) {
    if (!memory || size == 0) return NULL;

    size_t aligned_size = align_size(size);

    allocation_t *prev = NULL;
    allocation_t *next_block = NULL;

    switch (memory->strategy) {
        case FIRST_FIT:
            next_block = find_first_fit(memory, size, &prev);
            break;
        case BEST_FIT:
            next_block = find_best_fit(memory, size, &prev);
            break;
        case WORST_FIT:
            next_block = find_worst_fit(memory, size, &prev);
            break;
    }

    void *alloc_start = NULL;
    if (prev == NULL) {
        if (next_block == NULL) {
            // Aloca no início do pool
            if (memory->total_size < size) return NULL;
            alloc_start = memory->pool;
            printf("Alocando %zu bytes em %p\n", size, alloc_start);
        } else {
            // Aloca antes do primeiro bloco
            if ((char*)next_block->start - (char*)memory->pool < (ptrdiff_t)size)
                return NULL;
            alloc_start = memory->pool;
            printf("Alocando %zu bytes em %p\n", size, alloc_start);
        }
    } else {
        alloc_start = (char*)prev->start + prev->size;
        if (next_block) {
            if ((char*)next_block->start - (char*)alloc_start < (ptrdiff_t)size)
                return NULL;
        } else {
            if ((char*)memory->pool + memory->total_size - (char*)alloc_start < (ptrdiff_t)size)
                return NULL;
        }
    }

    allocation_t *new_alloc = (allocation_t*)malloc(sizeof(allocation_t));
    if (!new_alloc) return NULL;

    new_alloc->start = alloc_start;
    new_alloc->size = size;
    
    if (prev == NULL) {
        new_alloc->next = memory->head; 
        memory->head = new_alloc;
    } else {
        new_alloc->next = prev->next;
        prev->next = new_alloc;
    }

    return alloc_start;
}

void mymemory_free(mymemory_t *memory, void *ptr) {
    if (!memory || !ptr) return;

    allocation_t *current = memory->head;
    allocation_t *prev = NULL;

    while (current && current->start != ptr) {
        prev = current;
        current = current->next;
    }

    if (!current) return;

    if (prev) {
        prev->next = current->next;
    } else {
        memory->head = current->next;
    }

    free(current);
}

void mymemory_display(mymemory_t *memory) {
    if (!memory) return;

    printf("Alocações atuais:\n");
    allocation_t *current = memory->head;
    size_t index = 0;

    while (current) {
        printf("[%zu] Início: %p, Tamanho: %zu bytes\n", 
               index++, current->start, current->size);
        current = current->next;
    }

    if (index == 0) {
        printf("Nenhuma alocação ativa.\n");
    }
}

static void calculate_stats(mymemory_t *memory, 
                          size_t *total_alloc, 
                          size_t *total_free, 
                          size_t *largest_free, 
                          size_t *fragments) {
    *total_alloc = 0;
    *total_free = memory->total_size;
    *largest_free = 0;
    *fragments = 0;

    allocation_t *current = memory->head;
    void *prev_end = memory->pool;

    while (current) {
        *total_alloc += current->size;
        
        // Espaço livre antes da alocação atual
        if ((char*)current->start > (char*)prev_end) {
            size_t free_size = (char*)current->start - (char*)prev_end;
            (*fragments)++;
            if (free_size > *largest_free) {
                *largest_free = free_size;
            }
        }
        
        prev_end = (char*)current->start + current->size;
        current = current->next;
    }

    // Espaço livre final
    if ((char*)prev_end < (char*)memory->pool + memory->total_size) {
        size_t free_size = (char*)memory->pool + memory->total_size - (char*)prev_end;
        (*fragments)++;
        if (free_size > *largest_free) {
            *largest_free = free_size;
        }
    }

    *total_free = memory->total_size - *total_alloc;
}

void mymemory_stats(mymemory_t *memory) {
    if (!memory) return;

    size_t total_alloc, total_free, largest_free, fragments;
    calculate_stats(memory, &total_alloc, &total_free, &largest_free, &fragments);

    // Contagem de alocações ativas
    size_t active_allocs = 0;
    allocation_t *current = memory->head;
    while (current) {
        active_allocs++;
        current = current->next;
    }

    printf("Estatísticas de memória:\n");
    printf("- Tamanho total do pool: %zu bytes\n", memory->total_size); // Correto
    printf("- Alocações ativas: %zu\n", active_allocs); 
    printf("- Memória total alocada: %zu bytes\n", total_alloc); // Correto
    printf("- Memória total livre: %zu bytes\n", total_free); // Correto
    printf("- Maior bloco livre: %zu bytes\n", largest_free);
    printf("- Fragmentos livres: %zu\n", fragments); 
}

void mymemory_cleanup(mymemory_t *memory) {
    if (!memory) return;

    allocation_t *current = memory->head;
    while (current) {
        allocation_t *next = current->next;
        free(current);
        current = next;
    }

    free(memory->pool);
    free(memory);
}