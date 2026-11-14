#ifndef UI_H
#define UI_H

#include <string>
#include <vector>

void uiIniciar();
void uiCerrar();

int uiMenu(const std::string &titulo, const std::string &subtitulo,
           const char *const *opciones, int cantidad);

void uiMostrar(const std::string &titulo, const std::vector<std::string> &lineas);
void uiExito(const std::string &titulo, const std::string &mensaje);
void uiError(const std::string &titulo, const std::string &mensaje);

bool uiLeerTexto(const std::string &titulo, const std::string &etiqueta,
                 std::string &destino, bool oculto = false);
bool uiLeerEntero(const std::string &titulo, const std::string &etiqueta, int &destino);
bool uiLeerMonto(const std::string &titulo, const std::string &etiqueta, double &destino);

#endif
