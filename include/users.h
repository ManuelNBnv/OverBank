#ifndef USERS_H
#define USERS_H

#include <string>

struct Usuarios {
  int id;
  std::string nombre;
  std::string contraseña;
  double saldo;
};

bool pinValido(const std::string &pin);
void registrarUsuarios();
void iniciarSeccion();

#endif
