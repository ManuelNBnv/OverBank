#include "../include/storage.h"
#include "../include/users.h"
#include <fstream>
#include <iostream>

using namespace std;

void guardarUsuario(const Usuarios &usuario) {
  ofstream archivo("data/users.txt", ios::app);

  if (!archivo.is_open()) {
    cout << "Error al abrir el archivo\n";
    return;
  }

  archivo << usuario.id << "|" << usuario.nombre << "|" << usuario.contraseña
          << "|" << usuario.saldo << "\n";

  archivo.close();
}
