 /**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramática Regular a partir de un DFA
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file grammar.h
 * @brief Declaración de la clase Grammar que representa una gramática regular y las producciones.
 * @date 3/11/2026
 * @version 1.0
 */

#ifndef DFA2GRAMMAR_GRAMMAR_H_
#define DFA2GRAMMAR_GRAMMAR_H_

#include <string>
#include <vector>

/**
 * @brief Representa una producción de una gramática regular.
 */
struct Production {
  std::string left_hand_side; // Símbolo no terminal situado a la izquierda.
  std::string right_hand_side; // Parte derecha, & representa la cadena vacía.
};

/**
 * @class Grammar
 * @brief Almacena una gramática regular y permite escribir su especificación.
 *
 * La clase es responsable de conservar el alfabeto, los no terminales, el
 * símbolo de arranque y las producciones, así como de serializarlos al formato
 * de fichero .gra.
 */
class Grammar {
 public:
  /**
   * @brief Construye una gramática regular.
   * @param terminal_symbols Símbolos terminales de la gramática.
   * @param nonterminal_symbols Símbolos no terminales de la gramática.
   * @param start_symbol Símbolo no terminal de arranque.
   * @param productions Producciones de la gramática.
   */
  Grammar(std::vector<char> terminal_symbols,
          std::vector<std::string> nonterminal_symbols,
          std::string start_symbol,
          std::vector<Production> productions);

  /**
   * @brief Escribe la especificación de la gramática en un fichero .gra.
   * @param filename Ruta del fichero de salida.
   * @throws std::runtime_error Si no se puede crear o escribir el fichero.
   */
  void WriteToFile(const std::string& filename) const;

 private:
  std::vector<char> terminal_symbols_; // Alfabeto de símbolos terminales.
  std::vector<std::string> nonterminal_symbols_; // Símbolos no terminales.
  /// Símbolo no terminal desde el que comienza la derivación.
  std::string start_symbol_;
  std::vector<Production> productions_; // Producciones de la gramática.
};

#endif