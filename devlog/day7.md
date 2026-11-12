# Day 7
## Fixed Makefile
I realized how much more you need to write yourself using Makefiles 
instead of CMakeLists. Though, I committed myself to the decision,
and added dependency tracking through the Makefile. Embarrassingly,
I accidentally let a `bin/` directory containing the built executable
get on the repo, since it wasn't in the .gitignore. I removed that 
and made so all build files and executable are in `build-folder/<debug | release>`.

## Naming Conventions
I also fixed the inconsistent function naming convention and changed
all of them to PascalCase.
