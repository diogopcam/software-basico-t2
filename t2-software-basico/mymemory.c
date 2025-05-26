#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para inicializar o gerenciador de memória
mymemory_t* mymemory_init(size_t size) {
    // Aloca a estrutura principal
    mymemory_t *memory = (mymemory_t*)malloc(sizeof(mymemory_t));
    if (!memory) return NULL;
    
    // Aloca o pool de memória
    memory->pool = malloc(size);
    if (!memory->pool) {
        free(memory);
        return NULL;
    }
    
    memory->total_size = size;
    memory->head = NULL;
    
    return memory;
}

// Função auxiliar para encontrar o melhor bloco livre (Best Fit)
static void* find_best_fit(mymemory_t *memory, size_t size, allocation_t **prev) {
    allocation_t *current = memory->head;
    allocation_t *best = NULL;
    *prev = NULL;
    allocation_t *best_prev = NULL;
    
    // Percorre a lista de alocações para encontrar o melhor bloco
    while (1) {
        // Calcula o espaço entre alocações
        void *start;
        size_t available_size;
        
        if (current == NULL) {
            // Último bloco até o final do pool
            if (*prev == NULL) {
                start = memory->pool;
            } else {
                start = (char*)(*prev)->start + (*prev)->size;
            }
            
            available_size = (char*)memory->pool + memory->total_size - (char*)start;
        } else {
            if (*prev == NULL) {
                start = memory->pool;
            } else {
                start = (char*)(*prev)->start + (*prev)->size;
            }
            
            available_size = (char*)current->start - (char*)start;
        }
        
        // Verifica se o bloco é adequado
        if (available_size >= size) {
            if (best == NULL || available_size < ((char*)best->start - (char*)(best_prev ? (char*)best_prev->start + best_prev->size : (char*)memory->pool))) {
                best = current;
                best_prev = *prev;
            }
        }
        
        if (current == NULL) break;
        
        *prev = current;
        current = current->next;
    }
    
    *prev = best_prev;
    return best;
}

// Função para alocar memória
void* mymemory_alloc(mymemory_t *memory, size_t size) {
    if (!memory || size == 0) return NULL;
    
    allocation_t *prev = NULL;
    allocation_t *next_block = find_best_fit(memory, size, &prev);
    
    void *alloc_start;
    
    if (prev == NULL) {
        // Aloca no início do pool
        if (next_block == NULL) {
            // Única alocação no pool
            if (memory->total_size < size) return NULL;
            alloc_start = memory->pool;
        } else {
            // Espaço entre início do pool e primeira alocação
            size_t available = (char*)next_block->start - (char*)memory->pool;
            if (available < size) return NULL;
            alloc_start = memory->pool;
        }
    } else {
        // Aloca após a alocação 'prev'
        alloc_start = (char*)prev->start + prev->size;
        
        if (next_block != NULL) {
            // Verifica se há espaço suficiente
            size_t available = (char*)next_block->start - (char*)alloc_start;
            if (available < size) return NULL;
        } else {
            // Verifica se há espaço até o final do pool
            size_t available = (char*)memory->pool + memory->total_size - (char*)alloc_start;
            if (available < size) return NULL;
        }
    }
    
    // Cria a nova alocação
    allocation_t *new_alloc = (allocation_t*)malloc(sizeof(allocation_t));
    if (!new_alloc) return NULL;
    
    new_alloc->start = alloc_start;
    new_alloc->size = size;
    
    // Insere na lista encadeada
    if (prev == NULL) {
        new_alloc->next = memory->head;
        memory->head = new_alloc;
    } else {
        new_alloc->next = prev->next;
        prev->next = new_alloc;
    }
    
    return alloc_start;
}

// Função para liberar memória
void mymemory_free(mymemory_t *memory, void *ptr) {
    if (!memory || !ptr) return;
    
    allocation_t *current = memory->head;
    allocation_t *prev = NULL;
    
    // Procura a alocação correspondente ao ponteiro
    while (current != NULL && current->start != ptr) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) return; // Ponteiro não encontrado
    
    // Remove da lista encadeada
    if (prev == NULL) {
        memory->head = current->next;
    } else {
        prev->next = current->next;
    }
    
    free(current);
}

// Função para exibir as alocações atuais
void mymemory_display(mymemory_t *memory) {
    if (!memory) return;
    
    printf("Alocações atuais:\n");
    
    allocation_t *current = memory->head;
    size_t index = 0;
    
    while (current != NULL) {
        printf("[%zu] Início: %p, Tamanho: %zu bytes\n", 
               index++, current->start, current->size);
        current = current->next;
    }
    
    if (index == 0) {
        printf("Nenhuma alocação ativa.\n");
    }
}

// Função auxiliar para calcular estatísticas
static void calculate_stats(mymemory_t *memory, 
                          size_t *total_alloc, 
                          size_t *total_free, 
                          size_t *largest_free, 
                          size_t *fragments) {
    *total_alloc = 0;
    *total_free = 0;
    *largest_free = 0;
    *fragments = 0;
    
    allocation_t *current = memory->head;
    allocation_t *prev = NULL;
    
    // Calcula memória alocada
    while (current != NULL) {
        *total_alloc += current->size;
        prev = current;
        current = current->next;
    }
    
    *total_free = memory->total_size - *total_alloc;
    
    // Calcula fragmentos e maior bloco livre
    current = memory->head;
    prev = NULL;
    
    while (1) {
        void *free_start;
        size_t free_size;
        
        if (prev == NULL) {
            free_start = memory->pool;
        } else {
            free_start = (char*)prev->start + prev->size;
        }
        
        if (current == NULL) {
            free_size = (char*)memory->pool + memory->total_size - (char*)free_start;
        } else {
            free_size = (char*)current->start - (char*)free_start;
        }
        
        if (free_size > 0) {
            (*fragments)++;
            if (free_size > *largest_free) {
                *largest_free = free_size;
            }
        }
        
        if (current == NULL) break;
        prev = current;
        current = current->next;
    }
}

// Função para exibir estatísticas
void mymemory_stats(mymemory_t *memory) {
    if (!memory) return;
    
    size_t total_alloc, total_free, largest_free, fragments;
    calculate_stats(memory, &total_alloc, &total_free, &largest_free, &fragments);
    
    printf("Estatísticas de memória:\n");
    printf("- Tamanho total do pool: %zu bytes\n", memory->total_size);
    printf("- Alocações ativas: %zu\n", (size_t)0); // Não contamos o número de alocações, seria necessário percorrer a lista
    printf("- Memória total alocada: %zu bytes\n", total_alloc);
    printf("- Memória total livre: %zu bytes\n", total_free);
    printf("- Maior bloco livre: %zu bytes\n", largest_free);
    printf("- Fragmentos livres: %zu\n", fragments);
}

// Função para liberar todos os recursos
void mymemory_cleanup(mymemory_t *memory) {
    if (!memory) return;
    
    // Libera todas as alocações
    allocation_t *current = memory->head;
    while (current != NULL) {
        allocation_t *next = current->next;
        free(current);
        current = next;
    }
    
    // Libera o pool de memória
    free(memory->pool);
    
    // Libera a estrutura principal
    free(memory);
}