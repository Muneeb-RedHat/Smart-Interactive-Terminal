#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <utility>
#include <vector>

// Holds one parsed command: its name (first token) and argument tokens.
class Command {
public:
    std::string name;
    std::vector<std::string> args;

    Command() = default;

    Command(std::string cmdName, std::vector<std::string> arguments)
        : name(std::move(cmdName)), args(std::move(arguments)) {}
};

#endif