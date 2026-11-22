# Smart Interactive Terminal (SIT)

Smart Interactive Terminal (SIT) is a beginner-friendly C++17 console application that replaces conventional terminal syntax with readable commands. It provides file and directory operations, command history, colour-coded prompts and output, and selected Roman Urdu, Roman Pashto, and Roman Saraiki commands.

## Project Objectives

- Provide a simple, human-readable interface for common terminal operations.
- Introduce object-oriented design through separate terminal, parser, executor, and command components.
- Support basic file and directory management from one console application.
- Demonstrate cross-platform C++17 programming for Windows and Linux.
- Make command use more accessible through natural-language aliases and multilingual Roman-script commands.

## Features

- File operations: create, delete, rename, write, read, and inspect files.
- Directory operations: create, change, move back, and print directories.
- Command history and history clearing.
- Coloured startup banners, prompts, status information, and command help.
- Built-in Roman Urdu, Roman Pashto, and Roman Saraiki commands.
- Shortcuts for commonly used file and directory operations.
- System-shell fallback for commands not handled by SIT.
- Windows (`cls`) and Linux (`clear`) screen clearing.

## Technologies Used

- C++17
- Standard Template Library
- C++ `<filesystem>` library
- Standard process and file I/O APIs
- ANSI terminal colour and Unicode box-drawing sequences

No third-party libraries or external packages are required.

## Architecture

| Class | Responsibility |
|---|---|
| `Terminal` | Runs the main prompt loop, displays startup and welcome screens, reads the user name, tracks history, and coordinates input handling. |
| `Parser` | Converts each input line into a `Command` containing a command name and argument tokens. |
| `Executor` | Executes built-in commands, handles file and directory operations, and passes unrecognized commands to the operating system shell. |
| `Command` | Stores the command name and its argument list. |

### Execution Flow

1. `Terminal::run()` displays the startup interface and prompt.
2. `Parser::parse()` splits the entered line into a command name and arguments.
3. `Executor::execute()` checks built-in commands and performs the matching operation.
4. Commands not recognized by SIT are passed to `system()` so the host shell can execute them.
5. The loop continues until `exit` or `band karo` ends the application.

## Installation

### Requirements

- A C++17-compatible compiler
- A Windows or Linux terminal

Clone or download the project, then open a terminal in the project directory.

### Compile with GCC or Clang

```sh
g++ main.cpp Terminal.cpp Parser.cpp Executor.cpp -std=c++17 -o sit
```

On Windows, compile with GCC or Clang if one is installed and available on `PATH`.

### Compile with Microsoft Visual C++

Open a Developer Command Prompt for Visual Studio in the project directory and run:

```bat
cl /std:c++17 /EHsc main.cpp Terminal.cpp Parser.cpp Executor.cpp /Fe:sit.exe
```

## Running the Application

After compilation:

```sh
./sit
```

On Windows Command Prompt or PowerShell:

```powershell
.\sit.exe
```

The application asks for a name, then displays a prompt in this format:

```text
[YourName]@SIT >
```

## Usage Examples

### File Operations

```text
show files
create file notes.txt
write file notes.txt Hello from SIT
read file notes.txt
file info notes.txt
rename file notes.txt tasks.txt
delete file tasks.txt
```

### Directory Operations

```text
pwd
make dir projects
change dir projects
go back
```

### Terminal Operations

```text
help
history
clear history
clear
whoami
exit
```

### Shortcuts

```text
show
mkfile sample.txt
mkfolder sample-folder
delete sample.txt
```

The `delete <name>` shortcut removes a file or directory using `std::filesystem::remove_all()`. Use it with care.

## Multilingual Command Examples

| Language | Command | Built-in equivalent |
|---|---|---|
| Roman Urdu | `band karo` | `exit` |
| Roman Urdu | `saf karo` | `clear` |
| Roman Pashto | `shata larsha` | `go back` |
| Roman Pashto | `daa charta` | `pwd` |
| Roman Saraiki | `vekho sab` | `show files` |
| Roman Saraiki | `madad` | `help` |

Command names and arguments are case-sensitive. Any line not recognized as a built-in command is passed to the operating-system shell.

## Team Members and Contributions

| Team Member | Contribution |
|---|---|
| Ahad Sohail | Senior Developer; command structure and project organization. |
| Muneeb Sajid | Executor functionality, application testing, and cross-platform implementation. |
| Duaan Ahmed | Executor functionality. |
| Zainab Khan | Terminal class and terminal interface. |
| Muzammil Shah | Parser class and command parsing. |

## Academic Context

- Course: OOP-112 (Object-Oriented Programming)
- University: GIKI
- Semester: 2nd Semester, 2025
- Project: Smart Interactive Terminal