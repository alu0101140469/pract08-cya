 /**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramática Regular a partir de un DFA
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file main.cc
 * @brief Programa principal que convierte un autómata finito determinista (DFA) en una gramática regular equivalente.
 * @date 3/11/2026
 * @version 1.0
 */

  /*
 MODIF: A partir del DFA que reconoce al lenguaje de cadenas sobre el alfabeto {a,b} 
 con longitud par y que comienza y termina con el mismo símbolo, generar la gramática regular equivalente.
 Hacer un README.md con el dfa diseñado, las pruebas de que el dfa funciona, y la gramatica regular generada con el codigo.
 */

#include <exception>
#include <iostream>
#include <string>

#include "automata.h"

namespace {

/**
 * @brief Muestra la sintaxis de ejecución del programa.
 */
void PrintUsage() {
  std::cout << "Modo de empleo: ./dfa2grammar input.dfa output.gra\n"
            << "Pruebe 'dfa2grammar --help' para más información.\n";
}

/**
 * @brief Muestra una explicación breve del funcionamiento del programa.
 */
void PrintHelp() {
  std::cout << "dfa2grammar convierte un autómata finito determinista (DFA) "
               "en una gramática regular equivalente.\n\n"
            << "Uso: ./dfa2grammar input.dfa output.gra\n\n"
            << "El fichero input.dfa contiene el alfabeto, el número de "
               "estados, el estado inicial y las transiciones del DFA.\n"
            << "El fichero output.gra contiene la gramática regular "
               "resultante. Cada estado de aceptación genera una producción "
               "con &, que representa la cadena vacía.\n";
}

}

/**
 * @brief Ejecuta la conversión de un fichero .dfa a un fichero .gra.
 * @param argc Número de argumentos de la línea de comandos.
 * @param argv Argumentos de la línea de comandos.
 * @return 0 si la conversión termina correctamente, un valor distinto de cero
 *         si se solicita ayuda incorrecto o se produce un error.
 */
int main(int argc, char* argv[]) {
  // Muestra la ayuda si se solicita con --help o si el número de argumentos es incorrecto.
  if (argc == 2 && std::string(argv[1]) == "--help") {
    PrintHelp();
    return 0;
  }

  // Verifica que se proporcionen exactamente dos argumentos, el fichero de entrada y el fichero de salida.
  if (argc != 3) {
    PrintUsage();
    return 1;
  }

  try {
    // Crea un objeto Automata a partir del fichero de entrada y convierte el DFA en una gramática regular.
    const Automata automata(argv[1]);
    const Grammar grammar = automata.ConvertToGrammar();
    grammar.WriteToFile(argv[2]);

  } catch (const std::exception& error) {
    // Captura cualquier excepción lanzada durante la lectura del DFA o la escritura 
    // de la gramática y muestra un mensaje de error.
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }

  return 0;
}