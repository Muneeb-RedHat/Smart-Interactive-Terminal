#include "Terminal.h"

#include "Parser.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

constexpr const char* kReset = "\033[0m";
constexpr const char* kBorder = "\033[1;33m";    // yellow box frame
constexpr const char* kBoxText = "\033[1;37m"; // bright white inside boxes

// Double-line Unicode box (UTF-8). Borders are colored; inner text is bright white.
void printDoubleBox(const std::vector<std::string>& lines) {
    if (lines.empty()) {
        return;
    }

    std::size_t innerWidth = 0;
    for (const std::string& line : lines) {
        innerWidth = std::max(innerWidth, line.size());
    }
    innerWidth += 4;

    std::cout << kBorder;
    std::cout << "\u2554";
    for (std::size_t i = 0; i < innerWidth; ++i) {
        std::cout << "\u2550";
    }
    std::cout << "\u2557\n";

    for (const std::string& line : lines) {
        const std::size_t pad = innerWidth - line.size();
        const std::size_t left = pad / 2;
        const std::size_t right = pad - left;
        std::cout << kBorder << "\u2551" << kReset << kBoxText << std::string(left, ' ') << line
                  << std::string(right, ' ') << kBorder << "\u2551\n"
                  << kReset;
    }

    std::cout << kBorder;
    std::cout << "\u255a";
    for (std::size_t i = 0; i < innerWidth; ++i) {
        std::cout << "\u2550";
    }
    std::cout << "\u255d\n"
              << kReset;
}

} // namespace

void Terminal::showStartupBanner() const {
    printDoubleBox({
        "Smart Interactive Terminal v1.0",
        "GIKI . OOP . Lab . 2nd . Semester",
    });
}

void Terminal::askUserName() {
    std::cout << "\n\033[1;32mEnter your name:\033[0m " << std::flush;
    if (!std::getline(std::cin, userName_)) {
        userName_.clear();
        return;
    }
    if (userName_.empty()) {
        if (const char* envUser = std::getenv("USER")) {
            userName_ = envUser;
        } else {
            userName_ = "Guest";
        }
    }
}

void Terminal::showWelcomeBanner() const {
    std::cout << '\n';
    printDoubleBox({
        "Smart Interactive Terminal v1.0",
        "OOP Based Console Application",
        "GIKI . Computer Engineering . OOP Lab",
    });
}

void Terminal::printStatusLines() const {
    std::cout << '\n';
    std::cout << "\033[1;32mLogged in as:\033[0m \033[1;37m" << userName_ << '\n';

    std::error_code ec;
    const fs::path cwd = fs::current_path(ec);
    if (!ec) {
        std::cout << "\033[1;32mWorking dir:\033[0m \033[1;33m" << cwd.string() << '\n';
    } else {
        std::cout << "\033[1;32mWorking dir:\033[0m (could not read current path)\n";
    }

    std::cout << "\033[1;36mType help to see all available commands.\033[0m\n\n";
}

void Terminal::run() {
    showStartupBanner();
    askUserName();
    if (userName_.empty()) {
        return;
    }

    showWelcomeBanner();
    printStatusLines();

    std::string line;

    while (true) {
        std::cout << "\033[1;35m[\033[1;33m" << userName_ << "\033[1;35m]@SIT\033[0m \033[1;32m>\033[0m " << std::flush;

        if (!std::getline(std::cin, line)) {
            break;
        }

        if (!line.empty()) {
            history_.push_back(line);
        }

        const Command cmd = Parser::parse(line);
        if (!executor_.execute(cmd, history_)) {
            break;
        }
    }
}