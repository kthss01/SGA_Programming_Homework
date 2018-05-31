// 후위 표기법
/*
	중위 표기식이 주어졌을 때 후위 표기식으로 고치고
	계산값 출력하기
	ex) 
	입력
	2+3*(5+4*2)-3+6
	출력
	23542*+*+3-6+
*/

#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

vector<char> v; // 후위 수식을 담을 벡터
stack<char> st; // 연산자 스택
string infix; // 중위표기식
string postfix; // 후위표기식

//stack<int> st2; // 계산을 위한 stack

// stack 구현
////////////////
int Stack[101], top = -1;

void push(int x) {
	Stack[++top] = x;
}

int empty() {
	if (top < 0) return 1;
	else return 0;
}

void pop() {
	if (!empty()) Stack[top--] = 0;
}

char peek() {
	if (!empty()) return Stack[top];
	else return 0;
}
/////////////////

int getPriority(char op) {
	
	if (op == '*' || op == '/') return 3;
	if (op == '+' || op == '-') return 2;
	if (op == '(') return 1;

	//if (op >= '0' && op <= '9') return -1;
	return -1;
}

void convertPostfix() {
	for (int i = 0; i < infix.length(); i++) {
		char ch = infix[i];
		// 숫자
		if (ch >= '0' && ch <= '9')
			v.push_back(infix[i]);
		else {
			// '(' or 연산자 스택이 비어있으면
			if (ch == '(' || st.empty())
				st.push(ch);
			// ')'
			else if (ch == ')') {
				char op = st.top();
				st.pop();
				while (op != '(') {
					v.push_back(op);
					op = st.top();
					st.pop();
				}
			}
			// '+', '-', '-', '/' : 
			// 현재 연산자가 연산자 스택의 탑보다 가중치가 높을 때
			else if (getPriority(ch) > getPriority(st.top()))
				st.push(ch);
			// '+', '-', '-', '/' : 
			// 현재 연산자가 연산자 스택의 탑보다 가중치가 낮거나 같을 때
			else {
				while (!st.empty() &&
					getPriority(ch) <= getPriority(st.top())) {
					v.push_back(st.top());
					st.pop();
				}
				st.push(ch);
			}
		}
	}
	// 스택 연산자의 남은 연산자 후위 표기식에 넣기
	while (!st.empty()) {
		v.push_back(st.top());
		st.pop();
	}

	for (int i = 0; i < v.size(); i++)
		postfix += v[i];
	cout << "후위 표기식 : " << postfix << "\n";
}

int calcTwoOprd(int oprd1, int oprd2, char op) {
	if (op == '+') return oprd1 + oprd2;
	if (op == '-') return oprd1 - oprd2;
	if (op == '*') return oprd1 * oprd2;
	if (op == '/') return oprd1 / oprd2;
}

void caculate() {
	for (int i = 0; i < postfix.length(); i++) {
		char ch = postfix[i];
		// 숫자
		if (ch >= '0' && ch <= '9')
			//st2.push(ch - '0');
			push(ch - '0');
		// 연산자
		else {
			//int op2 = st2.top();
			//st2.pop();
			//int op1 = st2.top();
			//st2.pop();
			//st2.push(calcTwoOprd(op1, op2, ch));
			int op2 = peek();
			pop();
			int op1 = peek();
			pop();
			push(calcTwoOprd(op1, op2, ch));
		}
	}

	//int ans = st2.top();
	//st2.pop();
	int ans = peek();
	pop();
	cout << "계산: " << ans << "\n";
}

int main() {
	//cin >> express;
	infix = "2+3*(5+4*2)-3+6";

	// test
	cout << "중위 표기식 : " << infix << "\n";

	convertPostfix();

	caculate();

	return 0;
}