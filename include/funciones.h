#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "../include/users.h"

void consultarSaldo(Usuarios &usuario);
void depositarDinero(Usuarios &usuario);
void retirarDinero(Usuarios &usuario);
void transferirDinero(Usuarios &usuario);
void verHistorial(Usuarios &usuario);
void mostrarDatosCuenta(Usuarios &usuario);
void cambiarContraseña(Usuarios &usuario);

#endif
