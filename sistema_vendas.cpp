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

/*Função responsável por fazer teste de erro
TESTE 1 - Testa se é composto somente de letras
TESTE 2- Testa se é composto somente de números

Caso haja um erro ele retorna 1, caso contrário retorna 0*/
int tratamento(char analise[], int teste){
	int i;
	
	for(i = 0; analise[i] != '\0'; i++){
		
		if(teste == 1){
			if(isalpha(analise[i]) == 0){
				return 1;
			}
		}
		else{
			if(isdigit(analise[i]) == 0){
				return 1;
			}
		}
	}
	
	return 0;
}
// Função para cadastrar cliente
void cadastrarCliente() {
    char buffer[MAX_CARACTER];

    if (numClientes < MAX_CLIENTES) {
        printf("Digite o nome do cliente: ");
        scanf(" %49[^\n]s", buffer);

        strcpy(clientes[numClientes].nome, buffer);
        clientes[numClientes].id = numClientes + 1;
        numClientes++;

        printf("Cliente cadastrado com sucesso!\n");
    } else {
        printf("Limite de clientes atingido.\n");
    }
}

// Função para adicionar quantidade ao estoque de um produto
void adicionar(Produto produtos[], int numProduto) {
    char nomeP[MAX_CARACTER];
    int qtdP;

    printf("Digite o nome do produto: ");
    scanf(" %49[^\n]s", nomeP);

    printf("quantidade a ser adicionadas: ");
    scanf("%d", &qtdP);

    if(qtdP<=0){
        printf("a quantidade escolhida deve ser possitiva!\n");
    }

    for (int i = 0; i < MAX_PRODUTOS; i++) {
        if (strcmp(produtos[i].nome, nomeP) == 0) {
            produtos[i].quantidade += qtdP;

            printf("Quantidade de %s atualizada para %d.\n", produtos[i].nome, produtos[i].quantidade);
        }
    }
    printf("Produto de nome %s não encontrado.\n", nomeP);
}

// Função para cadastrar produto
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

// Função para realizar uma compra
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
                    printf("Não há quantidade deste produto no estoque...\n");
                    break;
                }

                do {
                    printf("Quantidade de '%s' em estoque - %d;\n", produtos[x].nome, produtos[x].quantidade);
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

// Função para listar todos os clientes
void listarClientes() {
    int i;
    printf("Lista de Clientes: \n");
    for (i = 0; i < numClientes; i++) {
        printf("ID: %d, Nome: %s\n", clientes[i].id, clientes[i].nome);
    }
}

// Função para listar todos os produtos
void listarProdutos() {
    int i;
    for (i = 0; i < numProdutos; i++) {
        printf("ID: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
}

// Função para listar todas as compras efetuadas
void listarCompras() {
    int j;
    printf("Nome cliente: %s\n", carrinho.nomecliente);

    for (j = 0; j < carrinho.num_produtos; j++) {
        printf("Nome: %s, Quantidade: %d, Preço: %.2f\n", carrinho.nomeproduto[j], carrinho.quantidade[j], carrinho.preco[j]);
    }
}

// Função para excluir um produto do estoque
void delete_product(Produto estoque[], int *num_produtos, int produto_id) {
    bool produto_existe = false;
    int indice = -1;
    int i;  // Declare a variável fora do loop

    // Verificar se o produto existe no estoque
    for (i = 0; i < *num_produtos; i++) {  // Use a variável aqui
        if (estoque[i].id == produto_id) {
            produto_existe = true;
            indice = i;
            break;
        }
    }

    if (!produto_existe) {
        printf("Produto com ID %d não encontrado no estoque.\n", produto_id);
        return;
    }

    // Remover o produto do estoque e ajustar a lista
    for (i = indice; i < *num_produtos - 1; i++) {  // Use a variável aqui
        estoque[i] = estoque[i + 1];
    }
    (*num_produtos)--;

    printf("Produto com ID %d foi removido do estoque.\n", produto_id);
}

// Função para trocar de usuário
int logar() {
	char buffer[4];
    int senha_usuario;
    int cod_usuario;
	
	while(1){
	    printf("Informe o login: ");
	    scanf("%s", buffer);
	    
	    if(tratamento(buffer, 2)){
	    	system("cls");
	    	
	    	printf("ERRO!! Foi digitado algo diferente de um número...\n");
	    	printf("Por favor, tente novamente...\n");
	    	
	    	continue;
		}
		
		sscanf(buffer, "%d", &cod_usuario);
		
	    printf("Informe a senha: ");
	    scanf("%s", buffer);
	    
	    if(tratamento(buffer, 2)){
	    	system("cls");
	    	
	    	printf("ERRO!! Foi digitado algo diferente de um número...\n");
	    	printf("Por favor, tente novamente...\n");
	    	
	    	continue;
		}
	    
	    sscanf(buffer, "%d", &senha_usuario);
	
	    // Senha valida
	    if (cod_usuario == 2525 && senha_usuario == 1234) {
	        return 1;
	    }
	    // Senha ou login errados
	    return -1;
	}
}
//Reestruturar main para ter uma tela;
int main() {
	char buffer[4];
    int login, opcao = -1, produto_id;

    setlocale(LC_ALL, "Portuguese");

    do {
        login = logar;

        if (login == 1) {
            do {
                printf("1. Cadastrar Cliente\n");
                printf("2. Cadastrar Produto\n");
                printf("3. Realizar Venda\n");
                printf("4. Adicionar Produto\n");
                printf("5. Listar Compras\n");
                printf("6. Listar Produtos\n");
                printf("7. Remover Produto\n");
                printf("0. Sair\n");
                printf("Escolha uma opção: ");
                scanf("%s", buffer);
                
                system("cls");
                
                if(tratamento(buffer, 2)){
                	printf("ERRO!! Foi digitado algo que não era um número...\n");
                	printf("Por favor tente novamente.\n");
                	
                	opcao = -1;
                	continue;
				}
			
				sscanf(buffer, "%d", &opcao);
				
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
                        login = -1;
                        break;
                    case 0:
                        login = -1;
                        break;
                    default:
                        printf("Opção inválida, por favor tente novamente...\n");
                        break;
                }
			
            } while (opcao != 0);
        } 
		else{

            system("cls");
            printf("Usuário e/ou senha incorreta(as)...\n");
        }
    } while (opcao != 0);
    
    printf("FIM\n");
    return 0;
}

