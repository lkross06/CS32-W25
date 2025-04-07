#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <cctype>
using namespace std;

const char AND = '&';
const char OR = '|';
const char NOT = '!';
const char START_P = '(';
const char END_P = ')';

bool infix_to_postfix(string infix, string& postfix);
    //converts an infix boolean expression to postfix notation. return true if valid infix expression, false otherwise;
bool evaluate_postfix(const bool values[], string postfix);
    //evaluates a postfix boolean expression containing indices to an array of boolean values. return result of expression.
int precedence(char c1, char c2);
    //return 1 if operator c1 has precedence over operator c2, 0 if c1 has equal precedence to c2, and -1 if c1 does not have precedence over c2.
bool operate (bool o1, bool o2, char op);
    //performs operand1 [operator] operand2 and returns result
string remove_spaces(string s);
    //removes all whitespace from string s

// Evaluates a boolean expression
//   If infix is a syntactically valid infix boolean expression,
//   then set postfix to the postfix form of that expression, set
//   result to the value of the expression (where in that expression,
//   each digit k represents element k of the values array), and
//   return zero.  If infix is not a syntactically valid expression,
//   return 1.  (In that case, postfix may or may not be changed, but
//   result must be unchanged.)
int evaluate(string infix, const bool values[], string& postfix, bool& result){
    if (!infix_to_postfix(infix, postfix)) return 1;
    
    result = evaluate_postfix(values, postfix);
    
    return 0;
}


//Convert boolean expression from infix to postfix notation. Return true if valid infix expression, false otherwise (postfix will not be changed)
bool infix_to_postfix(string infix, string& postfix){
    if (infix.length() == 0) return false;
    
    infix = remove_spaces(infix);
    string original_postfix = postfix;
    postfix = "";
    
    //doesnt matter because it isnt checked unless i > 0
    char prev = '0';
    
    //there should be n-1 of |,& (! doesnt matter) for n operands
    int num_operands = 0;
    int num_operators = 0; //only |, &
    
    stack<char> s; //operator stack
    
    for (int i = 0; i < infix.length(); i++){
        char ch = infix.at(i);
        switch(ch){
            case AND:
            case OR:
                num_operators++;
                //something other than a number before a binary operator
                if ((i > 0 && !(prev >= '0' && prev <= '9')) || i == 0){
                    postfix = original_postfix;
                    return false;
                }
            case NOT: //unary operator
                while(!s.empty() && s.top() != START_P && precedence(ch, s.top()) <= 0){
                    char ch2 = s.top();
                    postfix += ch2;
                    s.pop();
                }
                s.push(ch);
                break;
            case START_P:
                s.push(ch);
                break;
            case END_P:
                if (prev == START_P){ //infix contains "()", invalid
                    postfix = original_postfix;
                    return false;
                }
                while (!s.empty() && s.top() != START_P){ // find matching '('
                    postfix += s.top();
                    s.pop();
                }
                if (s.empty()){ //no matching '(', invalid infix
                    postfix = original_postfix;
                    return false;
                }
                s.pop(); //remove matching '('
                break;
            default:
                if (!isdigit(ch)){ //some foreign non-digit character
                    postfix = original_postfix;
                    return false;
                }
                num_operands++;
                postfix += ch; //all operands
                break;
        }
        prev = ch;
    }
    //if there are too many &,| or there's a ! with no operand, return false
    if (num_operators + 1 != num_operands || infix.at(infix.size() - 1) == NOT){
        postfix = original_postfix;
        return false;
    }
    while(!s.empty()){
        if (s.top() == START_P){ //there wasn't a matching ')' for a '('
            postfix = original_postfix;
            return false;
        }
        postfix += s.top();
        s.pop();
    }
    
    return true;
}

bool evaluate_postfix(const bool values[], string postfix){
    stack<bool> s; //operand stack
    
    for (int i = 0; i < postfix.size(); i++){
        char ch = postfix.at(i);
        if (ch == AND || ch == OR){ //handling operators '|', '&'
            char operand2 = s.top();
            s.pop();
            char operand1 = s.top();
            s.pop();
            s.push(operate(operand1, operand2, ch));
        } else if (ch == NOT){ //handling operator '!'
            char operand = s.top();
            s.pop();
            s.push(operate(operand, ' ', ch)); //operand 2 wont get used anyways
        } else { //handling operands ('0','1', ... , '9')
            s.push(values[ch - '0']);
        }
    }
    //now stack contains one item, the result of the expression
    return s.top();
}

int precedence(char c1, char c2){
    if (c1 == c2) return 0;
    if (c1 == NOT) return 1;
    if (c1 == AND && c2 == OR) return 1;
    return -1;
}

bool operate (bool o1, bool o2, char op){
    if (op == NOT) return !o1;
    if (op == AND) return o1 && o2;
    if (op == OR) return o1 || o2;
    
    return false;
}

string remove_spaces(string s){
    string rtn = "";
    for (int i = 0; i < s.length(); i++){
        if (!isspace(s.at(i))) rtn += s.at(i);
    }
    return rtn;
}
