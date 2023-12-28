#include <iostream>
#include <vector>
#include<string>
#include <stack>
using namespace std;
#define Fast ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#define ll long long
#define EPS 1e-6

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    NUMBER,
    DELIMITER,
    UNKNOWN
};
struct Token {
    TokenType type;
    string value;
};
// Lexical Analysis
/*
    Keywords: int, float, if, while
    Identifiers: User-defined names for variables and functions
    Operators: +, -, *, /, >, <, ==, !=
    Numbers: Integers and floats
    Delimiters: (, ), [, ], {, }, ;, ,
*/
class LexicalAnalysis {
private:
    string code;

public:
    LexicalAnalysis() { }
    LexicalAnalysis(string _code) {
        code = _code;
    }
    Token get_next_token() {
        Token token;
        // if the user send an empty string
        if (code.empty()) {
            token.type = UNKNOWN;
            return token;
        }

        // Whitespace
        if (isspace(code[0])) { // int sum =
            code.erase(0, 1);
        }

        char c = code[0];
        code.erase(0, 1);

        // Keywords
        if (isalpha(c)) {
            string identifier; //int sum = 12
            identifier += c;
            while (isalnum(code[0])) {
                identifier += code[0];
                code.erase(0, 1);
            }

            const char* keyword = strstr("int float if while", identifier.c_str()); // sum
            if (keyword) {
                token.type = KEYWORD;
                token.value = identifier; // int
            }
            else {
                token.type = IDENTIFIER;
                token.value = identifier;
            }
        }
        // Numbers
        else if (isdigit(c)) {
            string number; // 12
            number += c;
            while (isdigit(code[0]) || code[0] == '.') {
                number += code[0];
                code.erase(0, 1);
            }
            token.type = NUMBER;
            token.value = number;
        }
        // Operators
        else if (ispunct(c)) {
            switch (c) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '=':
            case '!':
            case '%':
            case '>':
            case '<':
            case '&':
            case '|':
            case '^':
                token.type = OPERATOR;
                token.value += c;
                break;
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case ';':
            case ',':
                token.type = DELIMITER;
                token.value += c;
                break;
            default:
                token.type = UNKNOWN;
                break;
            }
        }

        return token;
    }
};


struct Quadruple {
    string op;
    string arg1;
    string arg2;
    string result;
};

// Intermediate Code Generation
class IntermediateCodeGeneration {
public:
    vector<Quadruple> tac; 
    int tempCount;

    IntermediateCodeGeneration() {
        tempCount = 0;
    }

    void Order(string code) {//5 + 6 * 3 / 9 - 5 ;
        stack<string> s; // 6 * 3
        // if stack is empty
        if (s.empty()) {
            s.push(to_string(code[0] - '0')); // 
        }

        // after adding first digit
        while (s.top()[0] != 't') {
            for (int i = 1; i < code.length(); i++) {
                if (isspace(code[i])) {
                    continue;
                }

                if (code[i] == ';') {
                    while (s.size() > 1) {
                        string arg1 = s.top();
                        s.pop();
                        string op = s.top();
                        s.pop();
                        string arg2 = s.top();
                        s.pop();
                        if (op == "+") {
                            string temp = genArithmetic(op, arg1, arg2);
                            s.push(temp);
                        }
                        else {
                            string temp = genArithmetic(op, arg2, arg1);
                            s.push(temp);
                        }
                    }
                    break;
                }

                if (isdigit(code[i])) {
                    if (s.top() == "*" || s.top() == "/") {
                        string op = s.top(); // * 3 6
                        s.pop();
                        string temp = genArithmetic(op, s.top(), to_string(code[i] - '0'));
                        s.pop();
                        s.push(temp);
                        continue;
                    }
                    else if (s.top() == "+" || s.top() == "-") {
                        s.push(to_string(code[i] - '0'));
                        continue;
                    }
                }
                else {
                    switch (code[i]) {
                    case '+':
                        s.push("+");
                        break;
                    case '-':
                        s.push("-");
                        break;
                    case '*':
                        s.push("*");
                        break;
                    case '/':
                        s.push("/");
                        break;
                    }
                }
            }
        }
        tac.push_back({ "=","result",s.top(),"" });

    }
     // Function to generate a unique temporary variable name
    string genTemp(string ch) {

        return ch + to_string(++tempCount);

    }

    // Function to generate TAC for an arithmetic expression
    string genArithmetic(string op, string arg1, string arg2) {
        string temp;

        temp = genTemp("t"); // t1

        tac.push_back({ op, arg1, arg2, temp }); // * 3 6 t1
        return temp;
    }

};

void solve() {
    cout << "Lexical Analysis:\n";
    string code = "int sum = 9 + 2 * 3; if (sum > 5) { print(sum); }";
    LexicalAnalysis Lex = LexicalAnalysis(code);
    Token token;
    while (token.type != UNKNOWN) {
        token = Lex.get_next_token();
        if (token.type != UNKNOWN) {
            cout << "Token: ";
            if (token.type == 0) cout << "KEYWORD";
            else if (token.type == 1) cout << "IDENTIFIER";
            else if (token.type == 2) cout << "OPERATOR";
            else if (token.type == 3) cout << "NUMBER";
            else if (token.type == 4) cout << "DELIMITER";
            else cout << "UNKNOWN";

            cout << ", Value: " << token.value << endl;
        }
    }

    /* ---------------------------------------------------------------------- */
    cout << "\nIntermediate Code Generation:\n";
    IntermediateCodeGeneration ICG = IntermediateCodeGeneration();
    ICG.Order("5 + 6 * 3 / 9 - 5 ;");

    // Print the generated TAC
    for (auto quad : ICG.tac) {
        cout << quad.arg1 << " " << quad.op << " " << quad.arg2 << " " << quad.result << endl;
    }

}

int main() {
    Fast
        //    freopen("C:\\Users\\hp\\CLionProjects\\untitled\\input.txt", "r", stdin);
        //    freopen("C:\\Users\\hp\\CLionProjects\\untitled\\output.txt", "w", stdout);
        int t = 1;

    while (t--) {
        solve();

    }
    return 0;
}
//int x = 5;
//4 + 6 * 2
//6 * 2 t1 
//4 + t1 t2
//