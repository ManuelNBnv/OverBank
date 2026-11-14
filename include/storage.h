#ifndef STORAGE_H
#define STORAGE_H

#include "../include/users.h"
#include <string>
#include <vector>

struct Movimiento {
  int idUsuario;
  std::string tipo;
  double monto;
  double saldo;
  std::string detalle;
  int idRelacionado;
  std::string fecha;
};

bool guardarUsuario(const Usuarios &usuario);
bool cargarUsuario(int idBuscado, Usuarios &usuarioEncontrado);
bool actualizarUsuario(const Usuarios &usuario);
int obtenerSiguienteID();

bool guardarMovimiento(const Movimiento &movimiento);
void cargarMovimientos(int idUsuario, std::vector<Movimiento> &movimientos);

#endif
