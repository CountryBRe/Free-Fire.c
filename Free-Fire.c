#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ===== CORES ANSI ===== */
#define RESET     "\033[0m"
#define VERMELHO  "\033[31m"
#define VERDE     "\033[32m"
#define AMARELO   "\033[33m"
#define AZUL      "\033[34m"
#define GREEN     VERDE

/* ===== CONSTANTES ===== */
#define INVENTARIO_MAX 10
#define COMPONENTE_MAX 20

/* ================== ESTRUTURAS ================== */
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

/* ================== UTILITÁRIOS ================== */
void lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
    }
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* ================== INVENTÁRIO NOVATO ================== */
Item inventario[INVENTARIO_MAX];
int total_itens = 0;

void listarInventario() {
    printf(AZUL "\n--- MOCHILA (%d/%d) ---\n" RESET, total_itens, INVENTARIO_MAX);
    if (total_itens == 0) {
        printf(AMARELO "Mochila vazia\n" RESET);
        return;
    }
    for (int i = 0; i < total_itens; i++) {
        printf("[%d] %s | %s | Quantidade: %d\n",
               i + 1, inventario[i].nome, inventario[i].tipo, inventario[i].quantidade);
    }
}

void inserirItem() {
    if (total_itens >= INVENTARIO_MAX) {
        printf(VERMELHO "Mochila cheia! Remova algo primeiro.\n" RESET);
        return;
    }

    Item item;
    printf(AZUL "Nome do item: " RESET);
    lerLinha(item.nome, sizeof(item.nome));
    printf(AZUL "Tipo (arma/municao/cura/ferramenta): " RESET);
    lerLinha(item.tipo, sizeof(item.tipo));
    printf(AZUL "Quantidade: " RESET);

    if (scanf("%d", &item.quantidade) != 1) {
        limparBuffer();
        printf(VERMELHO "Entrada inválida.\n" RESET);
        return;
    }
    limparBuffer();

    inventario[total_itens++] = item;
    printf(VERDE "Item adicionado!\n" RESET);
}

void removerItem() {
    if (total_itens == 0) {
        printf(VERMELHO "Mochila vazia.\n" RESET);
        return;
    }

    char nome[30];
    printf(AZUL "Nome do item para remover: " RESET);
    lerLinha(nome, sizeof(nome));

    int posicao = -1;
    for (int i = 0; i < total_itens; i++) {
        if (strcmp(inventario[i].nome, nome) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf(VERMELHO "Item não encontrado.\n" RESET);
        return;
    }

    for (int i = posicao; i < total_itens - 1; i++)
        inventario[i] = inventario[i + 1];

    total_itens--;
    printf(VERDE "Item removido!\n" RESET);
}

int buscarItemSequencial(const char *nome, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < total_itens; i++) {
        (*comparacoes)++;
        if (strcmp(inventario[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

void moduloNovato() {
    int opcao;
    do {
        printf(AZUL "\n=== INVENTÁRIO DO SOBREVIVENTE - NOVATO ===\n" RESET);
        listarInventario();

        printf("\n1 - Inserir item\n2 - Remover item\n3 - Buscar item\n0 - Voltar\nEscolha: ");
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
        }
        limparBuffer();

        if (opcao == 1) inserirItem();
        else if (opcao == 2) removerItem();
        else if (opcao == 3) {
            char nome[30];
            printf(AZUL "Nome do item: " RESET);
            lerLinha(nome, sizeof(nome));
            int comp;
            int pos = buscarItemSequencial(nome, &comp);
            if (pos >= 0) {
                printf(VERDE "Encontrado após %d comparações: %s | %s\n" RESET,
                       comp, inventario[pos].nome, inventario[pos].tipo);
            } else {
                printf(VERMELHO "Item não encontrado (%d comparações).\n" RESET, comp);
            }
        }

    } while (opcao != 0);
}

/* ================== PROGRAMA PRINCIPAL ================== */
int main() {
    srand(time(NULL));
    int escolha;

    do {
        printf(AZUL "\n=== INVENTÁRIO DO SOBREVIVENTE ===\n" RESET);
        printf("1 - Nível Novato\n2 - Nível Aventureiro\n3 - Nível Mestre\n0 - Sair\nEscolha: ");

        if (scanf("%d", &escolha) != 1) {
            limparBuffer();
            escolha = -1;
        }
        limparBuffer();

        if (escolha == 1) moduloNovato();
        else if (escolha == 2)
            printf(AMARELO "\nNível Aventureiro ainda não ativado.\n" RESET);
        else if (escolha == 3)
            printf(AMARELO "\nNível Mestre ainda não ativado.\n" RESET);

    } while (escolha != 0);

    printf(VERDE "\nEncerrando jogo. Boa sorte sobrevivente!\n" RESET);
    return 0;
}
