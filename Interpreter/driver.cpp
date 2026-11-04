#include "driver.h"
#include "ast.h"
#include "core.h"
#include "diagnostic.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <string>

namespace driver {

int repl() {
    
    std::cout << "Goobuscript interactive console\n";
    std::cout << "Type :quit to exit this shell.\n\n";

    while (true) {
        diagnostic::has_error = false; // reset diagnostic state
        std::cout << "> ";
        std::string input;
        
        if (!std::getline(std::cin, input)) {
            return 1;
        }

        string_strip(&input);

        if (input == ":quit") {
            break;
        }

        parse::Lexer lexer(input);
        auto tokens = lexer.tokenize();

        if (diagnostic::has_error) {
            continue;
        }

        parse::Parser parser(tokens);
        SyntaxTree tree = parser.parse();

        if (diagnostic::has_error) {
            continue;
        }

        std::cout << tree;
    }

    return 0;
}

}