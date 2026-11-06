#include "../include/storage.h"
#include "../include/users.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

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

bool cargarUsuario(int idBuscado, Usuarios &usuarioEncontrado) {

  ifstream archivo("data/users.txt", ios::in);
  string linea;
  string contraseñaIngresada;

  if (!archivo.is_open()) {
    return false;
  }

  while (getline(archivo, linea)) {

    stringstream registro(linea);

    string idTexto;
    string nombre;
    string pin;
    string saldoTexto;

    getline(registro, idTexto, '|');
    getline(registro, nombre, '|');
    getline(registro, pin, '|');
    getline(registro, saldoTexto);

    int id = stoi(idTexto);
    double saldo = stod(saldoTexto);

    usuarioEncontrado.id = id;
    usuarioEncontrado.nombre = nombre;
    usuarioEncontrado.contraseña = pin;
    usuarioEncontrado.saldo = saldo;

    if (id == idBuscado) {
      return true;
    }
  }

  return false;
}

int obtenerSiguienteID() {
  ifstream archivo("data/users.txt");

  if (!archivo.is_open()) {
    return 1000;
  }

  int ultimoID = 999;
  string linea;

  while (getline(archivo, linea)) {
    stringstream registro(linea);

    string idTexto;
    getline(registro, idTexto, '|');

    int id = stoi(idTexto);

    if (id > ultimoID) {
      ultimoID = id;
    }
  }

  return ultimoID + 1;
}
