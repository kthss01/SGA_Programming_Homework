#include <iostream>
#include <time.h>

using namespace std;

void main() {
	char str[100];

	cout << "문자를 입력하세요. (100자 이내)" << endl;
	cout << "입력 : ";
	cin >> str;

	cout << "입력한 문자열 : " << str << endl;

	int len = strlen(str);

	for (int i = 0; i < len / 2; i++) {
		char temp;

		temp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = temp;
	}

	cout << "뒤집힌 문자열 : " << str << endl;

}