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
    char nomeproduto[MAX_COMPRAS];
    int quantidade[MAX_COMPRAS];
    float preco;
} Carrinho;

Cliente clientes[MAX_CLIENTES];
Produto produtos[MAX_PRODUTOS];
Carrinho carrinho[MAX_COMPRAS];

int numClientes = 0;
int numProdutos = 0;
int numCompras = 0;

//Modulo venda - cadastrar cliente
void cadastrarCliente() {
    if (numClientes < MAX_CLIENTES) {
        Cliente c;
        c.id = numClientes + 1;
        printf("Digite o nome do cliente: ");
        scanf("%s", c.nome);
        clientes[numClientes] = c;
        numClientes++;
        printf("Cliente cadastrado com sucesso!\n");
    } else {
        printf("Limite de clientes atingido.\n");
    }
}
//Modulo estoque - Adiciona uma quantidade ao estoque de um produto
void adicionar(Produto produtos[], int numProduto) {
    char nomeP[MAX_CARACTER];
    int qtdP;
    
    printf("Digite o nome do produto: ");
    scanf("%s", nomeP);
    
    printf("Digite a quantidade: ");
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
//Modulo estoque - Cadastra produto
void cadastrarProduto() {
    if (numProdutos < MAX_PRODUTOS) {
        Produto p;
        p.id = numProdutos + 1;
        printf("Digite o nome do produto: ");
        scanf(" %49[^\n]s", p.nome);
        printf("Digite o preco do produto: ");
        scanf("%f", &p.preco);
        produtos[numProdutos] = p;
        numProdutos++;
        printf("Produto cadastrado com sucesso!\n");
    } else {
        printf("Limite de produtos atingido.\n");
    }
}
//Modulo venda - realiza uma compra;
//Pessoa trabalhando no modulo - TAUÃ
void comprar() {
	char buffer[MAX_CARACTER], compara[MAX_CARACTER];
	int indice;
	int achou = 0;
	
    if (numClientes == 0 || numProdutos == 0) {
        printf("Não há clientes ou produtos cadastrados.\n");
        
        return;
    }

    int clienteId, produtoId, quantidade, i, x;
    
    printf("Digite o nome cliente: ");
    scanf(" %49[^\n]s", buffer);
    
    for(i = 0; buffer[i] != '\0'; i++){
    	buffer[i] = tolower(buffer[i]);	
	}


	for(i = 0; i < numClientes; i++){
		strcpy(compara, clientes[i].nome);
		
		for(x = 0; compara[x] != '\0'; x++){
			compara[x] = tolower(compara[x]);
		}
		
		if(strcmp(compara, buffer) == 0){
			achou = 1;
			indice = i;
			
			break;
		}
	}
	
    if (achou == 0) {
        printf("Cliente não encontrado.\n");
        return;
    }
    
    buffer[0] = toupper(buffer[0]);

    strcpy(carrinho[indice].nomecliente);
    c.clienteId = clienteId;
    c.numProdutos = 0;

    printf("Digite o número de produtos que deseja comprar: ");
    scanf("%d", &c.numProdutos);

    if (c.numProdutos > MAX_COMPRAS) {
        printf("Limite de produtos por compra excedido.\n");
        return;
    }

    for (i = 0; i < c.numProdutos; i++) {
        printf("Digite o ID do produto %d: ", i + 1);
        scanf("%d", &produtoId);
        if (produtoId <= 0 || produtoId > numProdutos) {
            printf("Produto não encontrado.\n");
            return;
        }
        printf("Digite a quantidade do produto %d: ", i + 1);
        scanf("%d", &quantidade);

        c.produtoIds[i] = produtoId;
        c.quantidade[i] = quantidade;
        produtos[c.produtoIds[i] - 1].quantidade -= quantidade;
        //atualiza o estoque
    }

    compras[numCompras] = c;
    numCompras++;
    printf("Compra realizada com sucesso!\n");
}
//Modulo venda - Lista todos os clientes;
void listarClientes() {
    int i;
    printf("Lista de Clientes: \n");
    for (i = 0; i < numClientes; i++) {
        printf("ID: %d, Nome: %s\n", clientes[i].id, clientes[i].nome);
    }
}
//Modulo estoque - Lista todos os produtos
void listarProdutos() {
    int i;
    for (i = 0; i < numProdutos; i++) {
        printf("ID: %d, Nome: %s, Preço: %.2f\n", produtos[i].id, produtos[i].nome, produtos[i].preco);
    }
}
//Modulo vendas - lista todas as compras efetuadas
void listarCompras() {
    int i, j;
    for (i = 0; i < numCompras; i++) {
        printf("Cliente ID: %d\n", compras[i].clienteId);
        for (j = 0; j < compras[i].numProdutos; j++) {
            printf("Produto ID: %d, Quantidade: %d\n", compras[i].produtoIds[j], compras[i].quantidade[j]);
        }
    }
}
//Modulo vendas/estoque - Permite a troca de usuários
int trocarUsuario(){
	int senha_usuario;
	int cod_usuario;

    int login;

	printf("Informe o login: ");
    scanf("%d",&cod_usuario);
    printf("Informe a senha:");
    scanf("%d",&senha_usuario);
    //Vendas
    if(cod_usuario == 2525 && senha_usuario == 1234){
        return 1;
    }
    //Estoque
    if(cod_usuario == 9973 && senha_usuario == 4321){
        return 2;
    }
    //Senha ou login errados;
    return -1;
	
}

int main() {
	int login, opcao;
	
	setlocale(LC_ALL, "Portuguese");

    do{
    	login = trocarUsuario();
    	
        if(login == 1){
            do{
                printf("1. Cadastrar Cliente\n");
                printf("2. Realizar Venda\n");
                printf("3. Listar carrinho\n");
                printf("4. Trocar de Usuário\n");
				printf("5. Atualizar carrinho\n"); 
                printf("0. Sair\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);
                
                system("cls");

                switch(opcao){
                    case 1:
                        cadastrarCliente();
                    break;
                    case 2:
                        comprar();
                    break;
                    case 3:
                        listarProdutos();
                    break;
                    case 4:
                        
                    break;
                    case 0:
                        login = -1;
                    break;
                    default:
                        printf("Opção inválida, por favor tente novamente...");
                    break;

                }
            }while(opcao != 0 && opcao != 4);
        }
        else if(login == 2){
        	do{
				
	        	printf("1. Cadastrar produtos\n");
	        	printf("2. Adicionar/Remover produto\n");
	        	printf("3. Listar produtos\n");
	        	printf("4. Trocar usuário\n");
	        	printf("0. Sair\n");
	        	printf("Escolha uma opção: ");
	        	scanf("%d", &opcao);
	        	
	        	system("cls");
	        	
	        	switch(opcao){
	        		case 1:
	        			cadastrarProduto();
	        		break;
	        		case 2:
	    
	        		break;
	        		case 3:
	        			
	        		break;
	        		case 4:
	        			
	        		break;
				}
			}while(opcao != 0 && opcao != 4);
		}
		else{
				system("cls");
			
				printf("Usuário e/ou senha incorreta(as)...\n");
			}
			
    }while(opcao != 0);
    
    printf("FIM");
    return 0;
}
