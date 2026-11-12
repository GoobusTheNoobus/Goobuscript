
#include "driver.h"

#include "core.h"
#include "diagnostic.h"
#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include "value.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace driver {

namespace {

int RunSource(const std::string& source, bool print_result = false) {

    diagnostic::has_error = false;

    parse::Lexer lexer(source);
    auto tokens = lexer.Tokenize();

    if (diagnostic::has_error) {
        return 1;
    }

    parse::Parser parser(tokens);
    SyntaxTree tree = parser.Parse();

    if (diagnostic::has_error) {
        return 1;
    }

    Evaluator evaluator;
    Value result = evaluator.Run(tree);

    if (diagnostic::has_error) {
        return 1;
    }

    if (print_result && result && result.type != Type::Void) {
        std::cout << result.ToString() << '\n';
    }

    return 0;
}

int RunFile(const std::string& filename) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "error: Cannot open file '" << filename << "'\n";
        return 1;
    }

    std::string source(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    return RunSource(source);
}

int Repl() {

    std::cout <<
        "============================================\n\n"
        "Goobuscript interactive console " VERSION "\n\n"
        "============================================\n"
        "Type :quit to exit this shell.\n\n";


    while (true) {

        std::cout << "> ";

        std::string input;

        if (!std::getline(std::cin, input)) {
            break;
        }

        StringStrip(&input);

        if (input == ":quit") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        RunSource(input, true);
    }

    return 0;
}


void PrintHelp() {

    std::cout
        << "Goobuscript " VERSION "\n\n"
        << "Usage: gs [options] [file]\n\n"
        << "Options:\n"
        << "  -e, --eval CODE    Evaluate source code\n"
        << "  -v, --version      Print version information\n"
        << "  -h, --help         Display this help message\n\n"
        << "If no file or option is provided, starts the interactive shell.\n";
}

} // namespace

int ParseAndRun(int argc, char* argv[]) {

    if (argc == 1) {
        return Repl();
    }

    std::string argument = argv[1];

    if (argument == "--version" || argument == "-v") {
        std::cout << "Goobuscript " VERSION << '\n';
        return 0;
    }

    if (argument == "--help" || argument == "-h") {
        PrintHelp();
        return 0;
    }

    if (argument == "--eval" || argument == "-e") {

        if (argc < 3) {
            std::cerr << "error: Expected source code after " << argument << '\n';
            return 1;
        }

        return RunSource(argv[2], true);
    }

    if (argument.starts_with("-")) {
        std::cerr << "error: Unknown option '" << argument << "'\n";
        return 1;
    }

    if (argc > 2) {
        std::cerr << "error: Too many arguments\n";
        return 1;
    }

    return RunFile(argument);
}

} // namespace driver
