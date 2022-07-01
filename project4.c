#define _CRT_SECURE_NO_WARNINGS //scanf 보안 경고로 인한 컴파일 에러 방지
#include<stdio.h>
#include<ctype.h> //isdigit 이용 위해 include
#include<malloc.h>

//연결 리스트에 사용할 노드 정의
typedef struct stacknode{
	struct stacknode* link;
	double data;
}stacknode;

//스택의 상단을 가리킬 스택 포인터 top정의
stacknode* top;

//스택 초기화 함수, isEmpty, push, pop함수 정의 (linked list이므로 isfull은 필요없음; 제한 X)
void init() { //스택, 스택포인터를 초기화시켜주는 함수
	top = NULL; //원래는 top을 -1로 바꿔줘야 하지만 linked list이므로 top포인터에 저장된 주소를 null로 할당해 linked list와 연결 끊고 처음부터 다시 시작
}

int isEmpty() {
	if (top != NULL) { //top포인터에 할당된 주소 있음 => stack에 node존재
		return 0; //isEmpty 가 false임
	}
	return 1; //isEmpty = True
}
int PIS(char operator) { //stack안에서 우선순위 측정해 해당 우선순위 return하는 switch문

	switch (operator) {
	case '(': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	case '^': return 3;
	default: break;
	}
}
int PIE(char operator) { //stack밖에서 우선순위 측정해 해당 우선순위 return하는 switch문
	switch (operator) {
	case '(': return 4;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	case '^': return 3;
	default: break;
	}
}
void push(double item) {
	stacknode *p; 
	p = (stacknode*)malloc(sizeof(stacknode)); //새 노드 생성, 해당 노드 가리키는 포인터 p

	//추가한 노드 기존 연결 리스트와 연결하는 과정
	if (p == NULL) {
		printf("메모리 할당 에러\n");
		exit(1);
	}
	p->data = item; //노드의 data 부분에 스택 값 item 넣기
	p->link = top; //기존에 존재하던 연결리스트 앞단에 새로 추가할 노드 넣기 (stack-> LIFO)
	top = p; //top + 1해줌
}
double pop() {
	double value = 0; //pop한 값 저장해 return할 변수 지정
	stacknode *freenode; //pop한 node free해주기 위해 주소 임시 저장할 포인터 변수

	freenode = top; //없앨 노드 주소 할당

	if (isEmpty()) { //pop하기 전 연결 리스트에 노드 존재하는지 (스택 있는지) 확인
		printf("stack is Empty\n"); //return 1-> 스택 비었음 출력
		exit(1); //강제 종료
	}
	value =freenode->data; //포인터 top이 가리키는 스택 top노드 얀 data 부분의 값 (item) value에 복사해 저장
	top = freenode->link; //top -1해주기
	free(freenode); //노드 삭제

	return value; //data 반환
}

double peek() { 
	double data=0;

	if (isEmpty()) { //stack이 비었을 경우
		printf("stack is empty\n");
		exit(1);
	}
	else{
		data = top->data;
		return data; //data 삭제 없이 값 보기만 하는 peek 함수
	}
}

//초기에 식 F_original로 받아올 함수
void getinfix(char* F_original) {
	printf("수식을 입력하세요(공백없이)\n");
	scanf("%s", F_original); //입력값 문자 하나씩 문자열 infix에 저장 -> F_original
}
//calpostfix에서 사용할 사칙연산 함수
double operatorcal(double e1, double e2, char operator) {
	double result = 0;//연산 결과값 저장할 변수

	switch (operator) {
	case '+': result = (e2 + e1); break; //두 번째 pop - 첫 번째 pop
	case '-': result = (e2 - e1); break;
	case '/': result = (e2 / e1); break;
	case '*': result = (e2 * e1); break;
	case '^': result = (pow(e2, e1)); break; //제곱승을 구하는 pow method이용
	default: break;
	}
	return result; //결과 반환
}


//(1)F_original -> F_postfix로 변환할 함수 (연결리스트 stack 이용)
void infix_to_postfix(char* F_original, char* F_postfix) {
	char token;
	int j = 0; //F_postfix index용 정수형 변수	

	init(); //스택 초기화

	for (int i = 0; F_original[i] != '\0'; i++) {
		token = F_original[i]; //F_original에 있는 값 하나씩 꺼내와 판단

		if (isdigit(token)) { //token이 opearand일 경우-> F_postfix로 바로 저장
			F_postfix[j++] = token;
		}
		else { //token이 operator일 경우
			if (top != NULL) { //nullptr 예외처리 위해
				if (token == ')') { //token='('일 경우-> ( 전까지 pop해서 F_postfix에 저장
					while (peek() != '(') {
						F_postfix[j++] = pop(); //top의 주소-> top이 가리키는 node pop해줌
					}
					pop(); //'('가 들은 top노드 삭제

				}
				else { //token이 )을 제외한 연산자일 경우
					if (PIS(peek()) >= PIE(token)) { //token의 우선순위가 stack의 top data의 우선순위보다 낮거나 같은 경우
						F_postfix[j++] = pop(); //top을 pop해서 postfix에 저장해주고 token을 push해줌
					}
					push(token); //token의 우선순위가 더 높은 경우 그냥 stack에 push
				}
				continue;

			}push(token); //공백 리스트인 경우 위 조건 고려 없이 그냥 push함
		}
	}
	//F_original = '\0' -> 끝 도달
	while (!isEmpty()) {
		F_postfix[j++] = pop(); //stack안에 값 모두 pop해서 F_postfix에 저장(스택이 empty일 때까지)
	}
}

//(2) F_postfix 계산 함수
int calpostfix(char* F_postfix) {
	char item; //F_postfix에서 요소 1개씩 꺼내 저장할 변수
	double e1 = 0;
	double e2 = 0; //스택에서 operand 꺼내 저장할 변수 선언 및 초기화
	double result = 0; //최종 연산 값 저장할 변수

	init(); //스택 초기화

	for (int k = 0; F_postfix[k] != '\0'; k++) { //F_postfix 끝까지 반복
		item = F_postfix[k]; //F_postfix에서 요소 1개씩 꺼내 저장

		if (isdigit(item)) { //꺼낸 item이 operand일 경우
			push((item - '0'));//item 숫자형으로 변환 뒤 stack에 저장
		}
		else { //꺼낸 item이 operator인 경우
			if (top != NULL) {
				e1 = pop();
				e2 = pop(); //스택에 저장된 operand 2개 꺼내 연산
				result = operatorcal(e1, e2, item); //상단에서 정의한 연산 함수 이용해 그 return값 변수에 저장
				push(result); //연산 결과 다시 stack에 push함
			}	
			}
		}
	//F_postfix 끝까지 도달 시 최종 결과값 출력
	return pop(); //최종 연산 결과 pop하며 끝

}

//메인함수
int main(void) {
	char F_original[50] = { 0 };
	char F_postfix[50] = { 0 }; //F_original, F_postfix의 선언 및 초기화
	//stackpointer* top = NULL; //포인터 top 선언 및 초기화 (공백 리스트 상태)

	getinfix(F_original); //식 입력받아서 F_original에 저장
	printf("F_original = %s\n", F_original);
	//(1) F_original -> F_postfix 전환
	infix_to_postfix(F_original, F_postfix);
	printf("F_postfix = %s\n", F_postfix);
	//(2) F_postfix 연산
	printf("result = %d\n", calpostfix(F_postfix));

	return 0;

}
