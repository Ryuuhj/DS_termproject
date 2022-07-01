#define _CRT_SECURE_NO_WARNINGS //scanf 보안 경고로 인한 컴파일 에러 방지
#include<stdio.h>
#include<stdlib.h> //malloc이용, rand 이용
#include<string.h>//strcpy

typedef struct DLNode {
	struct DLNode* llink;
	struct DLNode* rlink; //양 방향 노드이므로 좌우 링크 추가
	int num;
	char name[20];
}DLNode;

//tail-> 리스트의 마지막 노드를 지시하는 C를 가리키는 포인터 (C의 메모리 주소를 담고있음) 따라서 tail을 역참조함으로써 C에 담긴 주소를 바꿀 수 있음
void addDLNode(DLNode** tail, int num, char name[20]) {
	DLNode* newDLNode;
	newDLNode = (DLNode*)malloc(sizeof(DLNode));
	//노드 공간 배정
	newDLNode->num = num; //노드의 data 부분 채우기
	strcpy(newDLNode->name, name); //문자열을 채울 때는 strcpy함수를 이용해 복사 붙여넣기 함
	newDLNode->llink = NULL;
	newDLNode->rlink = NULL;
	//노드에 data 넣고 양쪽 link가 NULL인 노드 1개 생성
	
	//-- 기존에 존재하던 리스트 뒤로 삽입해 줄 차례--
	if ((*tail) == NULL) {
		printf("공백 리스트 입니다. 첫번째 노드로 추가합니다.\n"); //리스트의 시작 체크용 코드
		(*tail) = newDLNode; //생성된 노드 자체가 리스트의 전부임
		newDLNode->llink = (*tail); 
		newDLNode->rlink = (*tail); 
		//리스트에 노드 1개만 존재하는 경우
	}
	else { //리스트에 노드가 1개 이상 존재하는 경우
		newDLNode->rlink = (*tail)->rlink;
		(*tail)->rlink->llink = newDLNode;
		(*tail)->rlink = newDLNode;
		newDLNode->llink = (*tail);
		//새 노드를 맨 뒤에 삽입 tail이 가리키는 노드 뒤
		(*tail) = newDLNode;
		//tail은 맨 마지막 노드를 가리켜야 하므로 새로 삽입된 노드 주소로 값 변경
	}
}

void printList(DLNode* tail) {
	DLNode* p; //한바퀴 돌며 출력할 포인터 정의
	p = tail;
	if (p == NULL) { //리스트가 공백리스트인 경우 출력할 것 없음 -> 종료
		printf("공백리스트 입니다.\n");
		exit(1);
	}
	else {
		p = p->rlink; //마지막 노드를 가리키고 있으므로 한칸 전진 해 첫번째 노드부터 출력
		printf("[");
		do
		{
			printf("(%d - %s) ", p->num, p->name); //p가 가리키는 노드에 담긴 데이터 출력
			p = p->rlink;//한칸 전진

		} while (p != tail->rlink); //첫번째 노드(tail->rlink)로 다시 돌아오기 전까지 반복
		printf("]\n");
	}

}
//포인터에 저장된 주소를 직접적으로 수정해야 함 + 외부에서 정의된 포인터 -> 포인터의 포인터를 이용해, 역참조 방법으로 주소를 변경해 줘야함
//따라서 삭제할 노드를 가리키는 del역시 포인터의 포인터 형식으로 매개변수 전달해야 함

void deleteDLNode(DLNode** tail, DLNode** del, int flow) { //del-삭제할 노드 포인터의 포인터
	DLNode* p = NULL; //삭제한 노드 다음부터 다시 진행하기 위해 del 주소 업뎃 위한 주소 보관용 포인터

	if ((*tail) == NULL) { //공백리스트일 경우 예외처리
		printf("공백 리스트입니다. 삭제할 노드가 존재하지 않습니다.\n");
		exit(1);
	}
	else {
		if ((*del) == (*tail)) { //삭제할 노드 = 리스트의 마지막 노드일 경우
			(*tail) = (*del)->llink;
		} //리스트 마지막 노드의 포인터인 tail을 그 앞 노드 지칭으로 변경
//이 이후부터는 공통 진행
		if (flow){
			p = (*del)->rlink; //순차실행
		}
		else{
			p = (*del)->llink; //역실행
		} 
		//순방향, 역뱡향에 따른 진행 분기점 생성
		(*del)->llink->rlink = (*del)->rlink;
		(*del)->rlink->llink = (*del)->llink; //빠진 노드 양 끝 노드끼리 이어주기
		printf("빠지는 사람 : %d - %s\n", (*del)->num, (*del)->name); //노드 삭제 전 내용 출력
		free((*del)); //노드 삭제
		printf("남은 사람: ");
		printList((*tail)); //남은 리스트 출력
		(*del) = p; //del또한 삭제되면 전/다음 노드부터 다시 진행하므로 주소 재배정
		//이후로는 동일함
	}
}

void forward_backward(int skip_number, int flow) { //순방향, 역뱡향 진행 함수
	DLNode* C = NULL; //닻 포인터 정의
	DLNode* del = NULL; //삭제 노드 가리킬 포인터
	char name[11][20] = { "James","John","Charles","Mary","Linda","Raymond","Lisa", "Betty","Victor","Helen","Edward" };
	int length = 11; //리스트 길이 선언

	//노드에 정보 채우기
	for (int j = 0;j < 11;j++) {
		addDLNode(&C, j + 1, name + j); //name이 리스트의 시작 주소 담고있으므로  +j해주면 인덱스 j에 저장된 문자열 나옴
	}
	//printList(C); //**잘 채워졌는지 확인용 코드**
	//printf("C -> %d\n", C->num); //**닻 포인터 끝에 잘 위치했는지 체크용**

	printf("skipnumber = %d\n", skip_number);

	del = C->rlink; //1번 기준으로 삭제 카운트 시작
	//-----(1) 순방향 알고리즘
	if (flow){ //true==1임을 이용
		//printf("시작 번호 -> %d\n", del->num); //**시작 번호 체크용 코드**
		do {
			for (int i = 0; i < skip_number;i++) {
				del = del->rlink; //skip number만큼 del포인터 이동후 (우측 이동)
			}
			deleteDLNode(&C, &del, flow); //노드 삭제
			length--; //길이 하나 차감
		} while (length != 3);
	}
	//---------(2)역방향 알고리즘
	else{
		//printf("시작 번호 -> %d\n", del->num);
		do {
			for (int i = 0; i < skip_number;i++) {
				del = del->llink; //skip number만큼 del포인터 이동후 (좌측 이동)
			}
			deleteDLNode(&C, &del, flow); //노드 삭제
			length--;
		} while (length != 3);
	}
	printf("최종 탈출 명단:"); //각 방향에 따른 결과로 최종 리스트 출력
	printList(C);
	
}

void main(void) {
	//진행 방향- 순방향(1), 역방향(0)
	
	//난수 생성 전 초기화
	srand(time(NULL));
	//난수 생성후 저장
	int skip_number = (rand() % 9) + 1; //1~9사이 랜덤한 숫자
	//동일한 수행을 위해 외부에서 정의 후 인수로 값 전달

	printf("(1)순방향 진행\n");
	forward_backward(skip_number, 1); //순방향 -> flow == 1
	printf("\n");
	printf("(2)역방향 진행\n");
	forward_backward(skip_number, 0); //역방향 -> flow == 0

	return 0;
}
