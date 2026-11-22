#ifndef TERMINAL_H
#define TERMINAL_H

#include "Executor.h"

#include <string>
#include <vector>

// Main shell: welcome screen, prompt loop, Parser and Executor.
class Terminal {
public:
    void run();

private:
    void showStartupBanner() const;
    void askUserName();
    void showWelcomeBanner() const;
    void printStatusLines() const;

    Executor executor_;
    std::vector<std::string> history_;
    std::string userName_;
};

#endif