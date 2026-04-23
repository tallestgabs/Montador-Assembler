#ifndef UTILS_H
#define UTILS_H

#include <string>


// padroniza string em maiuscula
void to_upper(std::string &std);

// remove espacos indesejados (  oi    ) -> (oi)
std::string trim(const std::string &str);

// remove espacos entre argumentos (A, B || A    B) -> (A,B || A B)
std::string clear_spaces(const std::string &str);


#endif