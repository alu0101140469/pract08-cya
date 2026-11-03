 /**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramática Regular a partir de un DFA
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file automata.h
 * @brief Declaración de la clase Automata que representa un autómata finito determinista.
 * @date 3/11/2026
 * @version 1.0
 */

#ifndef DFA2GRAMMAR_AUTOMATA_H_
#define DFA2GRAMMAR_AUTOMATA_H_

#include <string>
#include <vector>

#include "grammar.h"
#include "state.h"

/**
 * @class Automata
 * @brief Representa un autómata finito determinista leído desde un fichero.
 *
 * La clase valida la especificación .dfa y convierte sus estados y
 * transiciones en los no terminales y producciones de una gramática regular.
 */
class Automata {
 public:
  /**
   * @brief Construye un DFA a partir de un fichero de especificación.
   * @param filename Ruta del fichero .dfa.
   * @throws std::runtime_error Si el fichero no existe, está mal formado o no
   *         describe un DFA válido.
   */
  explicit Automata(const std::string& filename);

  /**
   * @brief Convierte el autómata en una gramática regular equivalente.
   * @return Gramática regular que genera el lenguaje reconocido por el DFA.
   */
  Grammar ConvertToGrammar() const;

 private:
  std::vector<char> alphabet_; // Alfabeto de entrada del autómata.
  /// Estados del autómata ordenados por su identificador numérico.
  std::vector<State> states_;
  int initial_state_; // Identificador del estado de arranque.
};

#endif