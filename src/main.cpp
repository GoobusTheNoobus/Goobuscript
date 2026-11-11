#include "driver.h"

int main(int argc, char *argv[]) {

    if (argc == 1) { // No extra arguments given
        return driver::Repl();
    }
}