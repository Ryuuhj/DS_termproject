#define _CRT_SECURE_NO_WARNINGS //scanf 보안 경고로 인한 컴파일 에러 방지
#include<stdio.h>
#include<stdlib.h> //malloc이용, rand 이용
//#include<malloc.h>
#include<string.h>//strcpy

typedef struct Node {
	struct Node* link;
	int num;
	char name[20];
}Node;

//tail-> 리스트의 마지막 노드를 지시하는 C를 가리키는 포인터 (C의 메모리 주소를 담고있음) 따라서 tail을 역참조함으로써 C에 담긴 주소를 바꿀 수 있음

void addNode(Node** tail, int num, char name[20]) {
	Node* newNode;
	newNode = (Node*)malloc(sizeof(Node));
	//노드 공간 배정
	newNode->num = num;
	strcpy(newNode->name, name);
	newNode->link = NULL;
	//노드에 data 넣고 link NULL인 노드 1개 생성
	//-- 기존에 존재하던 리스트 뒤로 삽입해 줄 차례--
	if ((*tail) == NULL) {
		printf("공백 리스트 입니다. 첫번째 노드로 추가합니다.\n");
		(*tail) = newNode;
		newNode->link = (*tail);
	}
	else { //리스트에 노드가 1개 이상 존재하는 경우
		newNode->link = (*tail)->link;
		(*tail)->link = newNode;
		(*tail) = newNode;
	}
}
void printList(Node* tail) {
	Node* p; //한바퀴 돌며 출력할 포인터 정의
	p = tail;
	if (p == NULL) {
		printf("공백리스트 입니다.\n");
		exit(1);
	}
	else {
		p = p->link; //마지막 노드를 가리키고 있으므로 한칸 전진 해 첫번째 노드부터 출력
		printf("[");
		do
		{
			printf("(%d - %s) ", p->num, p->name); //데이터 출력
			p = p->link;//한칸 전진

		} while (p != tail->link);
		printf("]\n");
	}

}

//포인터에 저장된 주소를 직접적으로 수정해야 함 + 외부에서 정의된 포인터 -> 포인터의 포인터를 이용해, 역참조 방법으로 주소를 변경해 줘야함

void deleteNode(Node** tail, Node** del, Node* trail) { //del-삭제할 노드 포인터의 포인터, trail- 그 전 노드 포인터 (뒤처리용)
	if ((*tail) == NULL) { //공백리스트일 경우 예외처리
		printf("공백 리스트입니다. 삭제할 노드가 존재하지 않습니다.\n");
		exit(1);
	}
	else {
		if ((*del) == (*tail)) { //삭제할 노드 = 리스트의 마지막 노드일 경우
			(*tail) = trail;
		} //리스트 마지막 노드의 포인터인 tail을 그 앞 노드 지칭으로 변경
//이 이후부터는 공통 진행
		trail->link = (*del)->link; //del 노드 삭제 위한 작업
		printf("빠지는 사람 : %d - %s\n", (*del)->num, (*del)->name); //노드 삭제 전 내용 출력
		free((*del)); //노드 삭제
		printf("남은 사람: ");
		printList((*tail)); //남은 리스트 출력
		(*del) = trail->link; //del또한 삭제되면 다음 노드부터 다시 진행하므로 주소 재배정
	}
}



void main(void) {
	Node* C = NULL;
	Node* trail = NULL;
	Node* del = NULL;
	//C-마지막 노드 고정 포인터(=tail), trail, del-> 노드 삭제시 이용할 리스트 포인터 정의
	char name[11][20] = { "James","John","Charles","Mary","Linda","Raymond","Lisa", "Betty","Victor","Helen","Edward" };
	//값 할당시 편의를 위해 문자열의 배열 생성-> for문에 이용할 예정
	
	srand(time(NULL));
	//난수 생성 전 초기화
	int skip_number = (rand() % 9) + 1; //1~9사이 랜덤한 숫자
	//난수 생성후 저장
	int length = 11; //리스트 길이 선언 (3개 남았을 때 멈추기 위함)

	//노드에 정보 채우기
	for (int j = 0;j < 11;j++) {
		addNode(&C, j + 1, name + j); //name= 이름 담은 배열 시작 주소 -> char배열 이므로 +j해주면 인덱스 j에 저장된 이름 나옴
	}
	//printList(C); //**잘 채워졌는지 확인용 코드**

	del = C->link; //1번 기준으로 삭제 카운트 시작
	printf("skip number = %d\n", skip_number);
	//skip number확인용
	do {
		for (int i = 0; i < skip_number;i++) { //del 포인터 세팅
			trail = del;//trail에 del 주소 백업
			del = del->link; //skip number만큼 del포인터 이동후
		}
		deleteNode(&C, &del, trail); //노드 삭제
		printf("\n");
		length--; //리스트 길이 -1
	} while (length != 3); //3명 남았을 때 반복문 탈출
	
	/*for (int i = 0; i < 9; i++)
	{
		del = del->link;
		printf("del 번호 : %d", del->num);
	} -> 오류 디버깅용 코드
	*/ 

	printf("최종 탈출 명단:");
	printList(C);
	return 0;
}
