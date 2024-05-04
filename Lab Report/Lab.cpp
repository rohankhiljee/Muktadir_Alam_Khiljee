#include"bits/stdc++.h"
using namespace std;

vector<pair<int, string>> ans;

bool isValidVariableName(const string& name) {
    if (name.empty() || !isalpha(name[0]) && name[0] != '_') {
        return false;
    }
    for (char c : name) {
        if (!isalnum(c) && c != '_') {
            return false;
        }
    }
    return true;
}

bool isValidFunctionName(const string& name) {
    if (name.empty() || !isalpha(name[0]) && name[0] != '_') {
        return false;
    }
    for (char c : name) {
        if (!isalnum(c) && c != '_' && c != '(' && c != ')') {
            return false;
        }
    }
    return true;
}

bool isAllDigits(const string& str) {
    return all_of(str.begin(), str.end(), ::isdigit);
}

bool containsArithmeticOperator(const string& str) {
    string operators = "+-*/%";

    for (char c : str) {
        if (operators.find(c) != string::npos) {
            return true;
        }
    }

    return false;
}

void process(int lineNo, string s) {
    int left_idx = 0;
    int right_idx = 0;

    s += ' ';

    string temp;

    bool isKeyword = false;

    while (right_idx < s.size() && s[right_idx] == ' ') {
        right_idx++;
    }

    while (right_idx < s.size()) {
        if (s[right_idx] != ' ') {
            temp += s[right_idx];
            right_idx++;
        } else {
            string current_temp = temp;
            temp = "";

            //cout << current_temp << endl;

            while (right_idx < s.size() && s[right_idx] == ' ') {
                right_idx++;
            }

            if (current_temp == "#include") {
                ans.push_back({lineNo, "Header File Declaration."});
                return;
            }

            if (current_temp == "using") {
                ans.push_back({lineNo, "Namespace Declaration."});
                return;
            }

            if (current_temp == "return") {
                ans.push_back({lineNo, "Return Called."});
                return;
            }

            if (current_temp == "int" || current_temp == "char" || current_temp == "string" || current_temp == "float" || current_temp == "double") {
                current_temp += " = Keyword.";
                ans.push_back({lineNo, current_temp});
                isKeyword = true;
                continue;
            }

            if (current_temp == "cin" || current_temp == "cout" || current_temp == "<<" || current_temp == ">>") {
                current_temp += " = built in name.";
                ans.push_back({lineNo, current_temp});
                continue;
            }

            if (current_temp == "if" || current_temp == "else") {
                ans.push_back({lineNo, "Condition Declaration."});
                continue;
            }

            if (current_temp == "{" || current_temp == "}") {
                ans.push_back({lineNo, '"' + current_temp + '"' + " = Bracket Sequence."});
                continue;
            }





            if (current_temp == "=" || current_temp == "==" || current_temp == "!=" ) {
                ans.push_back({lineNo, '"' + current_temp + '"' + " = Equality Operators."});
                continue;
            }

            if (isKeyword) {
                isKeyword = false;
                if (isValidVariableName(current_temp)) {
                    ans.push_back({lineNo, '"' + current_temp + '"' + " = Valid Variable"});
                    continue;
                } else {
                    if (isValidFunctionName(current_temp)) {
                        ans.push_back({lineNo, '"' + current_temp + '"' + " = Valid Function Name"});
                        continue;
                    } else {
                        ans.push_back({lineNo, '"' + current_temp + '"' + " = Error inValid Variable/Function Name"});
                        continue;
                    }
                }
            }

            if (current_temp.size() && current_temp[current_temp.size() - 1] == ';') {
                if (right_idx < s.size()) {
                    ans.push_back({lineNo, '"' + current_temp + '"' + " = Error inValid Punctuation."});
                    continue;
                }

                string varCheck = current_temp;
                varCheck.pop_back();
                if (isValidVariableName(varCheck)) {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Variable"});
                } else if (isAllDigits(varCheck)) {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Constant"});
                } else if (varCheck[varCheck.size() - 1] == '"') {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Constant"});
                } else {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Error inValid Variable/Constant"});
                }

                ans.push_back({lineNo, "';' =  Punctuation"});
                continue;
            }

            if (current_temp.size() && (current_temp[current_temp.size() - 1] == ')' || current_temp[current_temp.size() - 1] == '}')) {

                string varCheck = current_temp;
                varCheck.pop_back();
                if (isValidVariableName(varCheck)) {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Valid Variable"});
                } else if (isAllDigits(varCheck)) {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Valid Constant"});
                } else {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Error inValid Variable/Constant"});
                }

                string aa;
                aa += current_temp[current_temp.size() - 1];

                ans.push_back({lineNo,  '"' + aa + '"' + " = Bracket Sequence"});
                continue;
            }

            if (current_temp.size() && (current_temp[0] == '(' || current_temp[0] == '{')) {

                string varCheck;
                for (int i = 1; i < current_temp.size(); i++)
                    varCheck += current_temp[i];

                string aa;
                aa += current_temp[0];

                ans.push_back({lineNo,  '"' + aa + '"' + " = Bracket Sequence"});

                if (isValidVariableName(varCheck)) {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Valid Variable"});
                } else if (isAllDigits(varCheck)) {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Valid Constant"});
                } else {
                    ans.push_back({lineNo, '"' + varCheck + '"' + " = Error inValid Variable/Constant"});
                }
                continue;
            }


            if (isValidVariableName(current_temp)) {
                ans.push_back({lineNo, '"' + current_temp + '"' + " = Variable"});
                continue;
            } else if (isAllDigits(current_temp)) {
                ans.push_back({lineNo, '"' + current_temp + '"' + " = Constant"});
                continue;
            } else if (containsArithmeticOperator(current_temp)) {
                ans.push_back({lineNo, '"' + current_temp + '"' + " = Arithmetic Operator"});
                continue;
            }

            ans.push_back({lineNo, current_temp + " = Error Occured"});
            continue;

        }

    }

}



int main() {
    ifstream inFile;
    inFile.open("input.txt");

    int cnt = 1;

    if (inFile.is_open()) {
        string input;
        while (getline(inFile, input)) {

            process(cnt, input);
            cnt++;
            //cout << input << endl;
        }
    }

    // for (auto it : ans) {
    //     cout << it.first << ' ' << it.second << endl;
    // }

    inFile.close();

    // Create and open a text file
    ofstream MyFile("myOutput.txt");

    // Write to the file
    for (auto it : ans) {
        MyFile << "Line No: " << it.first << ' ' << it.second << endl;
    }

    // Close the file
    MyFile.close();

}