#include "../include/funciones.h"
#include "../include/storage.h"
#include "../include/ui.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

string dinero(double monto) {
  ostringstream salida;
  salida << fixed;
  salida.precision(2);
  salida << monto;
  return salida.str();
}

bool registrarMovimiento(const Usuarios &usuario, const string &tipo, double monto,
                         const string &detalle, int idRelacionado) {
  Movimiento movimiento;
  movimiento.idUsuario = usuario.id;
  movimiento.tipo = tipo;
  movimiento.monto = monto;
  movimiento.saldo = usuario.saldo;
  movimiento.detalle = detalle;
  movimiento.idRelacionado = idRelacionado;
  return guardarMovimiento(movimiento);
}

} // namespace

void consultarSaldo(Usuarios &usuario) {
  vector<string> lineas;
  lineas.push_back("Titular: " + usuario.nombre);
  lineas.push_back("ID: " + to_string(usuario.id));
  lineas.push_back("Saldo disponible: " + dinero(usuario.saldo));
  uiMostrar("CONSULTAR SALDO", lineas);
}

void depositarDinero(Usuarios &usuario) {
  double monto = 0;

  if (!uiLeerMonto("DEPOSITAR DINERO", "Monto a depositar", monto) || monto <= 0) {
    uiError("DEPOSITAR DINERO", "El monto debe ser un numero mayor a 0.");
    return;
  }

  double anterior = usuario.saldo;
  usuario.saldo += monto;

  if (!actualizarUsuario(usuario) ||
      !registrarMovimiento(usuario, "DEPOSITO", monto, "Deposito en cuenta", 0)) {
    usuario.saldo = anterior;
    actualizarUsuario(usuario);
    uiError("DEPOSITAR DINERO", "No se pudo guardar el deposito.");
    return;
  }

  uiExito("DEPOSITAR DINERO",
          "Deposito de " + dinero(monto) + ". Nuevo saldo: " + dinero(usuario.saldo));
}

void retirarDinero(Usuarios &usuario) {
  double monto = 0;

  if (!uiLeerMonto("RETIRAR DINERO", "Monto a retirar", monto) || monto <= 0) {
    uiError("RETIRAR DINERO", "El monto debe ser un numero mayor a 0.");
    return;
  }

  if (monto > usuario.saldo) {
    uiError("RETIRAR DINERO", "Saldo insuficiente. Disponible: " + dinero(usuario.saldo));
    return;
  }

  double anterior = usuario.saldo;
  usuario.saldo -= monto;

  if (!actualizarUsuario(usuario) ||
      !registrarMovimiento(usuario, "RETIRO", monto, "Retiro en cajero", 0)) {
    usuario.saldo = anterior;
    actualizarUsuario(usuario);
    uiError("RETIRAR DINERO", "No se pudo guardar el retiro.");
    return;
  }

  uiExito("RETIRAR DINERO",
          "Retiro de " + dinero(monto) + ". Nuevo saldo: " + dinero(usuario.saldo));
}

void transferirDinero(Usuarios &usuario) {
  int idDestino = 0;
  double monto = 0;
  Usuarios destino;

  if (!uiLeerEntero("TRANSFERIR DINERO", "ID de la cuenta destino", idDestino)) {
    return;
  }

  if (idDestino == usuario.id) {
    uiError("TRANSFERIR DINERO", "No puedes transferirte a tu propia cuenta.");
    return;
  }

  if (!cargarUsuario(idDestino, destino)) {
    uiError("TRANSFERIR DINERO", "La cuenta destino no existe.");
    return;
  }

  if (!uiLeerMonto("TRANSFERIR DINERO",
                   "Monto para " + destino.nombre + " (ID " + to_string(destino.id) + ")",
                   monto) ||
      monto <= 0) {
    uiError("TRANSFERIR DINERO", "El monto debe ser un numero mayor a 0.");
    return;
  }

  if (monto > usuario.saldo) {
    uiError("TRANSFERIR DINERO",
            "Saldo insuficiente. Disponible: " + dinero(usuario.saldo));
    return;
  }

  double saldoOrigen = usuario.saldo;
  double saldoDestino = destino.saldo;

  usuario.saldo -= monto;
  destino.saldo += monto;

  if (!actualizarUsuario(usuario) || !actualizarUsuario(destino) ||
      !registrarMovimiento(usuario, "TRANSFERENCIA_ENVIADA", monto,
                           "Enviado a " + destino.nombre, destino.id) ||
      !registrarMovimiento(destino, "TRANSFERENCIA_RECIBIDA", monto,
                           "Recibido de " + usuario.nombre, usuario.id)) {
    usuario.saldo = saldoOrigen;
    destino.saldo = saldoDestino;
    actualizarUsuario(usuario);
    actualizarUsuario(destino);
    uiError("TRANSFERIR DINERO", "No se pudo completar la transferencia.");
    return;
  }

  uiExito("TRANSFERIR DINERO",
          "Enviaste " + dinero(monto) + " a " + destino.nombre +
              ". Nuevo saldo: " + dinero(usuario.saldo));
}

void verHistorial(Usuarios &usuario) {
  vector<Movimiento> movimientos;
  cargarMovimientos(usuario.id, movimientos);

  vector<string> lineas;
  if (movimientos.empty()) {
    lineas.push_back("Aun no hay movimientos en esta cuenta.");
  } else {
    for (int i = (int)movimientos.size() - 1; i >= 0; i--) {
      const Movimiento &movimiento = movimientos[i];
      lineas.push_back(movimiento.fecha + "  " + movimiento.tipo);
      lineas.push_back("  " + dinero(movimiento.monto) + " | saldo " +
                       dinero(movimiento.saldo) + " | " + movimiento.detalle);
      lineas.push_back("");
    }
  }

  uiMostrar("HISTORIAL", lineas);
}

void mostrarDatosCuenta(Usuarios &usuario) {
  vector<string> lineas;
  lineas.push_back("ID: " + to_string(usuario.id));
  lineas.push_back("Nombre: " + usuario.nombre);
  lineas.push_back("Saldo: " + dinero(usuario.saldo));
  lineas.push_back("PIN: ****");
  uiMostrar("DATOS DE CUENTA", lineas);
}

void cambiarContraseña(Usuarios &usuario) {
  string actual;
  string nueva;

  if (!uiLeerTexto("CAMBIAR PIN", "PIN actual", actual, true)) {
    return;
  }

  if (actual != usuario.contraseña) {
    uiError("CAMBIAR PIN", "El PIN actual no es correcto.");
    return;
  }

  if (!uiLeerTexto("CAMBIAR PIN", "Nuevo PIN de 4 digitos", nueva, true) ||
      !pinValido(nueva)) {
    uiError("CAMBIAR PIN", "El nuevo PIN debe tener exactamente 4 digitos.");
    return;
  }

  string anterior = usuario.contraseña;
  usuario.contraseña = nueva;

  if (!actualizarUsuario(usuario)) {
    usuario.contraseña = anterior;
    uiError("CAMBIAR PIN", "No se pudo guardar el nuevo PIN.");
    return;
  }

  uiExito("CAMBIAR PIN", "Tu PIN se actualizo correctamente.");
}
