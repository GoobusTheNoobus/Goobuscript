#include "driver.h"

int main(int argc, [[maybe_unused]]char *argv[]) {

    if (argc == 1) { // No extra arguments given
        return driver::Repl();
    }
}