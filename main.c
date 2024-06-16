#include <curses.h>
#include <string.h>
#include <locale.h>

#define WIDTH 70
#define HEIGHT 15

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string); //Função para printar no centro
void pag_login();   //Função para a pagina de login
void pag_principal(); //Função para a pagina principal
void cadastrar_cliente();
void draw_rounded_box(WINDOW *win, int starty, int startx, int height, int width);
void print_large_text(WINDOW *win, int starty, int startx, const char *string); //função para printar um texto longo


int main() {
    setlocale(LC_ALL, "Portuguese");
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);    // COR DO TEXTO BRANCO, COR DE FUNDO PRETO
    init_pair(2, COLOR_RED, COLOR_BLACK);     // Texto vermelho para erros
    init_pair(3, COLOR_BLACK, COLOR_WHITE);    // Texto preto, fundo branco (para campos de entrada)

    bkgd(COLOR_PAIR(1)); // Definir o fundo da janela principal como branco
    cbreak();
    noecho();
    curs_set(0);

    pag_login();

    endwin();
    return 0;
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

    // Lado esquerdo (vendas)
    mvwprintw(pagina_prin, 4, 3, "1 - Cadastrar Cliente");
    mvwprintw(pagina_prin, 5, 3, "2 - Realizar Venda");
    mvwprintw(pagina_prin, 6, 3, "3 - Listar Carrinho");
    mvwprintw(pagina_prin, 7, 3, "4 - Trocar de Usuario");

    // Lado direito (estoque)
    mvwprintw(pagina_prin, 4, width - strlen("5 - Cadastrar vendedor") - 3, "5 - Cadastrar vendedor");
    mvwprintw(pagina_prin, 5, width - strlen("6 - Atualizar Estoque") - 3, "6 - Atualizar Estoque");
    mvwprintw(pagina_prin, 6, width - strlen("7 - Consultar Produtos") - 3, "7 - Consultar Produtos");
    mvwprintw(pagina_prin, 7, width - strlen("8 - Excluir Produto") - 3, "8 - Excluir Produto");

    // Rodapé
    mvwprintw(pagina_prin, 9, (width - strlen("9 - Sair ")) / 2, "9 Sair ");

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
            realizar_venda();
            break;
    }



    wattroff(pagina_prin, A_BOLD | COLOR_PAIR(1));

    wrefresh(pagina_prin);

    // Aguarda a entrada do usuário para fechar
    wgetch(pagina_prin);

    delwin(pagina_prin);
}

void cadastrar_cliente(){


    WINDOW* cad_cli;
    int startx, starty, width, height;

    height = HEIGHT;
    width = WIDTH;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    cad_cli = newwin(height, width, starty, startx);
    wbkgd(cad_cli, COLOR_PAIR(1)); // Fundo branco
    draw_rounded_box(cad_cli, 0, 0, height, width);
    refresh();

    wattron(cad_cli, A_BOLD | COLOR_PAIR(1));
    print_large_text(cad_cli, 2, 0, "CADASTRAR CLIENTE");

    wgetch(cad_cli);
    delwin(cad_cli);
}

void realizar_venda(){
    WINDOW* rea_ven;
    int startx, starty, width, height;

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

    wgetch(rea_ven);
    delwin(rea_ven);
}
