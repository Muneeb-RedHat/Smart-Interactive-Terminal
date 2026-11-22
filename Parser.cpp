#include "Parser.h"

#include <sstream>

Command Parser::parse(const std::string& line) {
    std::istringstream stream(line);
    std::string token;
    std::vector<std::string> tokens;

    while (stream >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        return Command("", {});
    }

    std::string name = tokens.front();
    tokens.erase(tokens.begin());
    return Command(std::move(name), std::move(tokens));
}