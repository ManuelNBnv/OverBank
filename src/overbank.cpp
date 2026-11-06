
#include "../include/overbank.h"
#include "../include/funciones.h"
#include <ncurses.h>

void overbank() {
  const char *opciones[] = {"Consultar saldo",          "Depositar dinero",
                            "Retirar dinero",           "Transferir dinero",
                            "Historial de movimientos", "Datos de mi cuenta",
                            "Cambiar contraseña",       "Cerrar sesion"};

  const int cantidadOpciones = 8;
  int seleccion = 0;
  int tecla;

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
  }

  while (true) {
    clear();

    int filas, columnas;
    getmaxyx(stdscr, filas, columnas);

    int inicioY = (filas - 15) / 2;
    int inicioX = (columnas - 32) / 2;

    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(inicioY, inicioX + 5, "======================");
    mvprintw(inicioY + 1, inicioX + 5, "       OVERBANK       ");
    mvprintw(inicioY + 2, inicioX + 5, "======================");
    attroff(COLOR_PAIR(1) | A_BOLD);

    for (int i = 0; i < cantidadOpciones; i++) {
      if (i == seleccion) {
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(inicioY + 4 + i, inicioX + 3, " > %-27s", opciones[i]);
        attroff(COLOR_PAIR(2) | A_BOLD);
      } else {
        attron(COLOR_PAIR(3));
        mvprintw(inicioY + 4 + i, inicioX + 3, "   %-27s", opciones[i]);
        attroff(COLOR_PAIR(3));
      }
    }

    attron(A_DIM);
    mvprintw(inicioY + 13, inicioX + 3,
             "Flechas: navegar | Enter: seleccionar");
    attroff(A_DIM);

    refresh();

    tecla = getch();

    switch (tecla) {
    case KEY_UP:
      seleccion--;
      if (seleccion < 0) {
        seleccion = cantidadOpciones - 1;
      }
      break;

    case KEY_DOWN:
      seleccion++;
      if (seleccion >= cantidadOpciones) {
        seleccion = 0;
      }
      break;

    case 10:
    case KEY_ENTER:
      switch (seleccion) {
      case 0:
        consultarSaldo(usuario);
        break;

      case 1:
        depositarDinero(usuario);
        break;

      case 2:
        retirarDinero(usuario);
        break;

      case 7:
        endwin();
        return;
      }
      break;
    }
  }
}
