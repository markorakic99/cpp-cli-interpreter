# C++ Command-Line Interpreter (CLI Shell)

A modular, Unix-inspired command-line interpreter implemented in **C++** using Object-Oriented Programming principles. Developed as the capstone project for *Object-Oriented Programming 1* at the School of Electrical Engineering, University of Belgrade.

## Key Features
* **Lexical & Syntax Analysis (`Core/Tokenizer`, `Parser/`):** Tokenizes command lines up to 512 characters, handles quoted string literals, validates syntax, and provides visual error diagnostics (`^` pointers indicating exact positions of unexpected characters).
* **Polymorphic Command Hierarchy (`Commands/`):** Extensible `Command` base class implementing 11 built-in commands:
  * **Text & Stream Processing:** `echo`, `wc` (`-w` words, `-c` characters), `tr` (string replacement/removal), `head` (`-n<count>`).
  * **File Operations:** `touch`, `truncate`, `rm`.
  * **System & Shell Control:** `time`, `date`, `prompt`, `batch` (recursive script execution from file).
* **Pipelines & I/O Redirection (`Core/Interpreter`):** Supports multi-stage command chaining via pipes (`|`), input redirection (`<`), and output redirection (`>` overwrite, `>>` append).
* **Comprehensive Error Handling (`Core/InterpreterErrors`):** Detects and reports lexical, syntactic, semantic (stream conflicts), and OS-level file errors without crashing.

## Project Structure
* `Main.cpp` — Application entry point and interactive REPL loop.
* `Core/` — Tokenizer, interpreter execution engine, pipeline/stream manager, and error reporting.
* `Parser/` — Modular argument and option parsers (`ParserName`, `ParserNameArg`, `ParserOptArg`, `ParserTr`).
* `Commands/` — Abstract `Command` interface and concrete implementations for all 11 commands.

## Build & Run
Compile all `.cpp` files using any standard C++ compiler (e.g., `g++`):
```bash
g++ -std=c++17 Main.cpp Core/*.cpp Parser/*.cpp Commands/*.cpp -o cli_interpreter
./cli_interpreter
```
