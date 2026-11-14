#include "../include/ui.h"
#include <clocale>
#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

namespace {

const int ANCHO = 58;
const int PAR_TITULO = 1;
const int PAR_SELECCION = 2;
const int PAR_NORMAL = 3;
const int PAR_EXITO = 4;
const int PAR_ERROR = 5;
const int PAR_AYUDA = 6;

void colores() {
  if (!has_colors()) {
    return;
  }

  start_color();
  use_default_colors();
  init_pair(PAR_TITULO, COLOR_CYAN, -1);
  init_pair(PAR_SELECCION, COLOR_BLACK, COLOR_CYAN);
  init_pair(PAR_NORMAL, COLOR_WHITE, -1);
  init_pair(PAR_EXITO, COLOR_GREEN, -1);
  init_pair(PAR_ERROR, COLOR_RED, -1);
  init_pair(PAR_AYUDA, COLOR_YELLOW, -1);
}

void recuadro(int y, int x, int alto, int ancho) {
  mvaddch(y, x, ACS_ULCORNER);
  mvaddch(y, x + ancho - 1, ACS_URCORNER);
  mvaddch(y + alto - 1, x, ACS_LLCORNER);
  mvaddch(y + alto - 1, x + ancho - 1, ACS_LRCORNER);

  for (int i = 1; i < ancho - 1; i++) {
    mvaddch(y, x + i, ACS_HLINE);
    mvaddch(y + alto - 1, x + i, ACS_HLINE);
  }

  for (int i = 1; i < alto - 1; i++) {
    mvaddch(y + i, x, ACS_VLINE);
    mvaddch(y + i, x + ancho - 1, ACS_VLINE);
  }
}

void encabezado(int y, int x, const string &titulo, const string &subtitulo) {
  attron(COLOR_PAIR(PAR_TITULO) | A_BOLD);
  mvprintw(y, x + (ANCHO - 10) / 2, "OVERBANK");
  attroff(COLOR_PAIR(PAR_TITULO) | A_BOLD);

  attron(A_BOLD);
  mvprintw(y + 2, x + (ANCHO - (int)titulo.size()) / 2, "%s", titulo.c_str());
  attroff(A_BOLD);

  if (!subtitulo.empty()) {
    attron(COLOR_PAIR(PAR_AYUDA));
    int sx = x + (ANCHO - (int)subtitulo.size()) / 2;
    if (sx < x + 2) {
      sx = x + 2;
    }
    mvprintw(y + 3, sx, "%.*s", ANCHO - 4, subtitulo.c_str());
    attroff(COLOR_PAIR(PAR_AYUDA));
  }
}

void pie(int y, int x, const char *texto) {
  attron(A_DIM);
  mvprintw(y, x + 2, "%s", texto);
  attroff(A_DIM);
}

bool leerCampo(int y, int x, string &destino, bool oculto, int maxLen) {
  destino.clear();
  curs_set(1);
  keypad(stdscr, TRUE);

  while (true) {
    move(y, x);
    clrtoeol();
    recuadro(y - 1, x - 1, 3, ANCHO - 6);

    string visible = destino;
    if (oculto) {
      visible.assign(destino.size(), '*');
    }

    mvprintw(y, x, "%s", visible.c_str());
    move(y, x + (int)visible.size());
    refresh();

    int tecla = getch();

    if (tecla == 27) {
      curs_set(0);
      return false;
    }

    if (tecla == KEY_BACKSPACE || tecla == 127 || tecla == 8) {
      if (!destino.empty()) {
        destino.pop_back();
      }
      continue;
    }

    if (tecla == 10 || tecla == KEY_ENTER) {
      curs_set(0);
      return true;
    }

    if (tecla >= 32 && tecla <= 126 && (int)destino.size() < maxLen) {
      destino.push_back(static_cast<char>(tecla));
    }
  }
}

} // namespace

void uiIniciar() {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  colores();
}

void uiCerrar() { endwin(); }

int uiMenu(const string &titulo, const string &subtitulo,
           const char *const *opciones, int cantidad) {
  int seleccion = 0;

  while (true) {
    clear();

    int filas, columnas;
    getmaxyx(stdscr, filas, columnas);

    int alto = cantidad + 9;
    int y = (filas - alto) / 2;
    int x = (columnas - ANCHO) / 2;
    if (y < 0) {
      y = 0;
    }
    if (x < 0) {
      x = 0;
    }

    recuadro(y, x, alto, ANCHO);
    encabezado(y + 1, x, titulo, subtitulo);

    for (int i = 0; i < cantidad; i++) {
      int linea = y + 5 + i;
      if (i == seleccion) {
        attron(COLOR_PAIR(PAR_SELECCION) | A_BOLD);
        mvprintw(linea, x + 3, " > %-48s", opciones[i]);
        attroff(COLOR_PAIR(PAR_SELECCION) | A_BOLD);
      } else {
        attron(COLOR_PAIR(PAR_NORMAL));
        mvprintw(linea, x + 3, "   %-48s", opciones[i]);
        attroff(COLOR_PAIR(PAR_NORMAL));
      }
    }

    pie(y + alto - 2, x, "Flechas: navegar   Enter: seleccionar   Esc: atras");
    refresh();

    int tecla = getch();

    switch (tecla) {
    case KEY_UP:
      seleccion = (seleccion - 1 + cantidad) % cantidad;
      break;
    case KEY_DOWN:
      seleccion = (seleccion + 1) % cantidad;
      break;
    case 10:
    case KEY_ENTER:
      return seleccion;
    case 27:
      return cantidad - 1;
    }
  }
}

void uiMostrar(const string &titulo, const vector<string> &lineas) {
  int offset = 0;

  while (true) {
    clear();

    int filas, columnas;
    getmaxyx(stdscr, filas, columnas);

    int alto = filas > 22 ? 22 : filas;
    int visibles = alto - 8;
    if (visibles < 3) {
      visibles = 3;
    }

    int y = (filas - alto) / 2;
    int x = (columnas - ANCHO) / 2;
    if (y < 0) {
      y = 0;
    }
    if (x < 0) {
      x = 0;
    }

    recuadro(y, x, alto, ANCHO);
    encabezado(y + 1, x, titulo, "");

    if (lineas.empty()) {
      mvprintw(y + 6, x + 4, "No hay informacion para mostrar.");
    } else {
      if (offset > (int)lineas.size() - 1) {
        offset = (int)lineas.size() - 1;
      }
      if (offset < 0) {
        offset = 0;
      }

      int mostrar = visibles;
      if (offset + mostrar > (int)lineas.size()) {
        mostrar = (int)lineas.size() - offset;
      }

      for (int i = 0; i < mostrar; i++) {
        mvprintw(y + 5 + i, x + 3, "%.*s", ANCHO - 6, lineas[offset + i].c_str());
      }
    }

    pie(y + alto - 2, x, "Arriba/Abajo: desplazar   Enter o Esc: volver");
    refresh();

    int tecla = getch();
    if (tecla == KEY_UP && offset > 0) {
      offset--;
    } else if (tecla == KEY_DOWN && offset + 1 < (int)lineas.size()) {
      offset++;
    } else if (tecla == 10 || tecla == KEY_ENTER || tecla == 27) {
      return;
    }
  }
}

void uiExito(const string &titulo, const string &mensaje) {
  vector<string> lineas;
  lineas.push_back(mensaje);
  lineas.push_back("");
  lineas.push_back("Operacion completada.");
  uiMostrar(titulo, lineas);
}

void uiError(const string &titulo, const string &mensaje) {
  vector<string> lineas;
  lineas.push_back(mensaje);
  uiMostrar(titulo, lineas);
}

bool uiLeerTexto(const string &titulo, const string &etiqueta, string &destino,
                 bool oculto) {
  clear();

  int filas, columnas;
  getmaxyx(stdscr, filas, columnas);

  int alto = 12;
  int y = (filas - alto) / 2;
  int x = (columnas - ANCHO) / 2;

  recuadro(y, x, alto, ANCHO);
  encabezado(y + 1, x, titulo, etiqueta);
  pie(y + alto - 2, x, "Enter: confirmar   Esc: cancelar");
  refresh();

  return leerCampo(y + 6, x + 4, destino, oculto, 40);
}

bool uiLeerEntero(const string &titulo, const string &etiqueta, int &destino) {
  string texto;
  if (!uiLeerTexto(titulo, etiqueta, texto, false)) {
    return false;
  }

  try {
    size_t pos = 0;
    destino = stoi(texto, &pos);
    return pos == texto.size();
  } catch (...) {
    return false;
  }
}

bool uiLeerMonto(const string &titulo, const string &etiqueta, double &destino) {
  string texto;
  if (!uiLeerTexto(titulo, etiqueta, texto, false)) {
    return false;
  }

  try {
    size_t pos = 0;
    destino = stod(texto, &pos);
    return pos == texto.size();
  } catch (...) {
    return false;
  }
}
