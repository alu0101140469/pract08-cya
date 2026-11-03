 /**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramática Regular a partir de un DFA
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file state.cc
 * @brief Implementación de las transiciones y los estados de un autómata.
 * @date 3/11/2026
 * @version 1.0
 */

#include "state.h"

#include <stdexcept>

/**
 * @brief Construye un estado.
 * @param id Identificador numérico del estado.
 * @param is_accepting Indica si el estado es de aceptación.
 */
State::State(int id, bool is_accepting)
    : id_(id), is_accepting_(is_accepting) {
  if (id < 0) {
    throw std::invalid_argument(
        "El identificador de un estado no puede ser negativo.");
  }
}

/**
 * @brief Devuelve el identificador del estado.
 * @return Identificador numérico.
 */
int State::GetId() const noexcept {
  return id_;
}

/**
 * @brief Indica si el estado es de aceptación.
 * @return true cuando el estado es de aceptación.
 */
bool State::IsAccepting() const noexcept {
  return is_accepting_;
}

/**
 * @brief Devuelve las transiciones salientes del estado.
 * @return Referencia constante al vector de transiciones.
 */
const std::vector<Transition>& State::GetTransitions() const noexcept {
  return transitions_;
}

/**
 * @brief Añade una transición saliente.
 * @param input_symbol Símbolo que activa la transición.
 * @param destination_state Identificador del estado de destino.
 */
void State::AddTransition(char input_symbol, int destination_state) {
  if (destination_state < 0) {
    throw std::invalid_argument("El estado de destino no puede ser negativo.");
  }

  // Verifica que no exista ya una transición con el mismo símbolo de entrada.
  for (const Transition& transition : transitions_) {
    if (transition.input_symbol == input_symbol) {
      throw std::invalid_argument(
          "Un DFA no puede tener dos transiciones con el mismo símbolo "
          "desde un estado.");
    }
  }

  // Añade la nueva transición al vector de transiciones.
  transitions_.push_back({input_symbol, destination_state});
}