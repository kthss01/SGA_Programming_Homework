#include "Shop2.h"



Shop2::Shop2()
{
}

Shop2::~Shop2()
{
}

#define MAX_SIZE 1000

bool Shop2::ItemSetup()
{
	// item.txt 읽어와서 넣기
	FILE* infile;
	
	// 파일이 있는지 확인
	if ((infile = fopen("item.txt", "r")) == 0) {
		printf("item.txt 파일 읽기 오류\n");
		return false;
	}

	// 파일로부터 읽어온 문자열 담을 변수
	char str[MAX_SIZE];

	// 아이템 정보를 담을 구조체
	tagItemInfo item;

	// 파일의 끝까지 읽기
	while (feof(infile) == 0) {
		fgets(str, MAX_SIZE, infile);

		// 분할된 문자열 위치를 구분하기 위한 변수
		int cnt = 0;

		// strtok로 [ 와 ] 로 모두 분할
		char* token = strtok(str, "[]");

		// 모두 분할 될때까지 반복
		while (token != NULL) {
			switch (cnt) {
			case 0:	// 아이템의 종류
				item.itemKind = (ITEM)atoi(token);
				break;
			case 1: // 아이템의 이름
				item.name = token;
				break;
			case 2: // 아이템의 가격
				item.price = atoi(token);
				break;
			case 3: // 아이템의 특성
				item.attribute = atoi(token);
				break;
			case 4: // 아이템의 설명
				item.description = token;
				break;
			case 5: // 아이템의 갯수
				item.count = atoi(token);
				cnt = 0;	// 다시 0으로 초기화
				m_vItem.push_back(item); // 아이템을 벡터에 넣기
				break;
			}
			// 다음 문자열 가져오기
			token = strtok(NULL, "[]");
			cnt++;
		}
	}

	fclose(infile);	// 파일 닫기
	printf("item.txt 파일 읽기 성공\n");
	return true;
}

int Shop2::ItemOutput(int num, int selectNum)
{
	int itemCount = 0;
	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		// 내가 선택한 아이템 종류만 출력
		if (m_viItem->itemKind != num) continue;

		itemCount++;
		if (itemCount == selectNum) {
			SetColor(SET_COLOR);
		}
		cout << "==================================" << endl;
		cout << "아이템 번호 : " << itemCount << endl;
		cout << "이름 : " << m_viItem->name << endl;
		cout << "가격 : " << m_viItem->price << endl;
		cout << "능력치 : " << m_viItem->attribute << endl;
		cout << "설명 : " << m_viItem->description << endl;
		cout << "갯수 : ";
		// 삼항 연산자 (조건) ? 참 : 거짓
		(m_viItem->count > 0)
			? cout << m_viItem->count << endl
			: cout << "매진" << endl;
		cout << "==================================" << endl;
		SetDefaultColor();
	}
	return itemCount;
}

tagItemInfo Shop2::BuyItem(int itemKind, int num, int & gold)
{
	tagItemInfo buyItem;
	buyItem.itemKind = ITEM_EMPTY;
	int itemCount = 0;
	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		// 내가 선택한 아이템만 출력
		if (m_viItem->itemKind != itemKind) continue;

		// 내가 선택한 번호가 있을 때
		if (++itemCount == num) {
			// 구매할 돈이 있을 때
			if (gold >= m_viItem->price) {
				// 구매할 아이템 갯수가 남아있을 때
				if (m_viItem->count > 0) {
					cout << m_viItem->name << "을 "
						<< m_viItem->price << "gold에 구매" << endl;
					gold -= m_viItem->price;
					m_viItem->count--;
					buyItem = *m_viItem;	// 아이템 정보 넣어줌
				}
				else {
					cout << "재고가 없다" << endl;
				}
				// 아이템 찾았으므로 끝
				break;
			}
		}
	}
	return buyItem;
}

void Shop2::AddItem(tagItemInfo item)
{
	bool isNewItem = true;

	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		// 아이템 같은 종류인지 확인
		if (m_viItem->itemKind != item.itemKind) continue;

		// 아이템 이름 같으면 갯수만 증가
		if (m_viItem->name.compare(item.name) == 0) {
			m_viItem->count++;
			isNewItem = false;
			break;
		}
	}

	if (isNewItem == true) {
		m_vItem.push_back(item);
	}
}
