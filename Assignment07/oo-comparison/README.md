# C++ and Python object-model comparison

The programs use Alex Rivera, a Computer Science major, employee 1042, assigned to CS 240. No extra libraries or dynamic allocation are needed.

## Build and run

Requirements: a C++20 compiler and Python 3 (Python 3.6 or newer for f-strings). From inside `oo-comparison` on Linux/macOS or a shell with GCC:

```sh
mkdir -p build
g++ -std=c++20 -Wall -Wextra -pedantic cpp/nonvirtual_diamond.cpp -o build/nonvirtual_diamond
g++ -std=c++20 -Wall -Wextra -pedantic cpp/virtual_diamond.cpp -o build/virtual_diamond
./build/nonvirtual_diamond
./build/virtual_diamond
python3 python/cooperative_diamond.py
```

On Windows with GCC, use `.exe` output names and run them from PowerShell with `./build/nonvirtual_diamond.exe` and `./build/virtual_diamond.exe`. Use `py -3` instead of `python3` if needed.

## Experiments

- `cpp/nonvirtual_diamond.cpp`: two Person subobjects, separate names, and commented invalid expressions.
- `cpp/virtual_diamond.cpp`: one shared Person, virtual dispatch, and unrelated-type tests.
- `python/cooperative_diamond.py`: cooperative initialization, MRO, shared instance state, reversed bases, and duck typing. It first omits the TA override, then adds it to the class at runtime so both results appear in one run.
- `report.md`: explanations, selected observed output, and actual GCC diagnostics.

To reproduce a diagnostic, uncomment only the indicated invalid line and compile that file again. For the final-overrider error, remove only TeachingAssistant's `role()` definition in the virtual program. Restore working code after each experiment. The optional `announce_generic(42)` line demonstrates a missing operation at compile time.

Addresses and Python IDs vary between runs. Compare equality within one run, rather than exact numbers.

## Repository submission

Copy this entire `oo-comparison` folder into your GitHub repository, then commit and push it. The submitted source files have all intentionally rejected expressions commented out. Build executables do not need to be committed.
