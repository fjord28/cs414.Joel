# Assignment08

## Building and running

Start in the `Assignment08` folder. These commands require a C++17 compiler, CMake, OCaml, and Dune.

```sh
cmake -S cpp -B cpp/build
cmake --build cpp/build
(cd ocaml && dune build)
```

Run the C++ program:

```sh
./cpp/build/count_cpp data/sample.txt red
./cpp/build/count_cpp data/sample.txt red --fail
```

Run the OCaml program:

```sh
(cd ocaml && dune exec ./main.exe -- ../data/sample.txt red)
(cd ocaml && dune exec ./main.exe -- ../data/sample.txt red --fail)
```

Change the path and search string to try the other tests. Use `""` for an empty search string. Errors are printed and the program exits with status 1.

## Part A: C++

### Unsafe example

The unsafe version closes the file when the loop finishes and reaches `fclose`. If an exception happens after opening, it skips that statement and leaves the file open. If opening fails, there is no open file to close. A read error can also stop the loop, but the example does not check for it.

The fixed buffer can split a long line into pieces. This could miss a match between pieces or count the same line more than once.

### How my program works

`FileHandle` opens the file in its constructor and closes it in its destructor. Copying and moving are deleted so another object cannot accidentally own the same file. `get()` lets the counting function use the file without taking ownership. That pointer should not be closed separately.

The line reader builds a string until it reaches a newline or the end of the file. This lets it handle long lines and a last line without a newline. It also checks for reading errors. Each matching line counts once, even if the word appears several times.

### Written answers

1. The destructor runs when the function exits normally. If an exception occurs, it runs during stack unwinding, before the error reaches the caller's handler. If opening fails, the constructor throws and there is no successfully created handle to destroy.
2. This is safer because cleanup happens automatically when the object leaves scope. I do not have to remember a separate close statement for every return or exception.
3. Copying could cause two objects to close the same file. Moving would transfer ownership and leave the old owner empty. A `std::unique_ptr` also allows one owner and safe ownership transfers. A custom deleter would call `fclose` when that owner is destroyed.
4. RAII does not stop the file from being opened or read. It keeps the cleanup tied to the object's lifetime. The file operations still have side effects.

## Part B: OCaml

In the unsafe example, `End_of_file` from `input_line` is caught and returns the count. The program then reaches `close_in`. An exception inside the loop's processing code escapes and skips that close. The exception pattern only handles exceptions from `input_line`, not the branch body. The placeholder also needs a real substring search.

My `with_input_file` helper opens the file and immediately sets up `Fun.protect`. It calls the supplied function and uses `close_in_noerr` in the cleanup callback. On success, it returns the function's result. On failure, it closes the channel and lets the exception continue. `close_in_noerr` avoids replacing the original error with a closing error.

The callback must not return the channel or save it for later because it closes when the protected section ends. OCaml's channel type does not enforce this rule, so the programmer has to follow it.

The counting function rejects an empty search before opening. It uses `input_line` for complete lines and `String.sub` to check for the search string. With the failure option enabled, it raises after processing the first line, even if that line does not match.

`Fun.protect` works like `try ... finally`: cleanup runs after the work finishes or raises an exception. C++ handles this through an object's destructor, while OCaml uses an explicit cleanup function. Garbage collection manages memory, but it does not guarantee that an unused file closes promptly.

## Part C: Test results

Both programs were run with these inputs. All seven tests passed in both languages.

| Test | Input | C++ result | OCaml result | Cleanup in both |
| --- | --- | --- | --- | --- |
| T1 | Sample, `red` | 2 | 2 | Opened and closed |
| T2 | Sample, `orange` | 0 | 0 | Opened and closed |
| T3 | Empty file, `red` | 0 | 0 | Opened and closed |
| T4 | Missing file, `red` | Opening error | Opening error | No file acquired |
| T5 | Sample, `red`, forced failure | Simulated failure | Simulated failure | Closed before reporting error |
| T6 | Sample, empty search | Invalid argument | Invalid argument | Rejected before opening |
| T7 | Long line, `red` | 1 | 1 | Opened and closed |

`data/empty.txt` is empty. `data/long.txt` has 4094 `x` characters, `red`, 1000 `y` characters, and another ` red`. The first match crosses the 4096-character boundary. Both occurrences are on one line, so the answer is 1.

For a normal run, both programs logged acquisition and release and returned the count. For forced failure, both logged acquisition, then release, then the error. Missing files and empty searches produced no acquisition or release messages. These messages show that cleanup code ran, but they do not independently prove that the operating system released the file handle.

### Comparison

Opening, reading, closing, and printing messages have side effects. The count only tells us how many lines matched. It does not describe everything the program did.

RAII encapsulates the effect by keeping ownership and cleanup inside a class. `Fun.protect` keeps cleanup around a function call. Neither removes the effects of file I/O.

In C++, I have to keep one owner and avoid closing its borrowed pointer myself. In OCaml, I have to use the protected helper and keep the channel inside its scope. Both need cleanup that runs reliably. These guarantees apply to normal returns and exceptions, not a process being forcibly stopped.

Ownership decides who closes the file. Exception propagation makes sure the error can still reach the caller after cleanup. This provides **exception safety** for resource cleanup.

Resource cleanup is different from rollback. Closing a file does not undo a line already written or take back a network message. RAII and `Fun.protect` do not automatically reverse those changes. Garbage collection also is not the same as deterministic cleanup because it does not give a dependable time for releasing external resources.

## Part D: Option 2

`with_two_input_files` uses nested calls to `with_input_file`. It opens the first file, then the second, and runs the callback with both channels.

```sh
(cd ocaml && dune exec ./main.exe -- --two ../data/sample.txt ../data/empty.txt)
(cd ocaml && dune exec ./main.exe -- --two ../data/sample.txt ../data/missing.txt)
```

When both open, the second file closes first and the first closes last. The same order applies if the callback raises. If opening the second fails, the first still closes as the error leaves its protected scope. I tested both successful opening and failure on the second file, and both behaved this way. Neither channel should be saved outside the callback.
