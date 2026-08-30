#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

int main() {

    cout << "1. C++ Identifiers" << endl;

    regex identifier("^[A-Za-z_][A-Za-z0-9_]*$");

    vector<string> identifierTests = {
        "hello",
        "student_1",
        "123abc"
    };

    for (string test : identifierTests) {
        cout << test << " -> "
             << regex_match(test, identifier)
             << endl;
    }


    cout << "\n2. U.S. Phone Numbers" << endl;

    regex phone(
        "^(\\([0-9]{3}\\) [0-9]{3}-[0-9]{4}|[0-9]{3}-[0-9]{3}-[0-9]{4})$"
    );

    vector<string> phoneTests = {
        "(256) 555-1234",
        "256-555-1234",
        "2565551234"
    };

    for (string test : phoneTests) {
        cout << test << " -> "
             << regex_match(test, phone)
             << endl;
    }


    cout << "\n3. Floating Point Numbers" << endl;

    regex floating("^[+-]?[0-9]+(\\.[0-9]+)?$");

    vector<string> floatingTests = {
        "3.14",
        "-42.5",
        "hello"
    };

    for (string test : floatingTests) {
        cout << test << " -> "
             << regex_match(test, floating)
             << endl;
    }

    cout << "\n4. Binary Palindromes" << endl;

    regex palindrome(
        "^([01])([01])\\2\\1$|^([01])([01])\\4$"
    );

    vector<string> palindromeTests = {
        "101",
        "0110",
        "110"
    };

    for (string test : palindromeTests) {
        cout << test << " -> "
             << regex_match(test, palindrome)
             << endl;
    }

    return 0;
}
