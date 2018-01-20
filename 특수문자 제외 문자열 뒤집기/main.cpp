// 특수문자 제외 문자열 뒤집기
// "abs/sae&stge[" 

#include <iostream>

using namespace std;

bool isAlphabet(char c) {
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return true;
	else
		return false;
}

void main() {
	char str[100];

	cout << "문자를 입력하세요. (100자 이내)" << endl;
	cout << "입력 : ";
	cin >> str;

	cout << "입력한 문자열 : " << str << endl;

	int len = strlen(str);
	int start = 0;
	int end = len - 1;

	while(start < end) {
		// check
		cout << "\tstart: " << str[start] << ", end: " << str[end] << endl;

		// alphabet 인 경우 swap
		if (isAlphabet(str[start]) && isAlphabet(str[end])) {
			cout << "\t\t둘 다 알파벳 swap" << endl;
			char temp;
			temp = str[start];
			str[start] = str[end];
			str[end] = temp;
			start++;
			end--;
		}
		// start가 알파벳이 아닌 경우
		else if (isAlphabet(str[start]) == false) {
			cout << "\t\tstart 다음 문자로" << endl;
			start++;
		}
		// end가 알파벳이 아닌 경우
		else {
			cout << "\t\tend 이전 문자로" << endl;
			end--;
		}
	}

	cout << "뒤집힌 문자열 : " << str << endl;
	
}