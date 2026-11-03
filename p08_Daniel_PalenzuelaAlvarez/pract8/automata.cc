 /**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramática Regular a partir de un DFA
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file automata.cc
 * @brief Implementación de la clase Automata que representa un autómata finito determinista.
 * @date 3/11/2026
 * @version 1.0
 */

#include "automata.h"

#include <charconv>
#include <cctype>
#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {

/**
 * @brief Divide una línea en campos separados por espacios.
 */
std::vector<std::string> SplitLine(const std::string& line) {
  std::istringstream line_stream(line);
  std::vector<std::string> tokens;
  std::string token;

  // Extrae cada token separado por espacios y lo añade al vector.
  while (line_stream >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

/**
 * @brief Lee un entero no negativo y valida todo el contenido del campo.
 */
int ParseNonNegativeInteger(const std::string& token,
                            const std::string& description,
                            std::size_t line_number) {
  int value = 0;
  const char* begin = token.data();
  const char* end = begin + token.size();
  const std::from_chars_result result = std::from_chars(begin, end, value);

  // Verifica que la conversión se cumple, que todo el token fue consumido y que el valor es no negativo.
  if (result.ec != std::errc() || result.ptr != end || value < 0) {
    throw std::runtime_error(
        "Línea " + std::to_string(line_number) + ": " + description +
        " debe ser un entero no negativo.");
  }
  return value;
}

/**
 * @brief Genera los nombres de los no terminales asociados a los estados.
 *
 * El estado inicial recibe el nombre S. El resto recibe nombres alfabéticos
 * (A, B, ..., Z, AA, AB, ...), omitiendo S para evitar colisiones.
 */
std::vector<std::string> BuildNonterminalNames(std::size_t state_count,
                                               int initial_state) {
  // Inicializa el vector de nombres con el tamaño adecuado y asigna "S" al estado inicial.                                              
  std::vector<std::string> names(state_count);
  names[static_cast<std::size_t>(initial_state)] = "S";

  // Genera nombres alfabéticos para los estados restantes, evitando colisiones con S.
  std::size_t next_name_index = 0;
  for (std::size_t state_index = 0; state_index < state_count; ++state_index) {
    if (static_cast<int>(state_index) == initial_state) {
      continue;
    }

    std::string name;
    std::size_t value = next_name_index;
    do {
      name.insert(name.begin(), static_cast<char>('A' + (value % 26)));
      if (value < 26) {
        break;
      }
      value = value / 26 - 1;
    } while (true);

    ++next_name_index;

    // Asegura que el nombre generado no colisione con S.
    if (name == "S") {
      name.clear();
      value = next_name_index;
      do {
        name.insert(name.begin(), static_cast<char>('A' + (value % 26)));
        if (value < 26) {
          break;
        }
        value = value / 26 - 1;
      } while (true);
      ++next_name_index;
    }

    // Asigna el nombre generado al estado correspondiente.
    names[state_index] = name;
  }
  return names;
}

/**
 * @brief Lee y valida la especificación completa de un fichero .dfa.
 */
void ReadAutomatonFile(const std::string& filename,
                       std::vector<char>* alphabet,
                       std::vector<State>* states,
                       int* initial_state) {
  // Abre el fichero de entrada y verifica que se haya abierto correctamente.                      
  std::ifstream input_file(filename);
  if (!input_file.is_open()) {
    throw std::runtime_error(
        "No se pudo abrir el fichero de entrada '" + filename + "'.");
  }

  // Lee la primera línea que contiene el alfabeto y valida cada símbolo.
  std::string line;
  std::size_t line_number = 1;
  if (!std::getline(input_file, line)) {
    throw std::runtime_error("El fichero '.dfa' está vacío.");
  }

  // Divide la línea en tokens y valida que cada token sea un símbolo imprimible único.
  const std::vector<std::string> alphabet_tokens = SplitLine(line);
  std::unordered_set<char> alphabet_set;
  for (const std::string& token : alphabet_tokens) {
    if (token.size() != 1 ||
        std::isprint(static_cast<unsigned char>(token[0])) == 0) {
      throw std::runtime_error(
          "Línea 1: cada símbolo del alfabeto debe ser un carácter "
          "imprimible.");
    }
    // Verifica que el símbolo no sea & y que no esté repetido en el alfabeto.
    if (token[0] == '&') {
      throw std::runtime_error(
          "Línea 1: '&' está reservado para representar la cadena vacía "
          "en la gramática.");
    }
    // Verifica que el símbolo no esté repetido en el alfabeto.
    if (!alphabet_set.insert(token[0]).second) {
      throw std::runtime_error(
          "Línea 1: el alfabeto contiene símbolos repetidos.");
    }
    alphabet->push_back(token[0]);
  }

  // Lee la segunda línea que contiene el número total de estados y valida su formato.
  if (!std::getline(input_file, line)) {
    throw std::runtime_error("Falta la línea 2: número total de estados.");
  }
  ++line_number;
  // Divide la línea en tokens y valida que contenga únicamente un número entero no negativo.
  const std::vector<std::string> state_count_tokens = SplitLine(line);
  if (state_count_tokens.size() != 1) {
    throw std::runtime_error(
        "Línea 2: debe contener únicamente el número total de estados.");
  }
  // Convierte el token a un entero no negativo y valida que sea mayor que cero.
  const int state_count = ParseNonNegativeInteger(
      state_count_tokens[0], "El número de estados", line_number);
  // Verifica que el número de estados sea mayor que cero.  
  if (state_count == 0) {
    throw std::runtime_error(
        "Línea 2: el autómata debe tener al menos un estado.");
  }

  // Lee la tercera línea que contiene el estado de arranque y valida su formato.
  if (!std::getline(input_file, line)) {
    throw std::runtime_error("Falta la línea 3: estado de arranque.");
  }
  ++line_number;
  // Divide la línea en tokens y valida que contenga únicamente un número entero no negativo.
  const std::vector<std::string> initial_state_tokens = SplitLine(line);
  // Convierte el token a un entero no negativo y valida que esté dentro del rango de estados.
  if (initial_state_tokens.size() != 1) {
    throw std::runtime_error(
        "Línea 3: debe contener únicamente el estado de arranque.");
  }
  // Convierte el token a un entero no negativo y valida que esté dentro del rango de estados.
  *initial_state = ParseNonNegativeInteger(
      initial_state_tokens[0], "El estado de arranque", line_number);
  // Verifica que el estado de arranque esté dentro del rango de estados.    
  if (*initial_state >= state_count) {
    throw std::runtime_error(
        "Línea 3: el estado de arranque no pertenece al autómata.");
  }

  // Prepara un vector para almacenar los estados leídos y validados.
  std::vector<std::optional<State>> parsed_states(
      static_cast<std::size_t>(state_count));

  // Lee las siguientes líneas que contienen la definición de cada estado y sus transiciones.    
  for (int state_line = 0; state_line < state_count; ++state_line) {
    if (!std::getline(input_file, line)) {
      throw std::runtime_error(
          "Faltan líneas con la definición de los estados.");
    }
    ++line_number;

    // Divide la línea en tokens y valida que contenga al menos tres campos: identificador, aceptación y número de transiciones.
    const std::vector<std::string> tokens = SplitLine(line);
    if (tokens.size() < 3) {
      throw std::runtime_error("Línea " + std::to_string(line_number) +
                               ": se esperaban identificador, aceptación y "
                               "número de transiciones.");
    }

    // Convierte los tokens a enteros no negativos y valida su rango y consistencia.
    const int state_id = ParseNonNegativeInteger(
        tokens[0], "El identificador del estado", line_number);
    const int accepting_flag = ParseNonNegativeInteger(
        tokens[1], "La marca de aceptación", line_number);
    const int transition_count = ParseNonNegativeInteger(
        tokens[2], "El número de transiciones", line_number);

    // Verifica que el identificador del estado esté dentro del rango de estados y que la marca de aceptación sea 0 o 1.    
    if (state_id >= state_count) {
      throw std::runtime_error(
          "Línea " + std::to_string(line_number) +
          ": el identificador del estado está fuera de rango.");
    }
    if (accepting_flag != 0 && accepting_flag != 1) {
      throw std::runtime_error("Línea " + std::to_string(line_number) +
                               ": la marca de aceptación debe ser 0 o 1.");
    }
    if (parsed_states[static_cast<std::size_t>(state_id)].has_value()) {
      throw std::runtime_error(
          "Línea " + std::to_string(line_number) +
          ": el estado está definido más de una vez.");
    }

    // Verifica que el número de campos coincida con el número de transiciones declaradas.
    const std::size_t transition_field_count = tokens.size() - 3;
    if (transition_field_count % 2 != 0 ||
        transition_field_count / 2 !=
            static_cast<std::size_t>(transition_count)) {
      throw std::runtime_error(
          "Línea " + std::to_string(line_number) +
          ": el número de campos no coincide con las transiciones declaradas.");
    }

    // Crea un estado y añade sus transiciones, validando cada transición.
    State state(state_id, accepting_flag == 1);
    for (int transition_index = 0; transition_index < transition_count;
         ++transition_index) {
      const std::size_t symbol_position =
          3 + 2 * static_cast<std::size_t>(transition_index); // Posición del símbolo de entrada en los tokens.
      const std::string& symbol_token = tokens[symbol_position];
      // Verifica que el símbolo de entrada sea un único carácter y que pertenezca al alfabeto.
      if (symbol_token.size() != 1) {
        throw std::runtime_error(
            "Línea " + std::to_string(line_number) +
            ": cada transición debe utilizar un único símbolo.");
      }
      const char input_symbol = symbol_token[0];
      // Verifica que el símbolo de entrada no sea '&' y que pertenezca al alfabeto definido.
      if (input_symbol == '&') {
        throw std::runtime_error(
            "Línea " + std::to_string(line_number) +
            ": un DFA no admite transiciones épsilon; '&' no debe aparecer "
            "en un fichero .dfa.");
      }
      // Verifica que el símbolo de entrada pertenezca al alfabeto definido.
      if (alphabet_set.find(input_symbol) == alphabet_set.end()) {
        throw std::runtime_error(
            "Línea " + std::to_string(line_number) +
            ": la transición utiliza un símbolo ajeno al alfabeto.");
      }

      // Convierte el token del estado de destino a un entero no negativo y valida que esté dentro del rango de estados.
      const int destination_state = ParseNonNegativeInteger(
          tokens[symbol_position + 1], "El estado de destino", line_number);
      // Verifica que el estado de destino esté dentro del rango de estados.    
      if (destination_state >= state_count) {
        throw std::runtime_error(
            "Línea " + std::to_string(line_number) +
            ": el estado de destino está fuera de rango.");
      }

      // Añade la transición al estado, capturando cualquier excepción y lanzando un error con información de línea si ocurre.
      try {
        state.AddTransition(input_symbol, destination_state);
      } catch (const std::invalid_argument& error) {
        throw std::runtime_error(
            "Línea " + std::to_string(line_number) + ": " + error.what());
      }
    }

    // Almacena el estado leído y validado en el vector de estados, 
    // asegurando que no se sobrescriba un estado previamente definido.
    parsed_states[static_cast<std::size_t>(state_id)] = std::move(state);
  }

  // Verifica que todos los estados hayan sido definidos y los añade al vector de estados final.
  for (std::size_t state_index = 0; state_index < parsed_states.size();
       ++state_index) {
    if (!parsed_states[state_index].has_value()) {
      throw std::runtime_error(
          "No se ha definido el estado " + std::to_string(state_index) + ".");
    }
    states->push_back(std::move(parsed_states[state_index].value()));
  }

  // Verifica que no haya contenido adicional después de la definición de los estados.
  while (std::getline(input_file, line)) {
    ++line_number;
    if (!SplitLine(line).empty()) {
      throw std::runtime_error(
          "Línea " + std::to_string(line_number) +
          ": hay contenido adicional después de los estados.");
    }
  }
}

}

/**
 * @brief Construye un DFA a partir de un fichero de especificación.
 * @param filename Ruta del fichero .dfa.
 * @throws std::runtime_error Si el fichero no existe, está mal formado o no
 *         describe un DFA válido.
 */
Automata::Automata(const std::string& filename) : initial_state_(0) {
  ReadAutomatonFile(filename, &alphabet_, &states_, &initial_state_);
}

/**
 * @brief Convierte el autómata en una gramática regular equivalente.
 * @return Gramática regular que genera el lenguaje reconocido por el DFA.
 */
Grammar Automata::ConvertToGrammar() const {
  const std::vector<std::string> nonterminal_names =
      BuildNonterminalNames(states_.size(), initial_state_);
  std::vector<Production> productions;

  // Genera las producciones de la gramática a partir de los estados y transiciones del DFA.
  for (const State& state : states_) {
    const std::string& source_name =
        nonterminal_names[static_cast<std::size_t>(state.GetId())];

    // Añade producciones para cada transición saliente del estado.    
    for (const Transition& transition : state.GetTransitions()) {
      const std::string& destination_name = nonterminal_names[
          static_cast<std::size_t>(transition.destination_state)];
      const std::string right_hand_side =
          std::string(1, transition.input_symbol) + destination_name;
      productions.push_back({source_name, right_hand_side});
    }

    // Si el estado es de aceptación, añade una producción que derive a la cadena vacía.
    if (state.IsAccepting()) {
      productions.push_back({source_name, "&"});
    }
  }

  // Devuelve la gramática regular construida a partir del DFA.
  return Grammar(alphabet_, nonterminal_names,
                 nonterminal_names[static_cast<std::size_t>(initial_state_)],
                 std::move(productions));
}