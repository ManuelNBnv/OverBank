#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <string>

using namespace std;

struct Usuarios {
  int id;
  string nombre;
  string contraseña;
  double saldo;
};

void registrarUsuarios();
void iniciarSeccion();

#endif
