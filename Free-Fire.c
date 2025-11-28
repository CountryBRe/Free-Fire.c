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
#define INVENTARIO_MAX 10
#define COMPONENTES_MAX 20

/* ================= STRUCTS ================= */

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

/* ================= PROTÓTIPOS ================= */
void moduloNovato();
void moduloAventureiro();
void moduloMestre();

/* ================= UTILITÁRIOS ================= */

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerLinha(char *texto, int tamanho) {
    fgets(texto, tamanho, stdin);
    texto[strcspn(texto, "\n")] = '\0';
}

/* ================= INVENTÁRIO NOVATO ================= */

Item inventario[INVENTARIO_MAX];
int totalItens = 0;

void listarInventario() {
    printf(AZUL "\n--- MOCHILA (%d/%d) ---\n" RESET, totalItens, INVENTARIO_MAX);

    if (totalItens == 0) {
        printf(AMARELO "Mochila vazia\n" RESET);
        return;
    }

    for (int i = 0; i < totalItens; i++) {
        printf("[%d] %s | %s | Quantidade: %d\n", i + 1,
               inventario[i].nome,
               inventario[i].tipo,
               inventario[i].quantidade);
    }
}

void inserirItem() {
    if (totalItens >= INVENTARIO_MAX) {
        printf(VERMELHO "Mochila cheia!\n" RESET);
        return;
    }

    Item novo;

    printf(AZUL "Nome do item: " RESET);
    lerLinha(novo.nome, 30);

    printf(AZUL "Tipo (arma/municao/cura/ferramenta): " RESET);
    lerLinha(novo.tipo, 20);

    printf(AZUL "Quantidade: " RESET);
    scanf("%d", &novo.quantidade);
    limparBuffer();

    inventario[totalItens++] = novo;

    printf(VERDE "Item adicionado!\n" RESET);
}

void removerItem() {
    char nome[30];
    printf(AZUL "Nome do item a remover: " RESET);
    lerLinha(nome, 30);

    for (int i = 0; i < totalItens; i++) {
        if (strcmp(inventario[i].nome, nome) == 0) {
            for (int j = i; j < totalItens - 1; j++) {
                inventario[j] = inventario[j + 1];
            }
            totalItens--;
            printf(VERDE "Item removido!\n" RESET);
            return;
        }
    }

    printf(VERMELHO "Item não encontrado.\n" RESET);
}

void buscarItem() {
    char nome[30];
    int comparacoes = 0;

    printf(AZUL "Nome do item: " RESET);
    lerLinha(nome, 30);

    for (int i = 0; i < totalItens; i++) {
        comparacoes++;
        if (strcmp(inventario[i].nome, nome) == 0) {
            printf(VERDE "Encontrado após %d comparações: %s | %s\n" RESET,
                   comparacoes,
                   inventario[i].nome,
                   inventario[i].tipo);
            return;
        }
    }

    printf(VERMELHO "Item não encontrado (%d comparações).\n" RESET, comparacoes);
}

/* ================= MÓDULO NOVATO ================= */

void moduloNovato() {
    int op;

    do {
        printf(AZUL "\n=== INVENTÁRIO DO SOBREVIVENTE - NOVATO ===\n" RESET);
        listarInventario();
        printf("\n1 - Inserir item\n2 - Remover item\n3 - Buscar item\n0 - Voltar\nEscolha: ");
        scanf("%d", &op);
        limparBuffer();

        if (op == 1) inserirItem();
        else if (op == 2) removerItem();
        else if (op == 3) buscarItem();

    } while (op != 0);
}

/* ================= LISTA ENCADEADA ================= */

void inserirLista(No **inicio) {
    Item novo;
    printf(AZUL "Nome: " RESET);
    lerLinha(novo.nome, 30);
    printf(AZUL "Tipo: " RESET);
    lerLinha(novo.tipo, 20);
    printf(AZUL "Quantidade: " RESET);
    scanf("%d", &novo.quantidade);
    limparBuffer();

    No* no = malloc(sizeof(No));
    no->dados = novo;
    no->proximo = *inicio;
    *inicio = no;

    printf(VERDE "Inserido na lista.\n" RESET);
}

void listarLista(No *inicio) {
    printf(AZUL "\n--- MOCHILA (LISTA ENCADEADA) ---\n" RESET);
    int i = 1;

    while (inicio) {
        printf("[%d] %s | %s | %d\n", i++, inicio->dados.nome,
               inicio->dados.tipo,
               inicio->dados.quantidade);
        inicio = inicio->proximo;
    }
}

void buscarLista(No *inicio) {
    char nome[30];
    int comp = 0;

    printf(AZUL "Nome: " RESET);
    lerLinha(nome, 30);

    while (inicio) {
        comp++;
        if (strcmp(inicio->dados.nome, nome) == 0) {
            printf(VERDE "Encontrado (%d comparações)\n" RESET, comp);
            return;
        }
        inicio = inicio->proximo;
    }

    printf(VERMELHO "Não encontrado (%d comparações)\n" RESET, comp);
}

/* ================= MÓDULO AVENTUREIRO ================= */

void moduloAventureiro() {
    int opcao, estrutura;
    No *lista = NULL;

    do {
        printf(AZUL "\n=== INVENTÁRIO DO SOBREVIVENTE - AVENTUREIRO ===\n" RESET);
        printf("1 - Vetor\n2 - Lista Encadeada\n0 - Voltar\nEscolha: ");
        scanf("%d", &estrutura);
        limparBuffer();

        if (estrutura == 1) moduloNovato();
        else if (estrutura == 2) {
            do {
                printf(AZUL "\n--- LISTA ENCADEADA ---\n" RESET);
                printf("1 - Inserir\n2 - Listar\n3 - Buscar\n0 - Voltar\nEscolha: ");
                scanf("%d", &opcao);
                limparBuffer();

                if (opcao == 1) inserirLista(&lista);
                else if (opcao == 2) listarLista(lista);
                else if (opcao == 3) buscarLista(lista);

            } while (opcao != 0);
        }

    } while (estrutura != 0);
}

/* ================= MÓDULO MESTRE ================= */

Componente componentes[COMPONENTES_MAX];
int totalComp = 0;

void cadastrarComponente() {
    Componente c;

    printf(AZUL "Nome do componente: " RESET);
    lerLinha(c.nome, 30);

    printf(AZUL "Tipo: " RESET);
    lerLinha(c.tipo, 20);

    printf(AZUL "Prioridade: " RESET);
    scanf("%d", &c.prioridade);
    limparBuffer();

    componentes[totalComp++] = c;
}

void listarComponentes() {
    printf(AZUL "\n--- TORRE DE RESGATE ---\n" RESET);
    for (int i = 0; i < totalComp; i++) {
        printf("[%d] %s | %s | Prioridade %d\n",
               i + 1,
               componentes[i].nome,
               componentes[i].tipo,
               componentes[i].prioridade);
    }
}

void bubbleNome() {
    for (int i = 0; i < totalComp - 1; i++) {
        for (int j = 0; j < totalComp - i - 1; j++) {
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
                Componente temp = componentes[j];
                componentes[j] = componentes[j + 1];
                componentes[j + 1] = temp;
            }
        }
    }
    printf(VERDE "Ordenado por nome!\n" RESET);
}

void moduloMestre() {
    int op;

    do {
        printf(AZUL "\n=== INVENTÁRIO DO SOBREVIVENTE - MESTRE ===\n" RESET);
        printf("1 - Cadastrar componente\n2 - Listar torre\n3 - Ordenar por nome\n0 - Voltar\nEscolha: ");
        scanf("%d", &op);
        limparBuffer();

        if (op == 1) cadastrarComponente();
        else if (op == 2) listarComponentes();
        else if (op == 3) bubbleNome();

    } while (op != 0);
}

/* ================= MAIN ================= */

int main() {
    srand(time(NULL));
    int escolha;

    do {
        printf(AZUL "\n=== INVENTÁRIO DO SOBREVIVENTE ===\n" RESET);
        printf("1 - Nível Novato\n2 - Nível Aventureiro\n3 - Nível Mestre\n0 - Sair\nEscolha: ");
        scanf("%d", &escolha);
        limparBuffer();

        if (escolha == 1) moduloNovato();
        else if (escolha == 2) moduloAventureiro();
        else if (escolha == 3) moduloMestre();

    } while (escolha != 0);

    printf(VERDE "\nJogo encerrado. Boa sorte na ilha!\n" RESET);
    return 0;
}
