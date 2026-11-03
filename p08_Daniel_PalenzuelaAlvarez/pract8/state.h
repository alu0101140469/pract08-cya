 /**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramática Regular a partir de un DFA
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file state.h
 * @brief Declaración de las transiciones y los estados de un autómata.
 * @date 3/11/2026
 * @version 1.0
 */

#ifndef DFA2GRAMMAR_STATE_H_
#define DFA2GRAMMAR_STATE_H_

#include <vector>

/**
 * @brief Representa una transición de un autómata finito.
 */
struct Transition {
  char input_symbol; // Símbolo del alfabeto que activa la transición.
  int destination_state; // Identificador del estado de destino.
};

/**
 * @class State
 * @brief Modela un estado y las transiciones que parten de él.
 *
 * La clase mantiene encapsulados el identificador, la condición de aceptación
 * y el conjunto de transiciones salientes del estado.
 */
class State {
 public:
  /**
   * @brief Construye un estado.
   * @param id Identificador numérico del estado.
   * @param is_accepting Indica si el estado es de aceptación.
   */
  State(int id, bool is_accepting);

  /**
   * @brief Devuelve el identificador del estado.
   * @return Identificador numérico.
   */
  int GetId() const noexcept;

  /**
   * @brief Indica si el estado es de aceptación.
   * @return true cuando el estado es de aceptación.
   */
  bool IsAccepting() const noexcept;

  /**
   * @brief Devuelve las transiciones salientes del estado.
   * @return Referencia constante al vector de transiciones.
   */
  const std::vector<Transition>& GetTransitions() const noexcept;

  /**
   * @brief Añade una transición saliente.
   * @param input_symbol Símbolo que activa la transición.
   * @param destination_state Identificador del estado de destino.
   * @throws std::invalid_argument Si el destino no es válido o el símbolo ya
   *         tiene una transición desde este estado.
   */
  void AddTransition(char input_symbol, int destination_state);

 private:
  int id_; // Identificador numérico del estado.
  bool is_accepting_; // Indica si el estado es de aceptación.
  std::vector<Transition> transitions_; // Transiciones salientes del estado.
};

#endif