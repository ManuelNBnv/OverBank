#include "../include/storage.h"
#include "../include/users.h"
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

const char *ARCHIVO_USUARIOS = "data/users.txt";
const char *ARCHIVO_USUARIOS_TMP = "data/users.tmp";
const char *ARCHIVO_MOVIMIENTOS = "data/movimientos.txt";

string fechaActual() {
  time_t ahora = time(nullptr);
  tm *local = localtime(&ahora);
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", local);
  return string(buffer);
}

string sanitizar(const string &texto) {
  string limpio = texto;
  for (size_t i = 0; i < limpio.size(); i++) {
    if (limpio[i] == '|') {
      limpio[i] = ' ';
    }
  }
  return limpio;
}

void escribirUsuario(ostream &archivo, const Usuarios &usuario) {
  archivo << usuario.id << "|" << sanitizar(usuario.nombre) << "|"
          << usuario.contraseña << "|" << fixed << setprecision(2)
          << usuario.saldo << "\n";
}

bool parsearUsuario(const string &linea, Usuarios &usuario) {
  stringstream registro(linea);
  string idTexto;
  string nombre;
  string pin;
  string saldoTexto;

  if (!getline(registro, idTexto, '|') || !getline(registro, nombre, '|') ||
      !getline(registro, pin, '|') || !getline(registro, saldoTexto)) {
    return false;
  }

  try {
    usuario.id = stoi(idTexto);
    usuario.nombre = nombre;
    usuario.contraseña = pin;
    usuario.saldo = stod(saldoTexto);
    return true;
  } catch (...) {
    return false;
  }
}

} // namespace

bool guardarUsuario(const Usuarios &usuario) {
  ofstream archivo(ARCHIVO_USUARIOS, ios::app);
  if (!archivo.is_open()) {
    return false;
  }

  escribirUsuario(archivo, usuario);
  return true;
}

bool cargarUsuario(int idBuscado, Usuarios &usuarioEncontrado) {
  ifstream archivo(ARCHIVO_USUARIOS);
  if (!archivo.is_open()) {
    return false;
  }

  string linea;
  while (getline(archivo, linea)) {
    if (linea.empty()) {
      continue;
    }

    Usuarios actual;
    if (parsearUsuario(linea, actual) && actual.id == idBuscado) {
      usuarioEncontrado = actual;
      return true;
    }
  }

  return false;
}

bool actualizarUsuario(const Usuarios &usuario) {
  ifstream archivo(ARCHIVO_USUARIOS);
  ofstream temporal(ARCHIVO_USUARIOS_TMP);

  if (!archivo.is_open() || !temporal.is_open()) {
    return false;
  }

  string linea;
  bool actualizado = false;

  while (getline(archivo, linea)) {
    if (linea.empty()) {
      continue;
    }

    Usuarios actual;
    if (parsearUsuario(linea, actual) && actual.id == usuario.id) {
      if (!actualizado) {
        escribirUsuario(temporal, usuario);
        actualizado = true;
      }
    } else {
      temporal << linea << "\n";
    }
  }

  archivo.close();
  temporal.close();

  if (!actualizado) {
    remove(ARCHIVO_USUARIOS_TMP);
    return false;
  }

  return remove(ARCHIVO_USUARIOS) == 0 &&
         rename(ARCHIVO_USUARIOS_TMP, ARCHIVO_USUARIOS) == 0;
}

int obtenerSiguienteID() {
  ifstream archivo(ARCHIVO_USUARIOS);
  if (!archivo.is_open()) {
    return 1000;
  }

  int ultimoID = 999;
  string linea;

  while (getline(archivo, linea)) {
    if (linea.empty()) {
      continue;
    }

    Usuarios actual;
    if (parsearUsuario(linea, actual) && actual.id > ultimoID) {
      ultimoID = actual.id;
    }
  }

  return ultimoID + 1;
}

bool guardarMovimiento(const Movimiento &movimiento) {
  ofstream archivo(ARCHIVO_MOVIMIENTOS, ios::app);
  if (!archivo.is_open()) {
    return false;
  }

  string fecha = movimiento.fecha.empty() ? fechaActual() : movimiento.fecha;

  archivo << movimiento.idUsuario << "|" << sanitizar(movimiento.tipo) << "|"
          << fixed << setprecision(2) << movimiento.monto << "|"
          << movimiento.saldo << "|" << sanitizar(movimiento.detalle) << "|"
          << movimiento.idRelacionado << "|" << fecha << "\n";

  return true;
}

void cargarMovimientos(int idUsuario, vector<Movimiento> &movimientos) {
  movimientos.clear();

  ifstream archivo(ARCHIVO_MOVIMIENTOS);
  if (!archivo.is_open()) {
    return;
  }

  string linea;
  while (getline(archivo, linea)) {
    if (linea.empty()) {
      continue;
    }

    stringstream registro(linea);
    string idTexto;
    string tipo;
    string montoTexto;
    string saldoTexto;
    string detalle;
    string relacionadoTexto;
    string fecha;

    if (!getline(registro, idTexto, '|') || !getline(registro, tipo, '|') ||
        !getline(registro, montoTexto, '|') ||
        !getline(registro, saldoTexto, '|') || !getline(registro, detalle, '|') ||
        !getline(registro, relacionadoTexto, '|') || !getline(registro, fecha)) {
      continue;
    }

    try {
      Movimiento movimiento;
      movimiento.idUsuario = stoi(idTexto);
      if (movimiento.idUsuario != idUsuario) {
        continue;
      }

      movimiento.tipo = tipo;
      movimiento.monto = stod(montoTexto);
      movimiento.saldo = stod(saldoTexto);
      movimiento.detalle = detalle;
      movimiento.idRelacionado = stoi(relacionadoTexto);
      movimiento.fecha = fecha;
      movimientos.push_back(movimiento);
    } catch (...) {
      continue;
    }
  }
}
