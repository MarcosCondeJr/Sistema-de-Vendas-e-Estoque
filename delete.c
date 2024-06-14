#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <stdbool.h>

#define MAX_CLIENTES 100
#define MAX_PRODUTOS 100
#define MAX_COMPRAS 100
#define MAX_CARACTER 50

typedef struct {
    int id;
    char nome[MAX_CARACTER];
    float preco;
    int quantidade;
} Produto;

typedef struct {
    int id;
    char nome[MAX_CARACTER];
} Cliente;

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

// Modulo venda - cadastrar cliente
void cadastrarCliente() {
    char buffer[MAX_CARACTER];

    if (numClientes < MAX_CLIENTES) {
        printf("Digite o nome do cliente: ");
        scanf(" %49[^\n]s", buffer);

        strcpy(clientes[numClientes].nome, buffer);
        numClientes++;

        printf("Cliente cadastrado com sucesso!\n");
    } else {
        printf("Limite de clientes atingido.\n");
    }
}

// Modulo estoque - Adiciona uma quantidade ao estoque de um produto
void adicionar(Produto produtos[], int numProdutos) {
    char nomeP[MAX_CARACTER];
    int qtdP;
    int i;

    printf("Digite o nome do produto: ");
    scanf("%s", nomeP);

    printf("Digite a quantidade do produto a ser adicionada: ");
    scanf("%d", &qtdP);

    if (qtdP <= 0) {
        printf("A quantidade escolhida deve ser positiva!\n");
        return;
    }

    for (i = 0; i < numProdutos; i++) {
        if (strcmp(produtos[i].nome, nomeP) == 0) {
            produtos[i].quantidade += qtdP;
            printf("Quantidade de %s atualizada para %d.\n", produtos[i].nome, produtos[i].quantidade);
            return;
        }
    }
    printf("Produto de nome %s não encontrado.\n", nomeP);
}

// Modulo estoque - Cadastra produto
void cadastrarProduto() {
    if (numProdutos < MAX_PRODUTOS) {
        Produto p;
        p.id = numProdutos + 1;
        printf("Digite o nome do produto: ");
        scanf(" %49[^\n]s", p.nome);
        printf("Digite o preco do produto: ");
        scanf("%f", &p.preco);
        printf("Digite a quantidade do produto: ");
        scanf("%d", &p.quantidade);
        produtos[numProdutos] = p;
        numProdutos++;
        printf("Produto cadastrado com sucesso!\n");
    } else {
        printf("Limite de produtos atingido.\n");
    }
}

// Modulo venda - realiza uma compra
void comprar() {
    char buffer[MAX_CARACTER], compara[MAX_CARACTER];
    int qtd_compras, quantidade, i, x, z;
    int achou = 0;

    if (numClientes == 0 || numProdutos == 0) {
        printf("Não há clientes ou produtos cadastrados.\n");
        return;
    }

    printf("Digite o nome cliente: ");
    scanf(" %49[^\n]s", buffer);

    for (i = 0; buffer[i] != '\0'; i++) {
        buffer[i] = tolower(buffer[i]);
    }

    for (i = 0; i < numClientes; i++) {
        strcpy(compara, clientes[i].nome);
        for (x = 0; compara[x] != '\0'; x++) {
            compara[x] = tolower(compara[x]);
        }
        if (strcmp(compara, buffer) == 0) {
            achou = 1;
            break;
        }
    }

    if (achou == 0) {
        printf("Cliente não encontrado.\n");
        return;
    }

    buffer[0] = toupper(buffer[0]);
    achou = 0;
    strcpy(carrinho.nomecliente, buffer);

    printf("Digite o número de produtos que deseja comprar: ");
    scanf("%d", &qtd_compras);

    if (qtd_compras > MAX_COMPRAS) {
        printf("Limite de produtos por compra excedido.\n");
        return;
    }

    for (i = 0; i < qtd_compras; i++) {
        achou = 0;
        printf("Digite o nome do %d produto: ", i + 1);
        scanf(" %49[^\n]s", buffer);

        for (x = 0; buffer[x] != '\0'; x++) {
            buffer[x] = tolower(buffer[x]);
        }

        for (x = 0; x < numProdutos; x++) {
            strcpy(compara, produtos[x].nome);
            for (z = 0; compara[z] != '\0'; z++) {
                compara[z] = tolower(compara[z]);
            }

            if (strcmp(compara, buffer) == 0) {
                achou = 1;

                if (produtos[x].quantidade == 0) {
                    printf("Não quantidade deste produto no estoque...\n");
                    break;
                }

                do {
                    printf("Quantidade de \'%s\' em estoque - %d;\n", produtos[x].nome, produtos[x].quantidade);
                    printf("Digite a quantidade do %d produto: ", i + 1);
                    scanf("%d", &quantidade);

                    system("cls");
                } while (!(quantidade > 0 && quantidade <= produtos[x].quantidade));

                produtos[x].quantidade -= quantidade;

                strcpy(carrinho.nomeproduto[numCompras], buffer);
                carrinho.quantidade[numCompras] = quantidade;
                carrinho.preco[numCompras] = produtos[x].preco;

                numCompras++;

                printf("Compra realizada com sucesso!!\n");
                break;
            }
        }

        if (!achou) {
            printf("Produto não encontrado.\n");
            return;
        }
    }

    carrinho.num_produtos = numCompras;
}

// Modulo venda - Lista todos os clientes
void listarClientes() {
    int i;
    printf("Lista de Clientes: \n");
    for (i = 0; i < numClientes; i++) {
        printf("ID: %d, Nome: %s\n", clientes[i].id, clientes[i].nome);
    }
}

// Modulo estoque - Lista todos os produtos
void listarProdutos() {
    int i;
    for (i = 0; i < numProdutos; i++) {
        printf("ID: %d, Nome: %s, Preço: %.2f, quantidade: %d\n", produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
}

// Modulo vendas - lista todas as compras efetuadas
void listarCompras() {
    int j;
    printf("Nome cliente: %s\n", carrinho.nomecliente);
    for (j = 0; j < carrinho.num_produtos; j++) {
        printf("Produto: %s, Quantidade: %d, Preço: %.2f\n", carrinho.nomeproduto[j], carrinho.quantidade[j], carrinho.preco[j]);
    }
}

// Modulo vendas/estoque - Permite a troca de usuários
int trocarUsuario() {
    int senha_usuario;
    int cod_usuario;

    printf("Informe o login: ");
    scanf("%d", &cod_usuario);
    printf("Informe a senha: ");
    scanf("%d", &senha_usuario);
    // Vendas
    if (cod_usuario == 2525 && senha_usuario == 1234) {
        return 1;
    }
    // Estoque
    if (cod_usuario == 9973 && senha_usuario == 4321) {
        return 2;
    }
    // Senha ou login errados
    return -1;
}

// Modulo estoque - Função para excluir um produto do estoque pelo nome
void delete_product(Produto produtos[], int *numProdutos, char nomeP[]) {
    bool produto_existe = false;
    int indice = -1;
    int i;

    // Verificar se o produto existe no estoque
    for (i = 0; i < *numProdutos; i++) {
        if (strcmp(produtos[i].nome, nomeP) == 0) {
            produto_existe = true;
            indice = i;
            break;
        }
    }

    if (!produto_existe) {
        printf("Produto de nome %s não encontrado no estoque.\n", nomeP);
        return;
    }

    // Remover o produto do estoque e ajustar a lista de produtos
    for (i = indice; i < *numProdutos - 1; i++) {
        produtos[i] = produtos[i + 1];
    }

    (*numProdutos)--;
    printf("Produto removido com sucesso!\n");
}

int main() {
    int login, opcao = -1;
    char nomeP[MAX_CARACTER];

    setlocale(LC_ALL, "Portuguese");

    do {
        login = trocarUsuario();

        if (login == 1) {
            do {
                printf("1. Cadastrar Cliente\n");
                printf("2. Realizar Venda\n");
                printf("3. Listar Carrinho\n");
                printf("4. Trocar de Usuário\n");
                printf("0. Sair\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);

                system("cls");

                switch (opcao) {
                    case 1:
                        cadastrarCliente();
                        break;
                    case 2:
                        comprar();
                        break;
                    case 3:
                        listarCompras();
                        break;
                    case 4:
                        break;
                    case 0:
                        login = -1;
                        break;
                    default:
                        printf("Opção inválida, por favor tente novamente...\n");
                        break;
                }
            } while (opcao != 0 && opcao != 4);
        } else if (login == 2) {
            do {
                printf("1. Cadastrar Produto\n");
                printf("2. Adicionar Produto\n");
                printf("3. Listar Produtos\n");
                printf("4. Remover Produto\n");
                printf("5. Trocar Usuário\n");
                printf("0. Sair\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);

                system("cls");

                switch (opcao) {
                    case 1:
                        cadastrarProduto();
                        break;
                    case 2:
                        adicionar(produtos, numProdutos);
                        break;
                    case 3:
                        listarProdutos();
                        break;
                    case 4:
                        printf("Digite o nome do produto a ser removido: ");
                        scanf(" %49[^\n]s", nomeP);
                        delete_product(produtos, &numProdutos, nomeP);
                        break;
                    case 5:
                        break;
                    case 0:
                        login = -1;
                        break;
                    default:
                        printf("Opção inválida, por favor tente novamente...\n");
                        break;
                }
            } while (opcao != 0 && opcao != 5);
        } else {
            system("cls");
            printf("Usuário e/ou senha incorreta(as)...\n");
        }
    } while (opcao != 0);

    printf("FIM\n");
    return 0;
}
