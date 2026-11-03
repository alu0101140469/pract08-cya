 /**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramática Regular a partir de un DFA
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file grammar.cc
 * @brief Implementación de la clase Grammar que representa una gramática regular y las producciones.
 * @date 3/11/2026
 * @version 1.0
 */

#include "grammar.h"

#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <utility>

/**
 * @brief Construye una gramática regular.
 * @param terminal_symbols Símbolos terminales de la gramática.
 * @param nonterminal_symbols Símbolos no terminales de la gramática.
 * @param start_symbol Símbolo no terminal de arranque.
 * @param productions Producciones de la gramática.
 */
Grammar::Grammar(std::vector<char> terminal_symbols,
                 std::vector<std::string> nonterminal_symbols,
                 std::string start_symbol,
                 std::vector<Production> productions)
    : terminal_symbols_(std::move(terminal_symbols)),
      nonterminal_symbols_(std::move(nonterminal_symbols)),
      start_symbol_(std::move(start_symbol)),
      productions_(std::move(productions)) {
  // Verifica que no haya símbolos terminales repetidos y que todos sean imprimibles.
  const std::unordered_set<std::string> nonterminal_set(
      nonterminal_symbols_.begin(), nonterminal_symbols_.end());

  // Verifica que no haya símbolos terminales repetidos y que todos sean imprimibles.
  if (nonterminal_set.size() != nonterminal_symbols_.size()) {
    throw std::invalid_argument(
        "La gramática contiene símbolos no terminales repetidos.");
  }
  // Verifica que el símbolo de arranque esté declarado como no terminal.
  if (nonterminal_set.find(start_symbol_) == nonterminal_set.end()) {
    throw std::invalid_argument(
        "El símbolo de arranque no pertenece a los no terminales.");
  }

  // Verifica que todos los símbolos terminales sean imprimibles y no estén repetidos.
  for (const Production& production : productions_) {
    if (nonterminal_set.find(production.left_hand_side) ==
        nonterminal_set.end()) {
      throw std::invalid_argument(
          "La parte izquierda debe ser un no terminal declarado.");
    }
  }
}

/**
 * @brief Escribe la especificación de la gramática en un fichero .gra.
 * @param filename Ruta del fichero de salida.
 * @throws std::runtime_error Si no se puede crear o escribir el fichero.
 */
void Grammar::WriteToFile(const std::string& filename) const {
  // Abre el fichero de salida y verifica que se haya abierto correctamente.
  std::ofstream output_file(filename);
  if (!output_file.is_open()) {
    throw std::runtime_error(
        "No se pudo abrir el fichero de salida '" + filename + "'.");
  }

  // Escribe el alfabeto, los no terminales, el símbolo de arranque y las producciones en el fichero.
  output_file << terminal_symbols_.size() << '\n';
  for (char symbol : terminal_symbols_) {
    output_file << symbol << '\n';
  }

  output_file << nonterminal_symbols_.size() << '\n';
  for (const std::string& symbol : nonterminal_symbols_) {
    output_file << symbol << '\n';
  }

  output_file << start_symbol_ << '\n';
  output_file << productions_.size() << '\n';
  for (const Production& production : productions_) {
    output_file << production.left_hand_side << " -> "
                << production.right_hand_side << '\n';
  }

  // Asegura que los datos se escriban correctamente en el fichero.
  output_file.flush();
  if (!output_file.good()) {
    throw std::runtime_error(
        "Se produjo un error al escribir el fichero '" + filename + "'.");
  }
}