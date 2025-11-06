#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <regex>
#include <cmath>
#include <map>



using namespace std;


void writeJSON(map<string, double> metrics) {
    ofstream file("metrics.json");
    file << "{\n";
    int count = 0, size = metrics.size();
    for (auto &m : metrics) {
        file << "  \"" << m.first << "\": " << m.second;
        if (++count < size) file << ",";
        file << "\n";
    }
    file << "}\n";
    file.close();
}


// Operator and Keyword sets
set<string> operators = {
    "+", "-", "*", "/", "%", "++", "--", "=", "+=", "-=", "*=", "/=", "%=",
    "<", "<=", ">", ">=", "==", "!=", "&&", "||", "!", "&", "|", "^", "<<", ">>", "~",
    "if", "else", "for", "while", "return", "switch", "case", "break", "goto", "try",
    "include", "define", "ifdef", "ifndef", "endif", "undef", "continue",
    "default", "do", ";", ",", "?", ":", ".", "->", "::", ".*", "#",
    "sizeof", "typeid", "new", "delete", "catch", "throw", "(", ")", "{", "}"
};

set<string> keywords = {
    "int", "float", "double", "char", "void", "auto", "static", "const",
    "class", "struct", "public", "private", "protected", "using", "namespace",
    "long", "short", "signed", "unsigned", "bool", "true", "false"
};

// Helper functions
bool isNumber(const string &tok) {
    static regex number_rx(R"(^(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)");
    static regex hex_rx(R"(^0[xX][0-9a-fA-F]+$)");
    return regex_match(tok, number_rx) || regex_match(tok, hex_rx);
}

bool isStringLiteral(const string &tok) {
    return (tok.size() >= 2 &&
            ((tok.front() == '"' && tok.back() == '"') ||
             (tok.front() == '\'' && tok.back() == '\'')));
}

bool isIdentifier(const string &tok) {
    if (tok.empty()) return false;
    if (keywords.count(tok)) return false;
    if (operators.count(tok)) return false;
    if (isNumber(tok) || isStringLiteral(tok)) return false;
    return (isalpha(tok[0]) || tok[0] == '_');
}

// Tokenizer
vector<string> tokenize(const string &line) {
    vector<string> tokens;
    string token = "";

    for (char ch : line) {
        if (isalnum(ch) || ch == '_') {
            token += ch;
        } else {
            if (!token.empty()) {
                tokens.push_back(token);
                token = "";
            }
            if (!isspace(ch)) tokens.push_back(string(1, ch));
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

// Main
int main() {
    ifstream file("sample.cpp");
    if (!file.is_open()) {
        cerr << "Error: could not open file.\n";
        return 1;
    }

    set<string> distinctOps, distinctOperands;
    int N1 = 0, N2 = 0;  // total operator & operand occurrences
    string line;

    while (getline(file, line)) {
        vector<string> tokens = tokenize(line);

        for (string &t : tokens) {
            if (operators.count(t) || keywords.count(t)) {
                distinctOps.insert(t);
                N1++;
            } else if (isIdentifier(t) || isNumber(t) || isStringLiteral(t)) {
                distinctOperands.insert(t);
                N2++;
            }
        }
    }
    file.close();

    int n1 = distinctOps.size();
    int n2 = distinctOperands.size();
    double vocabulary = n1 + n2;
    double length = N1 + N2;
    double volume = (vocabulary > 0) ? length * log2(vocabulary) : 0;
    double difficulty = (n2 > 0) ? (n1 / 2.0) * (N2 / (double)n2) : 0;
    double effort = difficulty * volume;

    cout << "n1 = " << n1 << ", n2 = " << n2 << endl;
    cout << "N1 = " << N1 << ", N2 = " << N2 << endl;
    cout << "Volume = " << volume << ", Effort = " << effort << endl;
ofstream csv("metrics.csv");
csv << "Metric,Value\n";
csv << "n1," << n1 << "\n";
csv << "n2," << n2 << "\n";
csv << "N1," << N1 << "\n";
csv << "N2," << N2 << "\n";
csv << "Vocabulary," << vocabulary << "\n";
csv << "Length," << length << "\n";
csv << "Volume," << volume << "\n";
csv << "Difficulty," << difficulty << "\n";
csv << "Effort," << effort << "\n";
csv.close();

    cout << "Metrics written to metrics.csv " << endl;

   map<string, double> metrics = {
    {"Vocabulary", vocabulary},
    {"Length", length},
    {"Volume", volume},
    {"Difficulty", difficulty},
    {"Effort", effort}
};
writeJSON(metrics);


    return 0;
}

