#include <iostream>
#include <fstream>
#include <regex>
#include <stack>
#include <string>

using namespace std;
using namespace regex_constants;

bool check_main(const string &code) {
    return regex_search(code, regex(R"((int|void)\s+main\s*\(.*\)\s*\{)"));
};

bool is_matching(char a, char b) {
    return (a == '(' && b == ')') || (a == '{' && b == '}') || (a == '[' && b == ']');
};

bool check_brackets(const string &code) {
    stack<char> s;

    for (const char c : code)
    {
        if (c == '{' || c == '(' || c == '[')
            s.push(c);
        else if (c == '}' || c == ')' || c == ']')
        {
            if (s.empty() || !is_matching(s.top(), c))
                return false;
            s.pop();
        }
    }
    return s.empty();
}

bool check_semicolons(const string &code) {
    return !regex_search(code, regex(R"(^[^\n]*[^\s;{}]\s*$)", ECMAScript | multiline));
}

bool check_loops(const string &code) {
    return !regex_search(code, regex(R"(\b(for|while)\s*[^\s\(])")) && // for !(
           !regex_search(code, regex(R"(\bfor\s*\([^;]*;[^;]*\))")) && // for (;)
           !regex_search(code, regex(R"(\bfor\s*\([^;]*;[^;]*;[^;]*;[^;]*\))")) && // for (;;;)
           !regex_search(code, regex(R"(\b(for|while)\s*\(\s*\))")); // for, ()
}

int main()
{
    string code;
    string line;
    ifstream f("test.cpp");
    while (getline(f, line))
    {
        if (regex_search(line, regex(R"(^\s*//)")) || regex_search(line, regex(R"(^\s*#include)")))
            continue;
        code += line + "\n";
    }

    bool errors = !check_main(code) || !check_brackets(code) || !check_semicolons(code) || !check_loops(code);

    if (errors) {
        check_main(code) ? cout << "" : cerr << "Error: 'main' function not found\n";
        check_brackets(code) ? cout << "" : cerr << "Error: Unmatched braskets\n";
        check_semicolons(code) ? cout << "" : cerr << "Error: Missing semicolon\n";
        check_loops(code) ? cout << "" : cerr << "Error: Invalid loop syntax\n";
    }
    else cout << "No errors found\n";
    return errors ? 1 : 0;
}
