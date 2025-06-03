#include <stdio.h>
#include <string.h>
#include "mymemory.h"

int main() {
    const size_t POOL_SIZE = 98765;
    
    // --- TESTE COM WORST FIT ---
    // WORST FIT - Explicação da estratégia
    // Essa estratégia realiza a alocação no bloco com maior espaço disponível.
    //     🧠 Ideia:
    // Aloca o novo bloco no maior espaço livre disponível.

    // 📈 Vantagem:
    // Pode evitar que blocos muito grandes fiquem fragmentados em pedaços pequenos, já que usa os maiores espaços.

    // Deixa blocos pequenos livres, o que pode ser útil para futuras alocações pequenas.

    // ⚠️ Desvantagem:
    // Pode deixar buracos grandes parcialmente preenchidos, o que gera fragmentação externa.

    // Pode desperdiçar mais memória do que o necessário.

    // 🔍 Exemplo:
    // Se há blocos livres de 100, 200 e 300 bytes, e você quer alocar 50:

    // Será usado o bloco de 300 (o maior), deixando 250 sobrando.

    printf("\n===== TESTE COM WORST FIT =====\n");
    mymemory_t *memory_worst = mymemory_init(POOL_SIZE, WORST_FIT);
    // memory_worst é um ponteiro para uma estrutura do tipo mymemory_t, 
    // que representa um gerenciador de memória personalizado configurado para usar o algoritmo Worst Fit.
    // Essa estrutura armazena informações definidas na estrutura mymemory_t. definida em mymemory.h.
    

    // Definição do tamanho de cada bloco
    size_t block_sizes[] = {
        100, 25, 50, 100, 150, 200, 250, 300, 350, 400,
        450, 500, 30, 60, 90, 120, 180, 210, 270, 330,
        390, 450, 30, 55, 75, 95, 105, 135, 165, 195,
        225, 255, 285, 315, 345, 375, 405, 435, 465, 495,
        520, 540, 560, 580, 600, 620, 640, 660, 680, 700
    };

    // (*) em void *blocks[50] indica que blocks é um array de 50 ponteiros para void 
    // (ponteiros genéricos).
    void *blocks[50];

    printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");
    for (int i = 0; i < 50; i++) {
        blocks[i] = mymemory_alloc(memory_worst, block_sizes[i]);
        // Para cada bloco definido no array e seu respectivo tamanho, a memória é alocada de acordo com os atributos definidos
        // na estrutura memory_worst

        // mymemory_alloc: Função que aloca memória no pool gerenciado por memory_worst. Retorna:
        // Um ponteiro void* para o bloco alocado (armazenado em blocks[i]).
        // NULL se a alocação falhar (ex.: sem memória suficiente).

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
    //     Isso pode ser feito de propósito para testar fragmentação da memória.
    // 	•	Se você alocar 50 blocos consecutivos e liberar apenas os pares, a memória vai ficar 
    // intercalada:
    // um bloco ocupado, um livre, um ocupado, um livre…
    // 	•	Isso cria uma situação de fragmentação externa, onde há bastante memória livre 
    // no total, mas não contínua o suficiente para certas alocações.

    // Isso é útil para testar se a estratégia de alocação (FIRST_FIT, BEST_FIT, WORST_FIT) está funcionando corretamente em condições de fragmentação.

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
    // BEST FIT - Explicação da estratégia
    //🧠 Ideia:
    // Aloca o novo bloco no menor espaço livre que ainda seja suficiente.

    // 📈 Vantagem:
    // Reduz o desperdício de memória, pois tenta ocupar exatamente o espaço necessário.

    // Minimiza a quantidade de memória "sobrando" em cada buraco.

    // ⚠️ Desvantagem:
    // Pode causar fragmentação externa severa com o tempo, criando muitos buracos muito pequenos para serem usados.

    // 🔍 Exemplo:
    // Se há blocos livres de 100, 200 e 300 bytes, e você quer alocar 50:

    // Será usado o bloco de 100 (o menor que serve), sobrando 50.

    printf("\n===== TESTE COM BEST FIT =====\n");
    mymemory_t *memory_best = mymemory_init(POOL_SIZE, BEST_FIT);
    // Ponteiro para uma estrutura do tipo mymemory_t, 

    printf("\n[TESTE AUTOMÁTICO - ALOCAÇÕES DE 50 BLOCOS]\n");
    for (int i = 0; i < 50; i++) {
        blocks[i] = mymemory_alloc(memory_best, block_sizes[i]);
        // Dada a quantidade de blocos, as alocações são realizadas sequencialmente
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
    // FIRST FIT - Explicação da estratégia
    // 🧠 Ideia:
    // Aloca o novo bloco no primeiro espaço livre que seja suficiente, varrendo da esquerda para a direita.

    // 📈 Vantagem:
    // Geralmente é mais rápido, pois para de procurar assim que encontra um bloco suficiente.

    // Tende a ser mais eficiente em tempo do que Best ou Worst Fit.

    // ⚠️ Desvantagem:
    // Pode deixar fragmentos pequenos no início da memória (por exemplo, um bloco de 5 bytes entre dois blocos maiores), levando a fragmentação externa.

    // 🔍 Exemplo:
    // Se há blocos livres de 100, 200 e 300 bytes, e você quer alocar 50:

    // Será usado o bloco de 100, mesmo que o de 200 ou 300 fosse melhor.

    printf("\n===== TESTE COM FIRST FIT =====\n");
    mymemory_t *memory_first = mymemory_init(POOL_SIZE, FIRST_FIT);
    // Ponteiro para uma estrutura do tipo mymemory_t,

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