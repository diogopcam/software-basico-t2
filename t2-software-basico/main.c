#include <stdio.h>
#include "mymemory.h"

int main() {
    // Inicializa o pool de memória com 1000 bytes
    mymemory_t *memory = mymemory_init(1000);
    if (!memory) {
        printf("Falha ao inicializar o gerenciador de memória.\n");
        return 1;
    }

    // Aloca um array de 32 inteiros
    int *v = (int*)mymemory_alloc(memory, sizeof(int)*32);
    if (!v) {
        printf("Falha ao alocar memória para v.\n");
        mymemory_cleanup(memory);
        return 1;
    }

    // Preenche o array
    for (int i = 0; i < 32; i++) {
        v[i] = i+1;
    }

    // Aloca outro array de 16 inteiros
    int *v2 = (int*)mymemory_alloc(memory, sizeof(int)*16);
    if (!v2) {
        printf("Falha ao alocar memória para v2.\n");
    }

    // Libera o primeiro array
    mymemory_free(memory, v);

    // Aloca uma string
    char *str = (char*)mymemory_alloc(memory, sizeof(char)*16);
    if (str) {
        sprintf(str, "ola mundo");
        printf("String alocada: %s\n", str);
    } else {
        printf("Falha ao alocar memória para str.\n");
    }

    // Exibe as alocações atuais
    mymemory_display(memory);

    // Exibe estatísticas
    mymemory_stats(memory);

    // Libera todos os recursos
    mymemory_cleanup(memory);

    return 0;
}