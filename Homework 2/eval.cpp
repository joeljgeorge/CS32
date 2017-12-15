#include <string>
#include <stack>
#include <iostream>
#include <cassert>
using namespace std;
//Helper Functions
bool validNum(int pos, string infix);
bool validOpenParenthesis(int pos, string infix);
bool validClosedParenthesis(int pos, string infix);
bool validOr(int pos, string infix);
bool validAnd(int pos, string infix);
bool validNot(int pos, string infix);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{	
	if (infix.size() == 0)//if infix is empty
		return 1; 

	//Removing spaces from infix
	string copy_infix = infix;
	int string_size = copy_infix.size();
	for (int i = 0; i < string_size; i++) {
		if (copy_infix[i] == ' ') {
			for (int j = i; j < string_size - 1; j++) {
				copy_infix[j] = copy_infix[j + 1];
			}
			string_size--;
			i--;
		}
	}
	string final_infix;
	for (int i = 0; i < string_size; i++) {
		final_infix += copy_infix[i];
	}


	stack<char> charstack;
	int parenthesisCount = 0;
	postfix = "";

	for (int i = 0; i < string_size; i++) {
		char a = final_infix[i];
		switch (a) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0'://if character is valid digit, add to stack
			if (!(validNum(i, final_infix))) {
				//cerr << "Number Violation" << endl;
				return 1;
			}
			postfix += a;
			break;
		case ' '://not necessary
			break;
		case '(':
			if (!validOpenParenthesis(i, final_infix)) {//if character is valid open paranthesis, add to stack 
				//cerr << "Open Parenthesis Violation!" << endl;
				return 1;
			}
			parenthesisCount++;
			charstack.push(a);
			break;

		case ')':
			if (!validClosedParenthesis(i, final_infix)) {//same logic as before
				//cerr << "Closed Parenthesis Violation!" << endl;
				return 1;
			}
			while (charstack.top() != '(') {
				postfix += charstack.top();
				charstack.pop();
			}
			parenthesisCount--;
			charstack.pop();
			break;

		case '|':
			if (!validOr(i, final_infix)) {
				//cerr << "Or Operator Violation!" << endl;
				return 1;
			}
			/*if (charstack.empty()) {
				charstack.push(a);
				break;
			}*/
			while (!(charstack.empty()) && (charstack.top() == '|' || charstack.top() == '!' || charstack.top() == '&')) {//while the top of the stack is not (
				postfix += charstack.top();																				  //and has <= precendence as character
				charstack.pop();
			}
			charstack.push(a);
			break;

		case '&':
			if (!validAnd(i, final_infix)) {
				//cerr << "And Operator Violation!" << endl;
				return 1;
			}
			/*if (charstack.empty()) {
				charstack.push(a);
				break;
			}*/
			while (!(charstack.empty()) && (charstack.top() == '!' || charstack.top() == '&')) {//^^^
				postfix += charstack.top();
				charstack.pop();
			}
			charstack.push(a);
			break;
		case '!':
			if (!validNot(i, final_infix)) {
				//cerr << "Not Operator Violation!" << endl;
				return 1;
			}
			charstack.push(a);
			break;

		default:
			return 1;
		}

	}
	while (!(charstack.empty())) {
		postfix += charstack.top();
		charstack.pop();
	}
	if (parenthesisCount != 0) {
		//cerr << "Hanging Parentheses!" << endl;
		return 1;
	}
	
	//evaluate postfix
	stack<bool> bstack;
	bool v2; 
	bool v1;

	for (int i = 0; i < postfix.size(); i++) {
		char a = postfix[i];
		int b = 0;

		if (isdigit(a)) {
			b = a - '0';
			bstack.push(values[b]);
		}

		switch (a) {
		case '&': {
			v2 = bstack.top();
			bstack.pop();
			v1 = bstack.top();
			bstack.pop();
			bstack.push(v1 && v2);
			break;
		}
		case '|': {
			v2 = bstack.top();
			bstack.pop();
			v1 = bstack.top();
			bstack.pop();
			bstack.push(v1 || v2);
			break;
		}

		case '!': {
			v2 = bstack.top();
			bstack.pop();
			bstack.push(!v2);
		}

		default:
			break;
		}
	}

	result = bstack.top();
	bstack.pop();

	return 0; //change
}


//Helper Functions: 
bool validNum(int pos, string infix) {
	if (infix.size() == 1)//if number is only character in string
		return true;
	else if (infix.size() > 1 && pos!= 0 && infix[pos - 1] != '!' && infix[pos - 1] != '|' && infix[pos - 1] != '&' && infix[pos - 1] != '(')
		return false;//if not preceded by '!', '|', '&', or '(', return false 
	else if (infix.size() > 1 && pos != infix.size() - 1 && infix[pos + 1] != '&' && infix[pos + 1] != '|' && infix[pos + 1] != ')') {
		return false;//if not followed by '&', '|', or ')', return false
	}
	return true; 
}

bool validOpenParenthesis(int pos, string infix) {
	if (infix.size() == 1)//if only element in string, return false
		return false;
	else if (pos == infix.size() - 1)//if at end of string, return false
		return false;
	else if (infix.size() > 1 && pos != 0 && infix[pos - 1] != '&' && infix[pos - 1] != '|' && infix[pos - 1] != '(' && infix[pos - 1] != '!') {
		return false;//if not preceded by '&', '|', '(', or '!', return false
	}
	else if (infix.size() > 1 && pos != infix.size() - 1 && !(isdigit(infix[pos + 1])) && infix[pos + 1] != '(' && infix[pos + 1] != '!')
		return false;//if not followed by number, '(', or '!', return false
	return true;
}

bool validClosedParenthesis(int pos, string infix) {
	if (infix.size() == 1)//if only element in string, return false
		return false;
	else if (pos == 0)//if first character in string
		return false;
	else if (infix.size() > 1 && pos != 0 && !(isdigit(infix[pos - 1])) && infix[pos - 1] != ')')
		return false;//if not preceded by number or closing bracket, return false
	else if (infix.size() > 1 && pos != infix.size() - 1 && infix[pos + 1] != '|' && infix[pos + 1] != '&' && infix[pos + 1] != ')')
		return false;//if not followed by or, and, not, or closing bracket operator, return false
	return true;
}

bool validOr(int pos, string infix) {
	if (pos == infix.size() -1)//if character is at end of string, return false
		return false;
	if (pos == 0)//if character is at beginning of string, return false
		return false;
	else if (infix.size() > 1 && pos != 0 && !(isdigit(infix[pos - 1])) && infix[pos - 1] != ')')
		return false;//if not preceded by number or ')', return false
	else if (infix.size() > 1 && pos != infix.size() - 1 && !(isdigit(infix[pos + 1])) && infix[pos + 1] != '(' && infix[pos + 1] != '!')
		return false;//if not followed by number, '(', or '!' 
	return true;
}

bool validAnd(int pos, string infix) {
	if (pos == infix.size() - 1)//if character at end of string, return false
		return false;
	if (pos == 0)//if character is at beginning of string, return false
		return false;
	else if (infix.size() > 1 && pos != 0 && !(isdigit(infix[pos - 1])) && infix[pos - 1] != ')')
		return false;//if not preceded by number or ')', return false
	else if (infix.size() > 1 && pos != infix.size() - 1 && !(isdigit(infix[pos + 1])) && infix[pos + 1] != '(' && infix[pos + 1] != '!')
		return false;//if not followed by number, '(', or '!', return false
	return true;
}

bool validNot(int pos, string infix) {
	if (infix.size() == 1)//if only character in string, return false
		return false;
	else if (pos == infix.size() - 1)//if at the end of string, return false
		return false;
	else if (infix.size() > 1 && pos != 0 && infix[pos - 1] != '&' && infix[pos - 1] != '|' && infix[pos - 1] != '(' && infix[pos - 1] != '!') {
		return false;//if not preceded by '&', '|', '(', or '!', return false
	}
	else if (infix.size() > 1 && pos != infix.size() - 1 && !(isdigit(infix[pos + 1])) && infix[pos + 1] != '(' && infix[pos + 1] != '!')
		return false;//if not followed by number, '(', or '!', return false
	return true;
}


//Main function:
int main() {
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("|8", ba, pf, answer) == 1 && answer);
	assert(evaluate("4 & (5&5)", ba, pf, answer) == 0 && !answer);
	assert(evaluate("01", ba, pf, answer) == 1 && !answer);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("3|9&8", ba, pf, answer) == 0 && !answer);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&7|7)", ba, pf, answer) == 0 && pf == "67&7|" && !answer);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("!4  |  !!!3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "4!3!!!03&&|" && answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2|! 3", ba, pf, answer) == 0 && pf == "23!|" && answer);
	cout << "Passed all tests" << endl;
}