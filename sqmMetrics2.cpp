#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <map>
#include <cctype>
using namespace std;

// ---------- Helper sets ----------
set<string> dataTypes = {
    "int", "float", "double", "char", "bool", "long", "short", "string", "auto"
};

// ---------- Tokenize a line ----------
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
            if (!isspace(ch))
                tokens.push_back(string(1, ch));
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

// ---------- Main ----------
int main() {
    ifstream file("sample.cpp");
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    vector<string> codeLines;
    string line;
    while (getline(file, line)) {
        codeLines.push_back(line);
    }
    file.close();

    // Tracking
    set<string> declaredVars;
    map<int, set<string>> liveVarsAtLine;  // line number → live variables
    vector<string> variablesUsedLater;

    // ---------- PASS 1: Identify declared and used variables ----------
    for (size_t i = 0; i < codeLines.size(); ++i) {
        vector<string> tokens = tokenize(codeLines[i]);

        for (size_t j = 0; j < tokens.size(); ++j) {
            string token = tokens[j];

            // Declaration
            if (dataTypes.count(token) && j + 1 < tokens.size()) {
                string var = tokens[j + 1];
                if (isalpha(var[0]) || var[0] == '_') {
                    declaredVars.insert(var);
                    liveVarsAtLine[i].insert(var);
                }
            }
            // Usage
            else if (declaredVars.count(token)) {
                liveVarsAtLine[i].insert(token);
            }
        }
    }

    // ---------- PASS 2: Backward analysis (simulate liveness) ----------
    set<string> currentlyLive;
    for (int i = codeLines.size() - 1; i >= 0; --i) {
        // Add variables used in this line
        for (auto &v : liveVarsAtLine[i])
            currentlyLive.insert(v);

        // Store snapshot of live vars
        liveVarsAtLine[i] = currentlyLive;

        // If a variable is declared here, assume it's no longer live before this
        vector<string> tokens = tokenize(codeLines[i]);
        for (size_t j = 0; j < tokens.size(); ++j) {
            string token = tokens[j];
            if (dataTypes.count(token) && j + 1 < tokens.size()) {
                string var = tokens[j + 1];
                currentlyLive.erase(var);
            }
        }
    }

    // ---------- Compute average live variables ----------
    double totalLive = 0;
    for (auto &p : liveVarsAtLine)
        totalLive += p.second.size();

    double avgLive = (codeLines.empty()) ? 0 : totalLive / codeLines.size();

    // ---------- Print results ----------
    cout << "Total Declared Variables: " << declaredVars.size() << endl;
    cout << "Average Live Variables per line: " << avgLive << endl;
    cout << "\nLive variable snapshot:\n";
    for (auto &p : liveVarsAtLine) {
        cout << "Line " << p.first + 1 << ": ";
        for (auto &v : p.second)
            cout << v << " ";
        cout << endl;
    }

    // ---------- Write to CSV ----------
    ofstream csv("live_variables.csv");
    csv << "Line,Live Variables\n";
    for (auto &p : liveVarsAtLine) {
        csv << p.first + 1 << ",";
        for (auto &v : p.second) csv << v << " ";
        csv << "\n";
    }
    csv << "Average Live Variables," << avgLive << "\n";
    csv.close();

    cout << "\nLive variable data written to live_variables.csv ✅" << endl;

    return 0;
}
