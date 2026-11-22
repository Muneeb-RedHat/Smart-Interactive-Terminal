#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "Command.h"

#include <string>
#include <vector>

// Runs commands: built-in custom handlers first, otherwise system().
class Executor {
public:
    // Returns false when the shell should exit (e.g. "exit").
    // history stores each non-empty line the user typed (for history / clear history).
    bool execute(const Command& cmd, std::vector<std::string>& history);
};

#endif