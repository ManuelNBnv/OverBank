#ifndef STORAGE_H
#define STORAGE_H

#include "../include/users.h"

void guardarUsuario(const Usuarios &usuario);
bool cargarUsuario(int idBuscado, Usuarios &usuarioEncontrado);
int obtenerSiguienteID();

#endif
