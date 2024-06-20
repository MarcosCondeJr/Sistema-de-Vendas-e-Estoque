#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>

//Back end
#define MAX_CLIENTES 100
#define MAX_PRODUTOS 100
#define MAX_COMPRAS 100
#define MAX_CARACTER 50

//Front end
#define WIDTH 70
#define HEIGHT 15
//Estruturas do Backend
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

//Funções do Front end
void clear_line(int y);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string); //Função para printar no centro
void pag_login();   //Função para a pagina de login
void pag_principal(); //Função para a pagina principal
void cadastrar_cliente();
void draw_rounded_box(WINDOW *win, int starty, int startx, int height, int width);
void print_large_text(WINDOW *win, int starty, int startx, const char *string); //função para printar um texto longo


int main() {
    setlocale(LC_ALL, "PORTUGUESE");
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);    // COR DO TEXTO BRANCO, COR DE FUNDO PRETO
    init_pair(2, COLOR_RED, COLOR_BLACK);     // Texto vermelho para erros
    init_pair(3, COLOR_BLACK, COLOR_WHITE);    // Texto preto, fundo branco (para campos de entrada)
    init_pair(4, COLOR_GREEN, COLOR_BLACK); //texto verde, para sucessos
    init_pair(5, COLOR_BLUE, COLOR_BLACK); //texto azul, para sucessos
    init_pair(6, COLOR_YELLOW, COLOR_BLACK); //texto amarelo, para produtos
    init_pair(7, COLOR_BLACK, COLOR_BLUE); //texto amarelo, para produtos




    bkgd(COLOR_PAIR(1)); // Definir o fundo da janela principal como branco
    cbreak();
    noecho();
    curs_set(0);

    pag_login();

    endwin();
    return 0;
}

void clear_line(int y) {
    // Obtém o número de colunas da tela
    int cols = COLS;

    // Move o cursor para o início da linha desejada
    move(y, 0);

    // Limpa a linha imprimindo espaços até o final da linha
    for (int i = 0; i < cols; i++) {
        addch(' ');
    }

    // Retorna o cursor para o início da linha
    move(y, 0);

    // Atualiza a tela para refletir as mudanças
    refresh();
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string) {
    int length, x, y;
    float temp;

    if (win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startx + (int)temp;
    wattron(win, A_BOLD | COLOR_PAIR(1));
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, A_BOLD | COLOR_PAIR(1));
    refresh();
}

void draw_rounded_box(WINDOW *win, int starty, int startx, int height, int width) {
    mvwaddch(win, starty, startx, ACS_ULCORNER);
    mvwaddch(win, starty, startx + width - 1, ACS_URCORNER);
    mvwaddch(win, starty + height - 1, startx, ACS_LLCORNER);
    mvwaddch(win, starty + height - 1, startx + width - 1, ACS_LRCORNER);
    mvwhline(win, starty, startx + 1, ACS_HLINE, width - 2);
    mvwhline(win, starty + height - 1, startx + 1, ACS_HLINE, width - 2);
    mvwvline(win, starty + 1, startx, ACS_VLINE, height - 2);
    mvwvline(win, starty + 1, startx + width - 1, ACS_VLINE, height - 2);
}

void print_large_text(WINDOW *win, int starty, int startx, const char *string) {
    int length, x, y;
    float temp;

    length = strlen(string);
    temp = (WIDTH - length) / 2;
    x = startx + (int)temp;
    wattron(win, A_BOLD | COLOR_PAIR(1));
    mvwprintw(win, starty, x, "%s", string);
    wattroff(win, A_BOLD | COLOR_PAIR(1));
}

//Pagina de login
void pag_login() {
    WINDOW *login_win;
    int startx, starty, width, height;
    char username[20], password[20];
    int ch, i;

    height = HEIGHT;
    width = WIDTH;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    while (1) {
        login_win = newwin(height, width, starty, startx);
        wbkgd(login_win, COLOR_PAIR(1)); // Fundo Preto
        draw_rounded_box(login_win, 0, 0, height, width);
        refresh();

        wattron(login_win, A_BOLD | COLOR_PAIR(1));
        print_in_middle(login_win, 4, 0, WIDTH, "SISTEMA DE VENDAS");
        wattroff(login_win, A_BOLD | COLOR_PAIR(1));

        mvwprintw(login_win, 7, (WIDTH / 2)-10, "Usuario:");
        mvwprintw(login_win, 9, (WIDTH / 2)-10, "Senha:");

        // Highlighting the input fields
        wattron(login_win, COLOR_PAIR(3));
        mvwprintw(login_win, 7, (WIDTH/2), "        "); // Highlight the username field
        mvwprintw(login_win, 9, (WIDTH/2), "        "); // Highlight the password field
        wattroff(login_win, COLOR_PAIR(3));
        wrefresh(login_win);

        curs_set(1);

        // Insirir usuario
        wattron(login_win, COLOR_PAIR(3));
        wmove(login_win, 7, (WIDTH/2));
        echo();
        wgetnstr(login_win, username, sizeof(username) - 1);
        noecho();
        wattroff(login_win, COLOR_PAIR(3));

        // Inserir senha
        wattron(login_win, COLOR_PAIR(3));
        wmove(login_win, 9, (WIDTH/2));
        i = 0;
        while ((ch = wgetch(login_win)) != '\n') {
            if (ch == 127 || ch == KEY_BACKSPACE) {
                if (i > 0) {
                    i--;
                    mvwprintw(login_win, 6, 9 + i, " ");
                    wmove(login_win, 9, (WIDTH/2)+ i);
                }
            } else if (i < sizeof(password) - 1) {
                password[i++] = ch;
                mvwprintw(login_win, 9, (WIDTH/2) + i - 1, "*");
            }
            wrefresh(login_win);
        }
        password[i] = '\0';
        wattroff(login_win, COLOR_PAIR(3));

        // teste de usuario e senha
        if (strcmp(username, "2525") == 0 && strcmp(password, "1234") == 0) {
            delwin(login_win);
            pag_principal();
            break;
        } else {
            wclear(login_win);
            draw_rounded_box(login_win, 0, 0, height, width);
            wattron(login_win, A_BOLD | COLOR_PAIR(2));
            mvwprintw(login_win, (HEIGHT/2), 27, "Falha no Login!");
            wattroff(login_win, A_BOLD | COLOR_PAIR(2));
            wrefresh(login_win);
            wgetch(login_win);
            delwin(login_win);
        }
    }
}

//Pagina Principal
void pag_principal() {
    WINDOW *pagina_prin;
    int startx, starty, width, height;
    int opc_desejada;

    height = HEIGHT;
    width = WIDTH;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    pagina_prin = newwin(height, width, starty, startx);
    wbkgd(pagina_prin, COLOR_PAIR(1)); // Fundo branco
    draw_rounded_box(pagina_prin, 0, 0, height, width);
    refresh();

    wattron(pagina_prin, A_BOLD | COLOR_PAIR(1));
    print_large_text(pagina_prin, 2, 0, "SISTEMA DE VENDAS");

    // Lado esquerdo
    mvwprintw(pagina_prin, 4, 3, "1 - Cadastrar Cliente");
    mvwprintw(pagina_prin, 5, 3, "2 - Cadastrar Produto");
    mvwprintw(pagina_prin, 6, 3, "3 - Atualizar Estoque");
    mvwprintw(pagina_prin, 7, 3, "4 - Deletar Produto");

    // Lado direito
    mvwprintw(pagina_prin, 4, width - strlen("5 - Realizar Venda") - 4, "5 - Realizar Venda");
    mvwprintw(pagina_prin, 5, width - strlen("6 - Listar Carrinho") - 3, "6 - Listar Compras");
    mvwprintw(pagina_prin, 6, width - strlen("7 - Listar Clientes") - 3, "7 - Listar Clientes");
    mvwprintw(pagina_prin, 7, width - strlen("8 - Listar Produtos") - 3, "8 - Listar Produtos");

    // Rodapé
    mvwprintw(pagina_prin, 9, (width - strlen("9 - Sair ")) / 2+1, "9 Sair ");

    //Entrada da opção desejada
    mvwprintw(pagina_prin,12, (width - strlen("Enter:")) / 2-4, "Enter:");
    mvwprintw(pagina_prin,12, (width - strlen(" ")) / 2, " ");
    echo();
    opc_desejada = wgetch(pagina_prin);
    noecho();

    switch (opc_desejada) {
        case '1':
            wgetch(pagina_prin);
            cadastrar_cliente();
            break;
        case '2':
            wgetch(pagina_prin);
            cadastrar_produto();
            break;
        case '3':
            wgetch(pagina_prin);
            atualizar_estoque();
            break;
        case '4':
            wgetch(pagina_prin);
            deletar_produto();
            break;
        case '5':
            wgetch(pagina_prin);
            realizar_venda();
            break;
        case '6':
            wgetch(pagina_prin);
            listar_compras();
        case '7':
            wgetch(pagina_prin);
            listar_cliente();
            break;
        case '8':
            wgetch(pagina_prin);
            listar_prod();
            break;
        case '9':
            wgetch(pagina_prin);
            pag_login();
            break;
        default:
            wattron(pagina_prin, A_BOLD | COLOR_PAIR(2));
            mvwprintw(pagina_prin,13,(WIDTH/2)-8,"Opcao invalida");
            wattroff(pagina_prin, A_BOLD | COLOR_PAIR(2));
    }



    wattroff(pagina_prin, A_BOLD | COLOR_PAIR(1));

    wrefresh(pagina_prin);

    // Aguarda a entrada do usuário para fechar
    wgetch(pagina_prin);
    delwin(pagina_prin);
}

void cadastrar_produto() {
    if (numProdutos < MAX_PRODUTOS) {
        WINDOW *cad_prod;
        int startx, starty, width, height;
        char nome_pro[MAX_CARACTER];
        char valor[20], qtd[20];

        height = HEIGHT;
        width = WIDTH;
        starty = (LINES - height) / 2;
        startx = (COLS - width) / 2;

        cad_prod = newwin(height, width, starty, startx);
        wbkgd(cad_prod, COLOR_PAIR(1)); // Fundo branco
        draw_rounded_box(cad_prod, 0, 0, height, width);
        refresh();

        wattron(cad_prod, A_BOLD | COLOR_PAIR(1));
        print_large_text(cad_prod, 2, 0, "CADASTRO DE PRODUTO");
        wattroff(cad_prod, A_BOLD | COLOR_PAIR(1));

        // Entrada do nome do produto
        mvwprintw(cad_prod, 5, 10, "Nome do Produto: ");
        echo();
        wgetnstr(cad_prod, nome_pro, sizeof(nome_pro) - 1);
        noecho();

        // Entrada do valor do produto
        mvwprintw(cad_prod, 6, 10, "Valor: ");
        echo();
        wgetnstr(cad_prod, valor, sizeof(valor) - 1);
        noecho();

        // Entrada da quantidade do produto
        mvwprintw(cad_prod, 7, 10, "Quantidade: ");
        echo();
        wgetnstr(cad_prod, qtd, sizeof(qtd) - 1);
        noecho();

        // Converte os valores de string para os tipos apropriados
        Produto p;
        p.id = numProdutos + 1;
        strncpy(p.nome, nome_pro, MAX_CARACTER - 1);
        p.nome[MAX_CARACTER - 1] = '\0'; // Garantir terminação nula
        p.preco = atof(valor);
        p.quantidade = atoi(qtd);

        produtos[numProdutos] = p;
        numProdutos++;

        // Mensagem de produto cadastrado
        wattron(cad_prod, A_BOLD | COLOR_PAIR(4)); // COR VERDE NO NOME CLIENTE CADASTRADO
        mvwprintw(cad_prod, 10, (width - strlen("Produto cadastrado!")) / 2, "Produto cadastrado!");
        wattroff(cad_prod, A_BOLD | COLOR_PAIR(4));

        wgetch(cad_prod);
        delwin(cad_prod);
        pag_principal();
    } else {
        // Limite de produtos atingido
        mvprintw(LINES / 2, (COLS - strlen("Limite de produtos atingido.")) / 2, "Limite de produtos atingido.");
        refresh();
        getch();
        pag_principal();
    }
}

void cadastrar_cliente() {
    if (numClientes < MAX_CLIENTES) {
        WINDOW *cad_cli;
        int startx, starty, width, height;
        char client[MAX_CARACTER];

        height = HEIGHT;
        width = WIDTH;
        starty = (LINES - height) / 2;
        startx = (COLS - width) / 2;

        cad_cli = newwin(height, width, starty, startx);
        wbkgd(cad_cli, COLOR_PAIR(1)); // Fundo branco
        draw_rounded_box(cad_cli, 0, 0, height, width); //Quadro no terminal
        refresh();

        wattron(cad_cli, A_BOLD | COLOR_PAIR(1));
        print_large_text(cad_cli, 2, 0, "CADASTRAR CLIENTE");
        wattroff(cad_cli, A_BOLD | COLOR_PAIR(1));

        // Entrada do nome do Cliente
        mvwprintw(cad_cli, 5, 10, "Nome do Cliente: ");
        echo();
        wgetnstr(cad_cli, client, sizeof(client) - 1);
        noecho();

        // Armazena o cliente na estrutura
        Cliente c;
        c.id = numClientes + 1;
        strncpy(c.nome, client, MAX_CARACTER - 1);
        c.nome[MAX_CARACTER - 1] = '\0'; // Garantir terminação nula
        clientes[numClientes] = c;
        numClientes++;

        // Mensagem de cliente cadastrado
        wattron(cad_cli, A_BOLD | COLOR_PAIR(4)); // COR VERDE NO NOME CLIENTE CADASTRADO
        mvwprintw(cad_cli, 7, (width - strlen("Cliente cadastrado!")) / 2, "Cliente cadastrado!");
        wattroff(cad_cli, A_BOLD | COLOR_PAIR(4));

        wrefresh(cad_cli);

        wgetch(cad_cli); // Aguarda o usuário pressionar uma tecla para voltar para a pagina principal
        delwin(cad_cli);
        pag_principal();
    } else {
        // Limite de clientes atingido
        mvprintw(LINES / 2, (COLS - strlen("Limite de clientes atingido.")) / 2, "Limite de clientes atingido.");
        refresh();
        getch();
        pag_principal();
    }
}

void atualizar_estoque() {
    WINDOW *at_estoq;
    int startx, starty, width, height;
    char prod[MAX_CARACTER];
    char opc_desejada;
    char qtd_str[10];
    int qtd;
    int i;

    height = HEIGHT;
    width = WIDTH;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    at_estoq = newwin(height, width, starty, startx);
    wbkgd(at_estoq, COLOR_PAIR(1)); // Fundo branco
    draw_rounded_box(at_estoq, 0, 0, height, width);
    refresh();

    wattron(at_estoq, A_BOLD | COLOR_PAIR(1));
    print_large_text(at_estoq, 2, 0, "ATUALIZAR ESTOQUE");
    wattroff(at_estoq, A_BOLD | COLOR_PAIR(1));

    // Entrada do nome do produto
    mvwprintw(at_estoq, 5, 10, "Nome do Produto: ");
    echo();
    wgetnstr(at_estoq, prod, sizeof(prod) - 1);
    noecho();

    // Escolha da operação
    mvwprintw(at_estoq, 7, (WIDTH/2)-16, "1 - Adicionar      2 - Remover");
    mvwprintw(at_estoq, 8, (WIDTH/2)-4, "Enter: ");
    echo();
    opc_desejada = wgetch(at_estoq);
    noecho();

    // Entrada da quantidade
    switch(opc_desejada) {
        case '1':
            mvwprintw(at_estoq, 10, 10, "Quantidade a ser adicionada: ");
            echo();
            wgetnstr(at_estoq, qtd_str, sizeof(qtd_str) - 1);
            noecho();
            qtd = atoi(qtd_str);
            if (qtd <= 0) {
                wattron(at_estoq, A_BOLD | COLOR_PAIR(2));
                mvwprintw(at_estoq, 12, (WIDTH/2)-17, "A quantidade deve ser positiva!");
                wattroff(at_estoq, A_BOLD | COLOR_PAIR(2));
                wrefresh(at_estoq);
                wgetch(at_estoq);
                delwin(at_estoq);
                pag_principal();
                return;
            }

            for (i = 0; i < numProdutos; i++) {
                if (strcmp(produtos[i].nome, prod) == 0) {
                    produtos[i].quantidade += qtd;
                    wattron(at_estoq, A_BOLD | COLOR_PAIR(4));
                    mvwprintw(at_estoq, 12, (WIDTH/2)-18, "Quantidade de %s atualizada para %d.", produtos[i].nome, produtos[i].quantidade);
                    wattroff(at_estoq, A_BOLD | COLOR_PAIR(4));
                    wrefresh(at_estoq);
                    wgetch(at_estoq);
                    delwin(at_estoq);
                    pag_principal();
                    return;
                }
            }
            wattron(at_estoq, A_BOLD | COLOR_PAIR(2));
            mvwprintw(at_estoq, 12, (WIDTH/2)-18, "Produto de nome %s não encontrado.", prod);
            wattroff(at_estoq, A_BOLD | COLOR_PAIR(2));
            wrefresh(at_estoq);
            wgetch(at_estoq);
            break;

        case '2':
            mvwprintw(at_estoq, 10, 10, "Quantidade a ser removida: ");
            echo();
            wgetnstr(at_estoq, qtd_str, sizeof(qtd_str) - 1);
            noecho();
            qtd = atoi(qtd_str);
            if (qtd <= 0) {
                wattron(at_estoq, A_BOLD | COLOR_PAIR(2));
                mvwprintw(at_estoq, 12, (WIDTH/2)-17, "A quantidade deve ser positiva!");
                wattroff(at_estoq, A_BOLD | COLOR_PAIR(2));
                wrefresh(at_estoq);
                wgetch(at_estoq);
                delwin(at_estoq);
                pag_principal();
                return;
            }

            for (i = 0; i < numProdutos; i++) {
                if (strcmp(produtos[i].nome, prod) == 0) {
                    produtos[i].quantidade -= qtd;
                    wattron(at_estoq, A_BOLD | COLOR_PAIR(4));
                    mvwprintw(at_estoq, 12, (WIDTH/2)-18, "Quantidade de %s atualizada para %d.", produtos[i].nome, produtos[i].quantidade);
                    wattroff(at_estoq, A_BOLD | COLOR_PAIR(4));
                    wrefresh(at_estoq);
                    wgetch(at_estoq);
                    delwin(at_estoq);
                    pag_principal();
                    return;
                }
            }
            wattron(at_estoq, A_BOLD | COLOR_PAIR(2));
            mvwprintw(at_estoq, 12, 10, "Produto de nome %s nao encontrado.", prod);
            wattroff(at_estoq, A_BOLD | COLOR_PAIR(2));
            wrefresh(at_estoq);
            wgetch(at_estoq);
            break;

        default:
            wattron(at_estoq, A_BOLD | COLOR_PAIR(2));
            mvwprintw(at_estoq, 12, 10, "Opção inválida.");
            wattroff(at_estoq, A_BOLD | COLOR_PAIR(2));
            wrefresh(at_estoq);
            wgetch(at_estoq);
            break;
    }

    delwin(at_estoq);
    pag_principal();
}

void deletar_produto() {
    WINDOW *del_pro;
    int startx, starty, width, height;
    char prod[MAX_CARACTER];

    height = HEIGHT;
    width = WIDTH;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    del_pro = newwin(height, width, starty, startx);
    wbkgd(del_pro, COLOR_PAIR(1)); // Fundo branco
    draw_rounded_box(del_pro, 0, 0, height, width);
    refresh();

    wattron(del_pro, A_BOLD | COLOR_PAIR(1));
    print_large_text(del_pro, 2, 0, "DELETAR PRODUTO");
    wattroff(del_pro, A_BOLD | COLOR_PAIR(1));

    // Nome do produto a ser deletado
    mvwprintw(del_pro, 5, 10, "Nome do Produto: ");
    echo();
    wgetnstr(del_pro, prod, sizeof(prod) - 1);
    noecho();

    // Verificar se o produto existe no estoque
    bool produto_existe = false;
    int indice = -1;
    for (int i = 0; i < numProdutos; i++) {
        if (strcmp(produtos[i].nome, prod) == 0) {
            produto_existe = true;
            indice = i;
            break;
        }
    }

    if (produto_existe) {
        // Remover o produto do estoque e ajustar a lista de produtos
        for (int i = indice; i < numProdutos - 1; i++) {
            produtos[i] = produtos[i + 1];
        }
        numProdutos--;

        wattron(del_pro, A_BOLD | COLOR_PAIR(4)); // COR VERDE NO NOME PRODUTO DELETADO
        mvwprintw(del_pro, 7, (width - strlen("Produto Deletado")) / 2, "Produto Deletado");
        wattroff(del_pro, A_BOLD | COLOR_PAIR(4));
    } else {
        wattron(del_pro, A_BOLD | COLOR_PAIR(2)); // COR VERMELHA PARA ERRO
        mvwprintw(del_pro, 7, (width - strlen("Produto nao encontrado")) / 2, "Produto nao encontrado");
        wattroff(del_pro, A_BOLD | COLOR_PAIR(2));
    }

    wrefresh(del_pro);

    wgetch(del_pro); // Aguarda o usuário pressionar uma tecla para voltar para a página principal
    delwin(del_pro);
    pag_principal();
}

void realizar_venda() {
    WINDOW *rea_ven;
    int startx, starty, width, height;
    char nome_pro[MAX_CARACTER], nome_cli[MAX_CARACTER], qtd[MAX_CARACTER];
    char buffer[MAX_CARACTER], compara[MAX_CARACTER];
    int qtd_compras, quantidade, i, x, z;
    int achou = 0;


    height = HEIGHT;
    width = WIDTH;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    rea_ven = newwin(height, width, starty, startx);
    wbkgd(rea_ven, COLOR_PAIR(1)); // Fundo branco
    draw_rounded_box(rea_ven, 0, 0, height, width);
    refresh();

    wattron(rea_ven, A_BOLD | COLOR_PAIR(1));
    print_large_text(rea_ven, 2, 0, "REALIZAR VENDA");
    wattroff(rea_ven, A_BOLD | COLOR_PAIR(1));

    // Entrada do nome do cliente
    mvwprintw(rea_ven, 4, 10, "Nome do Cliente: ");
    echo();
    wgetnstr(rea_ven, nome_cli, sizeof(nome_cli) - 1);
    noecho();

    // Converte o nome do cliente para minúsculas para comparação
    for (i = 0; nome_cli[i] != '\0'; i++) {
        nome_cli[i] = tolower(nome_cli[i]);
    }

    // Verifica se o cliente existe
    for (i = 0; i < numClientes; i++) {
        strcpy(compara, clientes[i].nome);
        for (x = 0; compara[x] != '\0'; x++) {
            compara[x] = tolower(compara[x]);
        }
        if (strcmp(compara, nome_cli) == 0) {
            achou = 1;
            break;
        }
    }

    if (achou == 0) {
        wattron(rea_ven, A_BOLD | COLOR_PAIR(2));
        mvwprintw(rea_ven, 7, (width - strlen("Cliente nao encontrado")) / 2, "Cliente nao encontrado");
        wattroff(rea_ven, A_BOLD | COLOR_PAIR(2));
        wrefresh(rea_ven);
        wgetch(rea_ven);
        delwin(rea_ven);
        pag_principal();
        return;
    }

    nome_cli[0] = toupper(nome_cli[0]);
    achou = 0;
    strcpy(carrinho.nomecliente, nome_cli);

    // Entrada da quantidade de produtos que deseja comprar
    mvwprintw(rea_ven, 5, 10, "Produtos que deseja comprar: ");
    echo();
    wgetnstr(rea_ven, qtd, sizeof(qtd) - 1);
    noecho();
    qtd_compras = atoi(qtd);

    if (qtd_compras > MAX_COMPRAS) {
        wattron(rea_ven, A_BOLD | COLOR_PAIR(2));
        mvwprintw(rea_ven, 8, (width - strlen("Limite de produtos por compra excedido")) / 2, "Limite de produtos por compra excedido");
        wattroff(rea_ven, A_BOLD | COLOR_PAIR(2));
        wrefresh(rea_ven);
        wgetch(rea_ven);
        delwin(rea_ven);
        pag_principal();
        return;
    }

    for (i = 0; i < qtd_compras; i++) {
        achou = 0;
        mvwprintw(rea_ven, 7 , 10, "Nome do %d - produto: ", i + 1);
        echo();
        wgetnstr(rea_ven, nome_pro, sizeof(nome_pro) - 1);
        noecho();

        for (x = 0; nome_pro[x] != '\0'; x++) {
            nome_pro[x] = tolower(nome_pro[x]);
        }

        for (x = 0; x < numProdutos; x++) {
            strcpy(compara, produtos[x].nome);
            for (z = 0; compara[z] != '\0'; z++) {
                compara[z] = tolower(compara[z]);
            }

            if (strcmp(compara, nome_pro) == 0) {
                achou = 1;

                if (produtos[x].quantidade == 0) {
                    wattron(rea_ven, A_BOLD | COLOR_PAIR(2));
                    mvwprintw(rea_ven, 10 , 10, "Nao ha quantidade deste produto no estoque...");
                    wattroff(rea_ven, A_BOLD | COLOR_PAIR(2));
                    wrefresh(rea_ven);
                    wgetch(rea_ven);
                    delwin(rea_ven);
                    pag_principal();
                    return;
                }

                do {
                    wattron(rea_ven, A_BOLD | COLOR_PAIR(4));
                    mvwprintw(rea_ven, 9 , 10, "Quantidade de '%s' em estoque: %d", produtos[x].nome, produtos[x].quantidade);
                    wattroff(rea_ven, A_BOLD | COLOR_PAIR(4));
                    mvwprintw(rea_ven, 10 , 10, "Quantidade de compra do %d - produto: ", i + 1);
                    echo();
                    wgetnstr(rea_ven, qtd, sizeof(qtd) - 1);
                    noecho();
                    quantidade = atoi(qtd);

                } while (!(quantidade > 0 && quantidade <= produtos[x].quantidade));

                produtos[x].quantidade -= quantidade;

                strcpy(carrinho.nomeproduto[numCompras], nome_pro);
                carrinho.quantidade[numCompras] = quantidade;
                carrinho.preco[numCompras] = produtos[x].preco;

                numCompras++;

                wattron(rea_ven, A_BOLD | COLOR_PAIR(4));
                mvwprintw(rea_ven, 12 , (width - strlen("Compra realizada com sucesso!!")) / 2, "Compra realizada com sucesso!!");
                wattroff(rea_ven, A_BOLD | COLOR_PAIR(4));
                break;
            }

        }

        if (!achou) {
            wattron(rea_ven, A_BOLD | COLOR_PAIR(2));
            mvwprintw(rea_ven, 10 + i * 2, (width - strlen("Produto nao encontrado")) / 2, "Produto nao encontrado");
            wattroff(rea_ven, A_BOLD | COLOR_PAIR(2));
            wrefresh(rea_ven);
            wgetch(rea_ven);
            delwin(rea_ven);
            pag_principal();
        }
    }
    carrinho.num_produtos = numCompras;

    wrefresh(rea_ven);
    wgetch(rea_ven);
    delwin(rea_ven);
    pag_principal();
}

void listar_prod() {
    WINDOW *lis;
    int startx, starty, width, height;

    height = HEIGHT;
    width = WIDTH;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    lis = newwin(height, width, starty, startx);
    wbkgd(lis, COLOR_PAIR(1)); // Fundo branco
    draw_rounded_box(lis, 0, 0, height, width);
    refresh();

    wattron(lis, A_BOLD | COLOR_PAIR(1));
    print_large_text(lis, 2, 0, "LISTA DE PRODUTOS");
    wattroff(lis, A_BOLD | COLOR_PAIR(1));

    // Cabeçalho da lista de produtos
    wattron(lis, A_BOLD | COLOR_PAIR(5));
    mvwprintw(lis, 4, 20, "ID   NOME    VALOR   QUANTIDADE");
    wattroff(lis, A_BOLD | COLOR_PAIR(5));

    // Listagem dos produtos
    int i;
    for (i = 0; i < numProdutos; i++) {
        mvwprintw(lis, 6 + i, 20, "%d    %s    %.2f    %d",
                  produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }

    wrefresh(lis);
    wgetch(lis); // Espera o usuário pressionar uma tecla para continuar
    pag_principal();
}

float calcularTotalCarrinho() {
    float total = 0.0;
    for (int i = 0; i < carrinho.num_produtos; i++) {
        total += carrinho.preco[i] * carrinho.quantidade[i];
    }
    return total;
}

void listar_compras() {
    WINDOW *list_c;
    int startx, starty, width, height;
    int i;

    height = HEIGHT;
    width = WIDTH;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    list_c = newwin(height, width, starty, startx);
    wbkgd(list_c, COLOR_PAIR(1)); // Fundo branco
    draw_rounded_box(list_c, 0, 0, height, width);
    refresh();

    wattron(list_c, A_BOLD | COLOR_PAIR(1));
    print_large_text(list_c, 2, 0, "COMPRAS");
    wattroff(list_c, A_BOLD | COLOR_PAIR(1));

    // Exibe o nome do cliente
    wattron(list_c, A_BOLD | COLOR_PAIR(5));
    mvwprintw(list_c, 4, (WIDTH/2)-7, "Cliente: %s", carrinho.nomecliente);
    wattroff(list_c, A_BOLD | COLOR_PAIR(5));

    // Exibe os produtos do carrinho
    for (i = 0; i < carrinho.num_produtos; i++) {
        wattron(list_c, A_BOLD | COLOR_PAIR(6));
        mvwprintw(list_c, 6 , (WIDTH/2)-14, "Produto   Quantidade   Valor");
        wattroff(list_c, A_BOLD | COLOR_PAIR(6));
        mvwprintw(list_c, 7 + i, (WIDTH/2)-14, "%s        %d          %.2f",
                  carrinho.nomeproduto[i], carrinho.quantidade[i], carrinho.preco[i]);

    }

    // Calcula e exibe o total do carrinho
    float total = calcularTotalCarrinho();
    wattron(list_c, A_BOLD | COLOR_PAIR(4));
    mvwprintw(list_c, 7 + carrinho.num_produtos + 2, (WIDTH/2)-11, "Total da Compra: %.2f", total);
    wattroff(list_c, A_BOLD | COLOR_PAIR(4));

    wrefresh(list_c);
    wgetch(list_c); // Espera o usuário pressionar uma tecla para continuar
    delwin(list_c);
    pag_principal();
}


void listar_cliente() {
    WINDOW *lis_c;
    int startx, starty, width, height;
    int i;

    height = HEIGHT;
    width = WIDTH;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    lis_c = newwin(height, width, starty, startx);
    wbkgd(lis_c, COLOR_PAIR(1)); // Fundo branco
    draw_rounded_box(lis_c, 0, 0, height, width);
    refresh();

    wattron(lis_c, A_BOLD | COLOR_PAIR(1));
    print_large_text(lis_c, 2, 0, "LISTA DE CLIENTES");
    wattroff(lis_c, A_BOLD | COLOR_PAIR(1));

    // Cabeçalho da lista de clientes
    wattron(lis_c, A_BOLD | COLOR_PAIR(5));
    mvwprintw(lis_c, 4, (WIDTH/2)-7, "ID   CLIENTE");
    wattroff(lis_c, A_BOLD | COLOR_PAIR(5));

    // Lista os clientes
    for (i = 0; i < numClientes; i++) {
        mvwprintw(lis_c, 6 + i, (WIDTH/2)-7, "%d    %s", clientes[i].id, clientes[i].nome);
    }

    wrefresh(lis_c);
    wgetch(lis_c); // Espera o usuário pressionar uma tecla para continuar
    delwin(lis_c);
    pag_principal();
}
