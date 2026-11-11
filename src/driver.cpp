#include "driver.h"
#include "ast.h"
#include "core.h"
#include "diagnostic.h"
#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include "value.h"
#include <iostream>
#include <string>

namespace driver {

int Repl() {

    std::cout << "Goobuscript interactive console\n";
    std::cout << "Type :quit to exit this shell.\n\n";

    while (true) {
        diagnostic::has_error = false; // reset diagnostic state
        std::cout << "> ";
        std::string input;

        if (!std::getline(std::cin, input)) {
            return 1;
        }

        StringStrip(&input);

        if (input == ":quit") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        parse::Lexer lexer(input);
        auto tokens = lexer.Tokenize();

        if (diagnostic::has_error) {
            continue;
        }

        parse::Parser parser(tokens);
        SyntaxTree tree = parser.Parse();

        if (diagnostic::has_error) {
            continue;
        }

        Evaluator evaluator;
        Value last_value = evaluator.Run(tree);

        if (last_value && last_value.type != Type::Void) {
            std::cout << last_value.ToString() << std::endl;
        }
    }

    return 0;
}

} // namespace driver