#define _CRT_SECURE_NO_WARNINGS //scanf 보안 경고로 인한 컴파일 에러 방지
#include<stdio.h>
#include<stdlib.h> //malloc이용

typedef struct bcNode {
	struct bcNode* llink;
	struct bcNode* rlink;
	int num;
}bcn; //Bi-directional Circular Linked List를 구현하기 위한 노드의 struct 지정 (양방향이므로 링크 두 개 필요)

//노드 추가하는 함수
void addbcNode(bcn** bclist, int num) {
	bcn* newbcNode; //새 노드 포인터
	bcn* tail = NULL; //리스트에 노드가 1개 일 때 + 2개 이상일 때 동시에 구현 위한 포인터
	newbcNode = (bcn*)malloc(sizeof(bcn));
	//노드 공간 배정
	newbcNode->num = num; //매개변수로 받아온 값 data에 할당
	newbcNode->llink = NULL;
	newbcNode->llink = NULL;
	//노드에 data 넣고 양쪽 link NULL인 노드 1개 생성

	//-- 기존에 존재하던 리스트 뒤로 삽입해 줄 차례--
	if ((*bclist) == NULL) { //리스트가 공백리스트인 경우 bcList = NULL일 때
		printf("공백 리스트 입니다. 첫번째 노드로 추가합니다.\n"); //**리스트의 시작 체크용 코드**
		(*bclist) = newbcNode; //생성된 노드 자체가 리스트의 전부임
		newbcNode->llink = *bclist;
		newbcNode->rlink = *bclist;
		//자기 자신을 가리키게 해줌
	}
	else { //리스트에 노드가 1개 이상 존재하는 경우
		tail = (*bclist)->llink; //tail = bcList->llink로 지정하면, 1개 존재하는 경우 + 2개 이상 동시에 충족 가능
		newbcNode->rlink = *bclist;
		newbcNode->llink = tail; //newbcNode를 먼저 삽입해주고,
		tail->rlink = newbcNode;
		(*bclist)->llink = newbcNode; //전후를 이어줌
	}
}
//리스트 출력함수
void printList(bcn* bclist, int direction) { //direction-> 순방향(1), 역방향(0) 지정용 매개변수
	bcn* p; //한바퀴 돌며 출력할 포인터 정의
	p = bclist; //첫 노드부터 출력하므로 bcList 주소 세팅

	if (p == NULL) { //리스트가 공백리스트인 경우 출력할 것 없음 -> 종료
		printf("공백리스트 입니다.\n");
		exit(1);
	}
	else { //리스트에 노드 존재
		printf("bcList = (");
		do {
			printf("%d, ", p->num); //p가 가리키는 노드에 담긴 데이터 출력
			if (direction) { p = p->rlink; }
			else { p = p->llink; }//순방향(direction - 1), 역방향에 따른 분기문, 포인터의 좌/우 방향 이동 결정
		} while (p != bclist); //첫번째 노드(bcList)로 다시 돌아오기 전까지 반복
		printf(")\n");
	}
}
//선택 정렬 실행 함수
void selecsort(bcn* bclist, bcn** main, bcn** trace) { //외부 포인터의 값 변경이 필요하므로, 포인터의 포인터 사용
	int temp = 0; //trace, main의 data swap시 임시 보관할 변수 선언 및 초기화
	if (bclist == NULL) { //공백리스트인 경우 예외처리
		printf("정렬할 노드가 없습니다. 공백리스트 입니다.\n");
		exit(1);
	}
	else { //main을 중심으로 총 10번의 jump가 발생하고, 그 1번의 jump에 대해 trace가 10번 jump해 main과 비교하는 방식
		(*main) = bclist; //첫 노드부터 비교 시작 
		do { //main 진행 구간
			(*trace) = (*main)->rlink; //trace는 main 다음부터 jump 시작
			do { //trace 진행 구간
				if ((*main)->num < (*trace)->num) { /*printf("pass");//확인용 디버깅 코드*/ } //trace가 더 큰 경우 그대로 둠 (오름차순이므로)

				else { //main이 더 큰 경우 trace와 자리를 바꿔줘야 함
					temp = (*trace)->num; //변수에 trace data 임시 보관
					(*trace)->num = (*main)->num; //trace와 main의 데이터 교환
					(*main)->num = temp; //노드 안에 data인 num만 swqp해줌
					printf("선택 정렬 발생)) ");
					printList(bclist, 1); //순서 바뀔때마다 순방향으로 프린트 해줌
				}
				(*trace) = (*trace)->rlink; //trace 1칸 전진
			} while ((*trace) != bclist); //한바퀴 돌아 첫 노드로 오기 전까지 반복

			(*main) = (*main)->rlink; //main 한칸 전진
		} while ((*main) != bclist->llink); //main은 리스트의 마지막 노드 전까지 반복 (마지막 노드는 비교할 필요X)
	}
}
//합계 구하는 함수
int getsum(bcn* bclist) {
	bcn* trace; //한바퀴 돌며 출력할 포인터 정의
	int sum = 0; //총 합 저장할 변수 선언

	trace = bclist;// 첫 노드부터 시작
	do {
		sum += trace->num; //trace가 가리키는 노드에 담긴 num sum에 더하기
		trace = trace->rlink; //trace 전진
	} while (trace != bclist); //첫번째 노드(bcList)로 다시 돌아오기 전까지 반복

	return sum;//합계 반환
}

void main() {
	bcn* bcList = NULL;
	bcn* main = NULL;
	bcn* trace = NULL;
	//리스트의 포인터 bcList외, 사용하는 main, trace 정의
	int number[12] = { 99,88,11,76,79,54,43,66,62,32,21,7 }; //리스트에 들어갈 숫자 담긴 array 생성
	int sum = 0; //getsum의 반환값 저장할 변수 정의


	for (int j = 0;j < 12;j++) {
		addbcNode(&bcList, number[j]); //노드 12개 생성 및 연결
		//(1) - 노드 갱신마다 출력 (순방향 / 역방향)
		printf("%d번째 삽입)\n순방향 - ", j + 1);
		printList(bcList, 1);
		printf("역방향 - ");
		printList(bcList, 0);
		printf("\n");
	}
	//(2) selection sorting 출력
	printf("(2) selection sort 진행 \n");
	printList(bcList, 1); //세팅 완료된 bcList 전체 출력
	selecsort(bcList, &main, &trace); //선택 정렬 진행, 진행 중 변동 발생시 프린트
	//(3) 오름차순, 내림차순, 합, 평균
	printf("\n(3)오름차순, 내림차순 출력, 원소값 합, 평균 구하기\n");
	printf("1) 오름차순 출력\n");
	printList(bcList, 1); //1-> 순방향
	printf("2) 내림차순 출력\n");
	printList(bcList->llink, 0); //bcList->llink; => 리스트 맨 끝에서부터 역순으로 출력
	printf("3) 리스트 합, 평균 계산\n");
	sum = getsum(bcList); //반환 값(sum) 저장
	int avg = sum / (sizeof(number) / sizeof(int)); //총 합을 배열의 크기(12)로 나눠 평균 구함
	printf("리스트 합-> %d   평균 -> %d\n", sum, avg); //출력

	return 0;
}
