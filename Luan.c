#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

#define MAX_CLIENTES 100
#define MAX_PRODUTOS 100
#define MAX_COMPRAS 100
#define MAX_CARACTER 50

typedef struct {
    int id;
    char nome[MAX_CARACTER];
} Cliente;

typedef struct {
    int id;
    char nome[MAX_CARACTER];
    float preco;
    int quantidade;
} Produto;

typedef struct {
    char nomecliente[MAX_CARACTER];
    char nomeproduto[MAX_COMPRAS][MAX_CARACTER];
    int quantidade[MAX_COMPRAS];
    int num_produtos;
    float preco[MAX_COMPRAS];
} Carrinho;

Cliente clientes[MAX_CLIENTES];
Produto produtos[MAX_PRODUTOS];
Carrinho carrinho;

int numClientes = 0;
int numProdutos = 0;
int numCompras = 0;

// Função para cadastrar um novo cliente
void cadastrarCliente() {
    char nome[MAX_CARACTER];

    if (numClientes < MAX_CLIENTES) {
        printf("Digite o nome do cliente: ");
        scanf(" %49[^\n]", nome);

        strcpy(clientes[numClientes].nome, nome);
        clientes[numClientes].id = numClientes + 1;
        numClientes++;

        printf("Cliente cadastrado com sucesso!\n");
    } else {
        printf("Limite de clientes atingido.\n");
    }
}

// Função para adicionar uma quantidade ao estoque de um produto
void adicionarEstoque() {
    char nome[MAX_CARACTER];
    int quantidade;

    printf("Digite o nome do produto: ");
    scanf(" %49[^\n]", nome);

    for (int i = 0; i < numProdutos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            printf("Digite a quantidade a ser adicionada ao estoque: ");
            scanf("%d", &quantidade);

            if (quantidade <= 0) {
                printf("A quantidade a ser adicionada deve ser positiva.\n");
                return;
            }

            produtos[i].quantidade += quantidade;

            printf("Estoque do produto '%s' atualizado para %d unidades.\n", produtos[i].nome, produtos[i].quantidade);
            return;
        }
    }

    printf("Produto '%s' não encontrado.\n", nome);
}

// Função para cadastrar um novo produto
void cadastrarProduto() {
    if (numProdutos < MAX_PRODUTOS) {
        Produto novoProduto;

        printf("Digite o nome do produto: ");
        scanf(" %49[^\n]", novoProduto.nome);

        printf("Digite o preco do produto: ");
        scanf("%f", &novoProduto.preco);

        printf("Digite a quantidade do produto: ");
        scanf("%d", &novoProduto.quantidade);

        novoProduto.id = numProdutos + 1;
        produtos[numProdutos] = novoProduto;
        numProdutos++;

        printf("Produto cadastrado com sucesso!\n");
    } else {
        printf("Limite de produtos atingido.\n");
    }
}

// Função para realizar uma compra
void realizarCompra() {
    // Implementação da função aqui
}

// Função para listar todos os clientes
void listarClientes() {
    printf("Lista de Clientes:\n");
    for (int i = 0; i < numClientes; i++) {
        printf("ID: %d, Nome: %s\n", clientes[i].id, clientes[i].nome);
    }
}

// Função para listar todos os produtos
void listarProdutos() {
    printf("Lista de Produtos:\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("ID: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
}

// Função principal
int main() {
    int opcao;

    setlocale(LC_ALL, "Portuguese");

    do {
        printf("\n===== MENU =====\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Cadastrar Produto\n");
        printf("3. Adicionar Estoque\n");
        printf("4. Realizar Compra\n");
        printf("5. Listar Clientes\n");
        printf("6. Listar Produtos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarCliente();
                break;
            case 2:
                cadastrarProduto();
                break;
            case 3:
                adicionarEstoque();
                break;
            case 4:
                realizarCompra();
                break;
            case 5:
                listarClientes();
                break;
            case 6:
                listarProdutos();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

