#include "Executor.h"

#include <chrono>

// Cross-platform screen clear
#ifdef _WIN32
    #define CLEAR_CMD "cls"
#else
    #define CLEAR_CMD "clear"
#endif

#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace {

std::string rebuildShellLine(const Command& cmd) {
    std::ostringstream oss;
    oss << cmd.name;
    for (const std::string& arg : cmd.args) {
        oss << ' ' << arg;
    }
    return oss.str();
}

std::string joinArgsFrom(const std::vector<std::string>& args, std::size_t startIndex) {
    std::ostringstream oss;
    for (std::size_t i = startIndex; i < args.size(); ++i) {
        if (i > startIndex) {
            oss << ' ';
        }
        oss << args[i];
    }
    return oss.str();
}

void printHelp() {
    constexpr const char* reset = "\033[0m";
    constexpr const char* title = "\033[1;33m";
    constexpr const char* heading = "\033[1;36m";
    constexpr const char* cmd = "\033[1;32m";

    std::cout << title << "--- AVAILABLE COMMANDS ---" << reset << "\n\n";

    std::cout << heading << "File operations:\n" << reset;
    std::cout << cmd << "  show files" << reset << "              List all files in the current directory\n";
    std::cout << cmd << "  create file <n>" << reset << "    Create a new empty file\n";
    std::cout << cmd << "  delete file <n>" << reset << "    Delete a file\n";
    std::cout << cmd << "  rename file <old> <new>\n"
              << reset << "                          Rename a file\n";
    std::cout << cmd << "  write file <n> <text>\n"
              << reset << "                          Write text to a file (replaces contents)\n";
    std::cout << cmd << "  read file <n>" << reset << "      Show a file's contents\n";
    std::cout << cmd << "  file info <n>" << reset << "      Show size, path, and last modified time\n\n";

    std::cout << heading << "Directory operations:\n" << reset;
    std::cout << cmd << "  make dir <n>" << reset << "       Create a new directory\n";
    std::cout << cmd << "  change dir <path>" << reset << "     Change the current directory\n";
    std::cout << cmd << "  go back" << reset << "               Go up one directory (like cd ..)\n";
    std::cout << cmd << "  pwd" << reset << "                   Print the current directory\n\n";

    std::cout << heading << "Terminal:\n" << reset;
    std::cout << cmd << "  history" << reset << "               Show command history\n";
    std::cout << cmd << "  clear history" << reset << "         Clear command history\n";
    std::cout << cmd << "  clear" << reset << "                 Clear the screen\n";
    std::cout << cmd << "  whoami" << reset << "                Show current user (from USER)\n";
    std::cout << cmd << "  help" << reset << "                  Show this help\n";
    std::cout << cmd << "  exit" << reset << "                  Exit the terminal\n\n";

    std::cout << heading << "Shortcuts (same ideas, fewer words):\n" << reset;
    std::cout << cmd << "  show" << reset << "                  Same as \"show files\"\n";
    std::cout << cmd << "  mkfile <n>" << reset << "         Same as \"create file\"\n";
    std::cout << cmd << "  mkfolder <n>" << reset << "       Same as \"make dir\"\n";
    std::cout << cmd << "  delete <n>" << reset << "         Delete a file or folder (not \"delete file\")\n\n";

    std::cout << heading << "Roman Urdu (2 commands):\n" << reset;
    std::cout << cmd << "  band karo" << reset << "               Exit the terminal (shut down)\n";
    std::cout << cmd << "  saf karo" << reset << "                Clear the screen (make it clean)\n\n";

    std::cout << heading << "Pashto roman English (2 commands):\n" << reset;
    std::cout << cmd << "  shata larsha" << reset << "            Same as \"go back\"\n";
    std::cout << cmd << "  daa charta" << reset << "              Same as \"pwd\" (show current folder path)\n\n";

    std::cout << heading << "Saraiki roman English (2 commands):\n" << reset;
    std::cout << cmd << "  vekho sab" << reset << "               Same as \"show files\" (see everything here)\n";
    std::cout << cmd << "  madad" << reset << "                   Same as \"help\"\n\n";

    std::cout << "Any other line is run with system() as a shell command.\n";
}

void runShowFiles() {
    constexpr const char* nameColor = "\033[1;36m";
    constexpr const char* reset = "\033[0m";
    try {
        for (const auto& entry : fs::directory_iterator(fs::current_path())) {
            std::cout << nameColor << entry.path().filename().string() << reset << '\n';
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "show files: " << e.what() << '\n';
    }
}

void printLastWriteTime(const fs::path& p) {
    std::error_code ec;
    const auto ftime = fs::last_write_time(p, ec);
    if (ec) {
        std::cout << "Last modified: (unknown)\n";
        return;
    }
    using namespace std::chrono;
    const auto sctp = time_point_cast<system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + system_clock::now());
    const std::time_t tt = system_clock::to_time_t(sctp);
    std::cout << "Last modified: " << std::ctime(&tt);
}

} // namespace

bool Executor::execute(const Command& cmd, std::vector<std::string>& history) {
    if (cmd.name.empty()) {
        return true;
    }

    // --- Roman Urdu: band karo (exit), saf karo (clear screen) ---
    if (cmd.name == "band" && !cmd.args.empty() && cmd.args[0] == "karo") {
        return false;
    }
    if (cmd.name == "saf" && !cmd.args.empty() && cmd.args[0] == "karo") {
        const int clearStatus = std::system(CLEAR_CMD);
        (void)clearStatus;
        return true;
    }

    // --- Pashto roman: shata larsha (go back), daa charta (pwd) ---
    if (cmd.name == "shata" && !cmd.args.empty() && cmd.args[0] == "larsha") {
        std::error_code ec;
        fs::current_path(fs::current_path() / "..", ec);
        if (ec) {
            std::cerr << "shata larsha: " << ec.message() << '\n';
        }
        return true;
    }
    if (cmd.name == "daa" && !cmd.args.empty() && cmd.args[0] == "charta") {
        std::error_code ec;
        std::cout << fs::absolute(fs::current_path(ec)) << '\n';
        if (ec) {
            std::cerr << "daa charta: " << ec.message() << '\n';
        }
        return true;
    }

    // --- Saraiki roman: vekho sab (show files), madad (help) ---
    if (cmd.name == "vekho" && !cmd.args.empty() && cmd.args[0] == "sab") {
        runShowFiles();
        return true;
    }
    if (cmd.name == "madad") {
        printHelp();
        return true;
    }

    // --- exit ---
    if (cmd.name == "exit") {
        return false;
    }

    // --- help ---
    if (cmd.name == "help") {
        printHelp();
        return true;
    }

    // --- whoami ---
    if (cmd.name == "whoami") {
        const char* user = std::getenv("USER");
        if (!user) user = std::getenv("USERNAME");
        if (user) {
            std::cout << user << '\n';
        } else {
            std::cout << "(user not found)\n";
        }
        return true;
    }

    // --- pwd ---
    if (cmd.name == "pwd") {
        std::error_code ec;
        std::cout << fs::absolute(fs::current_path(ec)) << '\n';
        if (ec) {
            std::cerr << "pwd: " << ec.message() << '\n';
        }
        return true;
    }

    // --- history ---
    if (cmd.name == "history") {
        for (std::size_t i = 0; i < history.size(); ++i) {
            std::cout << (i + 1) << "  " << history[i] << '\n';
        }
        return true;
    }

    // --- clear history (before plain clear) ---
    if (cmd.name == "clear" && !cmd.args.empty() && cmd.args[0] == "history") {
        history.clear();
        std::cout << "Command history cleared.\n";
        return true;
    }

    // --- clear screen ---
    if (cmd.name == "clear") {
        const int clearStatus = std::system(CLEAR_CMD);
        (void)clearStatus;
        return true;
    }

    // --- go back ---
    if (cmd.name == "go" && !cmd.args.empty() && cmd.args[0] == "back") {
        std::error_code ec;
        fs::current_path(fs::current_path() / "..", ec);
        if (ec) {
            std::cerr << "go back: " << ec.message() << '\n';
        }
        return true;
    }

    // --- show files (and shortcut: show) ---
    if (cmd.name == "show") {
        if (cmd.args.empty()) {
            runShowFiles();
            return true;
        }
        if (cmd.args[0] == "files") {
            runShowFiles();
            return true;
        }
    }

    // --- create file ---
    if (cmd.name == "create" && !cmd.args.empty() && cmd.args[0] == "file") {
        if (cmd.args.size() < 2) {
            std::cerr << "create file: missing file name\n";
            return true;
        }
        std::ofstream file(cmd.args[1]);
        if (!file) {
            std::cerr << "create file: could not create \"" << cmd.args[1] << "\"\n";
        }
        return true;
    }

    // --- delete file ---
    if (cmd.name == "delete" && !cmd.args.empty() && cmd.args[0] == "file") {
        if (cmd.args.size() < 2) {
            std::cerr << "delete file: missing file name\n";
            return true;
        }
        std::error_code ec;
        fs::remove(cmd.args[1], ec);
        if (ec) {
            std::cerr << "delete file: " << ec.message() << '\n';
        }
        return true;
    }

    // --- rename file ---
    if (cmd.name == "rename" && !cmd.args.empty() && cmd.args[0] == "file") {
        if (cmd.args.size() < 3) {
            std::cerr << "rename file: need old name and new name\n";
            return true;
        }
        std::error_code ec;
        fs::rename(cmd.args[1], cmd.args[2], ec);
        if (ec) {
            std::cerr << "rename file: " << ec.message() << '\n';
        }
        return true;
    }

    // --- write file <n> <text...> ---
    if (cmd.name == "write" && !cmd.args.empty() && cmd.args[0] == "file") {
        if (cmd.args.size() < 2) {
            std::cerr << "write file: missing file name\n";
            return true;
        }
        const std::string content = joinArgsFrom(cmd.args, 2);
        std::ofstream out(cmd.args[1]);
        if (!out) {
            std::cerr << "write file: could not open \"" << cmd.args[1] << "\"\n";
            return true;
        }
        out << content;
        return true;
    }

    // --- read file ---
    if (cmd.name == "read" && !cmd.args.empty() && cmd.args[0] == "file") {
        if (cmd.args.size() < 2) {
            std::cerr << "read file: missing file name\n";
            return true;
        }
        std::ifstream in(cmd.args[1]);
        if (!in) {
            std::cerr << "read file: could not open \"" << cmd.args[1] << "\"\n";
            return true;
        }
        std::cout << in.rdbuf();
        if (std::cout.good()) {
            std::cout << '\n';
        }
        return true;
    }

    // --- file info ---
    if (cmd.name == "file" && !cmd.args.empty() && cmd.args[0] == "info") {
        if (cmd.args.size() < 2) {
            std::cerr << "file info: missing file name\n";
            return true;
        }
        const fs::path p = cmd.args[1];
        std::error_code ec;
        if (!fs::exists(p, ec)) {
            std::cerr << "file info: path does not exist\n";
            return true;
        }
        std::cout << "Path: " << fs::absolute(p) << '\n';
        if (fs::is_regular_file(p)) {
            std::cout << "Size: " << fs::file_size(p) << " bytes\n";
            printLastWriteTime(p);
        } else if (fs::is_directory(p)) {
            std::cout << "Type: directory\n";
            printLastWriteTime(p);
        } else {
            std::cout << "Type: other\n";
        }
        return true;
    }

    // --- make dir ---
    if (cmd.name == "make" && !cmd.args.empty() && cmd.args[0] == "dir") {
        if (cmd.args.size() < 2) {
            std::cerr << "make dir: missing directory name\n";
            return true;
        }
        std::error_code ec;
        fs::create_directory(cmd.args[1], ec);
        if (ec) {
            std::cerr << "make dir: " << ec.message() << '\n';
        }
        return true;
    }

    // --- change dir ---
    if (cmd.name == "change" && !cmd.args.empty() && cmd.args[0] == "dir") {
        if (cmd.args.size() < 2) {
            std::cerr << "change dir: missing path\n";
            return true;
        }
        std::error_code ec;
        fs::current_path(cmd.args[1], ec);
        if (ec) {
            std::cerr << "change dir: " << ec.message() << '\n';
        }
        return true;
    }

    // --- shortcuts: mkfile, mkfolder, delete (single target) ---
    if (cmd.name == "mkfile") {
        if (cmd.args.empty()) {
            std::cerr << "mkfile: missing file name\n";
            return true;
        }
        std::ofstream file(cmd.args[0]);
        if (!file) {
            std::cerr << "mkfile: could not create \"" << cmd.args[0] << "\"\n";
        }
        return true;
    }

    if (cmd.name == "mkfolder") {
        if (cmd.args.empty()) {
            std::cerr << "mkfolder: missing folder name\n";
            return true;
        }
        std::error_code ec;
        fs::create_directory(cmd.args[0], ec);
        if (ec) {
            std::cerr << "mkfolder: " << ec.message() << '\n';
        }
        return true;
    }

    if (cmd.name == "delete" && cmd.args.size() == 1) {
        std::error_code ec;
        fs::remove_all(cmd.args[0], ec);
        if (ec) {
            std::cerr << "delete: " << ec.message() << '\n';
        }
        return true;
    }

    const std::string line = rebuildShellLine(cmd);
    const int status = std::system(line.c_str());
    (void)status;
    return true;
}