#include "../include/users.h"
#include "../include/overbank.h"
#include "../include/storage.h"
#include "../include/ui.h"
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool pinValido(const string &pin) {
  if (pin.size() != 4) {
    return false;
  }

  for (size_t i = 0; i < pin.size(); i++) {
    if (!isdigit(static_cast<unsigned char>(pin[i]))) {
      return false;
    }
  }

  return true;
}

void registrarUsuarios() {
  Usuarios nuevoUsuario;
  nuevoUsuario.saldo = 0;

  if (!uiLeerTexto("REGISTRO", "Nombre completo", nuevoUsuario.nombre) ||
      nuevoUsuario.nombre.empty()) {
    uiError("REGISTRO", "El nombre no puede estar vacio.");
    return;
  }

  if (!uiLeerTexto("REGISTRO", "PIN de 4 digitos", nuevoUsuario.contraseña, true) ||
      !pinValido(nuevoUsuario.contraseña)) {
    uiError("REGISTRO", "El PIN debe tener exactamente 4 digitos.");
    return;
  }

  if (!uiLeerMonto("REGISTRO", "Saldo inicial", nuevoUsuario.saldo) ||
      nuevoUsuario.saldo < 0) {
    uiError("REGISTRO", "El saldo inicial debe ser un numero mayor o igual a 0.");
    return;
  }

  nuevoUsuario.id = obtenerSiguienteID();

  Movimiento apertura;
  apertura.idUsuario = nuevoUsuario.id;
  apertura.tipo = "APERTURA";
  apertura.monto = nuevoUsuario.saldo;
  apertura.saldo = nuevoUsuario.saldo;
  apertura.detalle = "Apertura de cuenta";
  apertura.idRelacionado = 0;

  if (!guardarUsuario(nuevoUsuario) || !guardarMovimiento(apertura)) {
    uiError("REGISTRO", "No se pudo guardar la cuenta.");
    return;
  }

  ostringstream saldoTexto;
  saldoTexto << fixed << setprecision(2) << nuevoUsuario.saldo;

  vector<string> lineas;
  lineas.push_back("Cuenta creada correctamente.");
  lineas.push_back("ID: " + to_string(nuevoUsuario.id));
  lineas.push_back("Nombre: " + nuevoUsuario.nombre);
  lineas.push_back("Saldo: " + saldoTexto.str());
  lineas.push_back("Guarda tu ID. El PIN no se volvera a mostrar.");
  uiMostrar("REGISTRO", lineas);

  overbank(nuevoUsuario);
}

void iniciarSeccion() {
  int idBuscado = 0;
  string pin;
  Usuarios usuarioEncontrado;

  if (!uiLeerEntero("INICIAR SESION", "Ingresa tu ID", idBuscado)) {
    return;
  }

  if (!cargarUsuario(idBuscado, usuarioEncontrado)) {
    uiError("INICIAR SESION", "Usuario no encontrado.");
    return;
  }

  if (!uiLeerTexto("INICIAR SESION", "Ingresa tu PIN", pin, true)) {
    return;
  }

  if (pin != usuarioEncontrado.contraseña) {
    uiError("INICIAR SESION", "PIN incorrecto.");
    return;
  }

  overbank(usuarioEncontrado);
}
