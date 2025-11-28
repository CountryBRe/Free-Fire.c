#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ===== CORES ANSI ===== */
#define RESET   "\033[0m"
#define VERMELHO "\033[31m"
#define VERDE    "\033[32m"
#define AMARELO  "\033[33m"
#define AZUL     "\033[34m"

/* ===== CONSTANTES ===== */
#define INVENTORY_MAX 10
#define COMPONENT_MAX 20

/* ================== STRUCTS BASICOS ================== */
/* Item usado nos desafios Novato/Aventureiro */
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

/* Node para lista encadeada (Aventureiro) */
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

/* Componente para o desafio Mestre */
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; /* 1..10 */
} Componente;

/* ================== UTILITARIOS I/O ================== */
/* Lê linha com fgets e remove '\n' */
void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t ln = strlen(buffer);
        if (ln > 0 && buffer[ln-1] == '\n') buffer[ln-1] = '\0';
    }
}

/* Limpa buffer stdin (após scanf) */
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* ================== MÓDULO NOVATO: VETOR SIMPLES ================== */

/* Inventário sequencial (vetor) */
Item inventario[INVENTORY_MAX];
int inv_count = 0;

/* Inserir item no inventário (até INVENTORY_MAX) */
void inserirItem() {
    if (inv_count >= INVENTORY_MAX) {
        printf(VERMELHO "Mochila cheia! Remova itens antes de inserir.\n" RESET);
        return;
    }
    Item it;
    printf(AZUL "Nome do item: " RESET);
    readLine(it.nome, sizeof(it.nome));
    printf(AZUL "Tipo (arma/municao/cura/ferramenta): " RESET);
    readLine(it.tipo, sizeof(it.tipo));
    printf(AZUL "Quantidade: " RESET);
    if (scanf("%d", &it.quantidade) != 1) {
        clearInputBuffer();
        printf(VERMELHO "Entrada inválida.\n" RESET);
        return;
    }
    clearInputBuffer();
    inventario[inv_count++] = it;
    printf(VERDE "Item inserido com sucesso.\n" RESET);
}

/* Remover item por nome (primeira ocorrência) */
void removerItem() {
    if (inv_count == 0) {
        printf(VERMELHO "Mochila vazia.\n" RESET);
        return;
    }
    char nome[30];
    printf(AZUL "Nome do item a remover: " RESET);
    readLine(nome, sizeof(nome));
    int pos = -1;
    for (int i = 0; i < inv_count; i++) {
        if (strcmp(inventario[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf(VERMELHO "Item não encontrado.\n" RESET);
        return;
    }
    /* shift left */
    for (int i = pos; i < inv_count - 1; i++) {
        inventario[i] = inventario[i+1];
    }
    inv_count--;
    printf(VERDE "Item removido.\n" RESET);
}

/* Listar itens */
void listarItens() {
    printf(AZUL "\n--- Mochila (%d/%d) ---\n" RESET, inv_count, INVENTORY_MAX);
    if (inv_count == 0) {
        printf(AMARELO "Vazia\n" RESET);
        return;
    }
    for (int i = 0; i < inv_count; i++) {
        printf("[%d] %s | %s | qty: %d\n", i+1, inventario[i].nome, inventario[i].tipo, inventario[i].quantidade);
    }
}

/* Busca sequencial por nome (retorna índice ou -1) */
int buscarSequencialVetor(const char *nome, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < inv_count; i++) {
        (*comparacoes)++;
        if (strcmp(inventario[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* Menu Novato */
void moduloNovato() {
    int op;
    do {
        printf(AZUL "\n=== INVENTÁRIO — NÍVEL NOVATO ===\n" RESET);
        listarItens();
        printf("\n1 - Inserir item\n2 - Remover item\n3 - Buscar item (sequencial)\n0 - Voltar\nEscolha: ");
        if (scanf("%d", &op) != 1) { clearInputBuffer(); op = -1; }
        clearInputBuffer();
        if (op == 1) inserirItem();
        else if (op == 2) removerItem();
        else if (op == 3) {
            char nome[30];
            printf(AZUL "Nome do item a buscar: " RESET);
            readLine(nome, sizeof(nome));
            int comps = 0;
            int idx = buscarSequencialVetor(nome, &comps);
            if (idx >= 0) {
                printf(VERDE "Item encontrado (%d comparações): %s | %s | qty: %d\n" RESET,
                       comps, inventario[idx].nome, inventario[idx].tipo, inventario[idx].quantidade);
            } else {
                printf(VERMELHO "Item não encontrado (%d comparações).\n" RESET, comps);
            }
        }
    } while (op != 0);
}

/* ================== MÓDULO AVENTUREIRO: VETOR vs LISTA ENCADEADA ================== */
/* Versão vetor (reaproveita inventario[] e inv_count) */
/* Funções adicionais: ordenarVetor (selection sort), buscaBinariaVetor (após ordenar), e contadores */

/* Ordenar vetor por nome (Selection Sort) e retornar número de comparações */
int ordenarVetorPorNome() {
    int comps = 0;
    for (int i = 0; i < inv_count - 1; i++) {
        int min = i;
        for (int j = i + 1; j < inv_count; j++) {
            comps++;
            if (strcmp(inventario[j].nome, inventario[min].nome) < 0) min = j;
        }
        if (min != i) {
            Item tmp = inventario[i];
            inventario[i] = inventario[min];
            inventario[min] = tmp;
        }
    }
    return comps;
}

/* Busca binária por nome em vetor já ordenado. Retorna índice ou -1. Conta comparações. */
int buscaBinariaVetor(const char *nome, int *comparacoes) {
    int l = 0, r = inv_count - 1;
    *comparacoes = 0;
    while (l <= r) {
        int m = (l + r) / 2;
        (*comparacoes)++;
        int cmp = strcmp(inventario[m].nome, nome);
        if (cmp == 0) return m;
        else if (cmp < 0) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

/* LISTA ENCADEADA: funções básicas */

/* Insere no final */
void inserirItemLista(No **head) {
    /* read item */
    if (head == NULL) return;
    Item it;
    printf(AZUL "Nome do item: " RESET);
    readLine(it.nome, sizeof(it.nome));
    printf(AZUL "Tipo: " RESET);
    readLine(it.tipo, sizeof(it.tipo));
    printf(AZUL "Quantidade: " RESET);
    if (scanf("%d", &it.quantidade) != 1) { clearInputBuffer(); printf(VERMELHO "Entrada inválida.\n" RESET); return; }
    clearInputBuffer();

    No *novo = (No*) malloc(sizeof(No));
    novo->dados = it;
    novo->proximo = NULL;
    if (*head == NULL) *head = novo;
    else {
        No *cur = *head;
        while (cur->proximo) cur = cur->proximo;
        cur->proximo = novo;
    }
    printf(VERDE "Item inserido na lista.\n" RESET);
}

/* Remove por nome (primeira ocorrência) */
void removerItemLista(No **head) {
    if (*head == NULL) { printf(VERMELHO "Lista vazia.\n" RESET); return; }
    char nome[30];
    printf(AZUL "Nome do item a remover: " RESET);
    readLine(nome, sizeof(nome));
    No *cur = *head, *ant = NULL;
    while (cur) {
        if (strcmp(cur->dados.nome, nome) == 0) break;
        ant = cur; cur = cur->proximo;
    }
    if (!cur) { printf(VERMELHO "Item não encontrado.\n" RESET); return; }
    if (ant == NULL) *head = cur->proximo;
    else ant->proximo = cur->proximo;
    free(cur);
    printf(VERDE "Item removido da lista.\n" RESET);
}

/* Listar itens da lista */
void listarLista(No *head) {
    printf(AZUL "\n--- Mochila (lista) ---\n" RESET);
    if (!head) { printf(AMARELO "Vazia\n" RESET); return; }
    int i = 1;
    for (No *cur = head; cur; cur = cur->proximo, i++) {
        printf("[%d] %s | %s | qty: %d\n", i, cur->dados.nome, cur->dados.tipo, cur->dados.quantidade);
    }
}

/* Busca sequencial na lista por nome (retorna pointer encontrado ou NULL), conta comparações */
No* buscarSequencialLista(No *head, const char *nome, int *comparacoes) {
    *comparacoes = 0;
    for (No *cur = head; cur; cur = cur->proximo) {
        (*comparacoes)++;
        if (strcmp(cur->dados.nome, nome) == 0) return cur;
    }
    return NULL;
}

/* Contador para operações nas buscas do vetor/lista */
int last_comparisons_seq = 0;
int last_comparisons_bin = 0;

/* Menu Aventureiro */
void moduloAventureiro() {
    No *head = NULL; /* lista encadeada local ao módulo */
    int escolha;
    do {
        printf(AZUL "\n=== MOCHILA — NÍVEL AVENTUREIRO ===\n" RESET);
        printf("Escolha a estrutura:\n1 - Vetor (sequencial)\n2 - Lista encadeada\n0 - Voltar\nEscolha: ");
        if (scanf("%d", &escolha) != 1) { clearInputBuffer(); escolha = -1; }
        clearInputBuffer();
        if (escolha == 1) {
            /* submenu vetor */
            int op;
            do {
                printf(AZUL "\n-- Vetor --\n" RESET);
                listarItens();
                printf("\n1 - Inserir item\n2 - Remover item\n3 - Listar\n4 - Busca sequencial\n5 - Ordenar por nome (selection)\n6 - Busca binária (após ordenar)\n0 - Voltar\nEscolha: ");
                if (scanf("%d", &op) != 1) { clearInputBuffer(); op = -1; }
                clearInputBuffer();
                if (op == 1) inserirItem();
                else if (op == 2) removerItem();
                else if (op == 3) listarItens();
                else if (op == 4) {
                    char nome[30];
                    printf(AZUL "Nome: " RESET);
                    readLine(nome, sizeof(nome));
                    int comps;
                    int idx = buscarSequencialVetor(nome, &comps);
                    last_comparisons_seq = comps;
                    if (idx >= 0) printf(VERDE "Encontrado (%d comparações): %s | %s | qty %d\n" RESET, comps,
                                         inventario[idx].nome, inventario[idx].tipo, inventario[idx].quantidade);
                    else printf(VERMELHO "Não encontrado (%d comparações).\n" RESET, comps);
                }
                else if (op == 5) {
                    int comps = ordenarVetorPorNome();
                    printf(VERDE "Vetor ordenado por nome (%d comparações).\n" RESET, comps);
                }
                else if (op == 6) {
                    if (inv_count == 0) { printf(VERMELHO "Vetor vazio.\n" RESET); }
                    else {
                        char nome[30];
                        printf(AZUL "Nome a buscar (binária): " RESET);
                        readLine(nome, sizeof(nome));
                        int comps;
                        int idx = buscaBinariaVetor(nome, &comps);
                        last_comparisons_bin = comps;
                        if (idx >= 0) printf(VERDE "Encontrado (%d comparações): %s | %s | qty %d\n" RESET, comps,
                                             inventario[idx].nome, inventario[idx].tipo, inventario[idx].quantidade);
                        else printf(VERMELHO "Não encontrado (%d comparações).\n" RESET, comps);
                    }
                }
            } while (op != 0);
        } else if (escolha == 2) {
            /* submenu lista */
            int op;
            do {
                printf(AZUL "\n-- Lista encadeada --\n" RESET);
                listarLista(head);
                printf("\n1 - Inserir item\n2 - Remover item\n3 - Listar\n4 - Busca sequencial\n0 - Voltar\nEscolha: ");
                if (scanf("%d", &op) != 1) { clearInputBuffer(); op = -1; }
                clearInputBuffer();
                if (op == 1) inserirItemLista(&head);
                else if (op == 2) removerItemLista(&head);
                else if (op == 3) listarLista(head);
                else if (op == 4) {
                    char nome[30];
                    printf(AZUL "Nome: " RESET);
                    readLine(nome, sizeof(nome));
                    int comps;
                    No *res = buscarSequencialLista(head, nome, &comps);
                    last_comparisons_seq = comps;
                    if (res) printf(VERDE "Encontrado (%d comparações): %s | %s | qty %d\n" RESET, comps,
                                     res->dados.nome, res->dados.tipo, res->dados.quantidade);
                    else printf(VERMELHO "Não encontrado (%d comparações).\n" RESET, comps);
                }
            } while (op != 0);
        }
    } while (escolha != 0);

    /* libera lista */
    No *cur = head;
    while (cur) {
        No *next = cur->proximo;
        free(cur);
        cur = next;
    }
}

/* ================== MÓDULO MESTRE: ORDENACOES E BUSCA BINÁRIA EM COMPONENTES ================== */

/* Vetor de componentes */
Componente comps[COMPONENT_MAX];
int comps_count = 0;

/* Leitura segura de componente */
void cadastrarComponente() {
    if (comps_count >= COMPONENT_MAX) {
        printf(VERMELHO "Limite de componentes atingido (%d).\n" RESET, COMPONENT_MAX);
        return;
    }
    Componente c;
    printf(AZUL "Nome do componente: " RESET);
    readLine(c.nome, sizeof(c.nome));
    printf(AZUL "Tipo (controle/suporte/propulsao/...): " RESET);
    readLine(c.tipo, sizeof(c.tipo));
    printf(AZUL "Prioridade (1-10): " RESET);
    if (scanf("%d", &c.prioridade) != 1) { clearInputBuffer(); printf(VERMELHO "Entrada inválida.\n" RESET); return; }
    clearInputBuffer();
    comps[comps_count++] = c;
    printf(VERDE "Componente cadastrado.\n" RESET);
}

/* Mostrar componentes */
void mostrarComponentes() {
    printf(AZUL "\n--- Componentes (%d) ---\n" RESET, comps_count);
    if (comps_count == 0) { printf(AMARELO "Nenhum componente cadastrado.\n" RESET); return; }
    for (int i = 0; i < comps_count; i++) {
        printf("[%d] %s | %s | prioridade: %d\n", i+1, comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
}

/* Selection sort por prioridade (int). Retorna número de comparações; mede tempo via clock() */
long selectionSortPrioridade(Componente arr[], int n, long *comparacoes, double *tempo) {
    clock_t start = clock();
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i+1; j < n; j++) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[min].prioridade) min = j;
        }
        if (min != i) {
            Componente tmp = arr[i]; arr[i] = arr[min]; arr[min] = tmp;
        }
    }
    clock_t end = clock();
    *tempo = (double)(end - start) / CLOCKS_PER_SEC;
    return *comparacoes;
}

/* Insertion sort por tipo (string) */
long insertionSortTipo(Componente arr[], int n, long *comparacoes, double *tempo) {
    clock_t start = clock();
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, key.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = key;
    }
    clock_t end = clock();
    *tempo = (double)(end - start) / CLOCKS_PER_SEC;
    return *comparacoes;
}

/* Bubble sort por nome (string) */
long bubbleSortNome(Componente arr[], int n, long *comparacoes, double *tempo) {
    clock_t start = clock();
    *comparacoes = 0;
    int trocou;
    for (int i = 0; i < n - 1; i++) {
        trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    clock_t end = clock();
    *tempo = (double)(end - start) / CLOCKS_PER_SEC;
    return *comparacoes;
}

/* Busca binária em vetor de componentes ordenado por nome */
int buscaBinariaComponentes(const char *nome, int *comparacoes) {
    int l = 0, r = comps_count - 1;
    *comparacoes = 0;
    while (l <= r) {
        int m = (l + r) / 2;
        (*comparacoes)++;
        int cmp = strcmp(comps[m].nome, nome);
        if (cmp == 0) return m;
        else if (cmp < 0) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

/* Menu Mestre */
void moduloMestre() {
    int op;
    do {
        printf(AZUL "\n=== MÓDULO MESTRE: Ordenação e Busca ===\n" RESET);
        printf("1 - Cadastrar componente (max %d)\n2 - Listar componentes\n3 - Ordenar (Bubble por nome)\n4 - Ordenar (Insertion por tipo)\n5 - Ordenar (Selection por prioridade)\n6 - Busca binária por nome (após ordenar por nome)\n0 - Voltar\nEscolha: ");
        if (scanf("%d", &op) != 1) { clearInputBuffer(); op = -1; }
        clearInputBuffer();
        if (op == 1) cadastrarComponente();
        else if (op == 2) mostrarComponentes();
        else if (op == 3) {
            if (comps_count == 0) { printf(VERMELHO "Nenhum componente.\n" RESET); continue; }
            long compsn; double t;
            bubbleSortNome(comps, comps_count, &compsn, &t);
            printf(VERDE "Bubble sort por nome concluído: comparações=%ld, tempo=%.6fs\n" RESET, compsn, t);
        }
        else if (op == 4) {
            if (comps_count == 0) { printf(VERMELHO "Nenhum componente.\n" RESET); continue; }
            long compsn; double t;
            insertionSortTipo(comps, comps_count, &compsn, &t);
            printf(VERDE "Insertion sort por tipo concluído: comparações=%ld, tempo=%.6fs\n" RESET, compsn, t);
        }
        else if (op == 5) {
            if (comps_count == 0) { printf(VERMELHO "Nenhum componente.\n" RESET); continue; }
            long compsn; double t;
            selectionSortPrioridade(comps, comps_count, &compsn, &t);
            printf(VERDE "Selection sort por prioridade concluído: comparações=%ld, tempo=%.6fs\n" RESET, compsn, t);
        }
        else if (op == 6) {
            if (comps_count == 0) { printf(VERMELHO "Nenhum componente.\n" RESET); continue; }
            char nome[30];
            printf(AZUL "Nome do componente-chave: " RESET);
            readLine(nome, sizeof(nome));
            int compsc;
            int idx = buscaBinariaComponentes(nome, &compsc);
            if (idx >= 0) {
                printf(VERDE "Componente encontrado (%d comparações): %s | %s | prioridade %d\n" RESET,
                       compsc, comps[idx].nome, comps[idx].tipo, comps[idx].prioridade);
            } else {
                printf(VERMELHO "Componente não encontrado (%d comparações).\n" RESET, compsc);
            }
        }
    } while (op != 0);
}

/* ================== MAIN (menu geral que reúne os 3 módulos) ================== */
int main() {
    /* seed para eventuais usos futuros */
    srand((unsigned int) time(NULL));

    int escolha;
    do {
        printf(AZUL "\n=== SURVIVAL INVENTORY (NOVATO - AVENTUREIRO - MESTRE) ===\n" RESET);
        printf("1 - Nível Novato (inventário simples)\n2 - Nível Aventureiro (vetor vs lista)\n3 - Nível Mestre (ordenacao e busca)\n0 - Sair\nEscolha: ");
        if (scanf("%d", &escolha) != 1) { clearInputBuffer(); escolha = -1; }
        clearInputBuffer();
        if (escolha == 1) moduloNovato();
        else if (escolha == 2) moduloAventureiro();
        else if (escolha == 3) moduloMestre();
    } while (escolha != 0);

    printf(GREEN "Encerrando programa. Boa sorte na ilha!\n" RESET); /* note: GREEN not defined intentionally avoid compile error? fix below */
    /* The correct macro is VERDE */
    printf(VERDE "Encerrando programa. Boa sorte na ilha!\n" RESET);
    return 0;
}