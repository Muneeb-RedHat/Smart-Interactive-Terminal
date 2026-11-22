#ifndef PARSER_H
#define PARSER_H

#include "Command.h"
#include <string>

// Turns a raw input line into a Command (name + arguments).
class Parser {
public:
    static Command parse(const std::string& line);
};

#endif