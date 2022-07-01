#define _CRT_SECURE_NO_WARNINGS //scanf 보안 경고로 인한 컴파일 에러 방지
#include<stdio.h>
#include<ctype.h> //isdigit 이용 위해 include
#include<math.h> //거듭제곱(pow)위해 

double stack[100] = { 0 }; //스택 선언(operator 만 저장하므로 char형으로 선언)
int top = -1; //stack의 top포인터 전역변수로 선언

int PIS(char operator);
int PIE(char operator);
void push(double item);
double pop();
double operatorcal(double e1, double e2, char operator);
void gettoArray();
void infix_to_postfix(char *F_original, char *F_postfix);


//스택 이용한 연산시 필요한 함수 정의
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
void push(double item) { //stack에 push하는 경우 함수
	if (top >= 23) {
		printf("stack is full\n");
		return;
	}
	else {
		top += 1; //top 한칸 증가-> stack에 해당 원소 넣기
		stack[top] = item; 
		//printf("top= %d\n", top); //**top 숫자 체크용 디버깅 코드**
	}
}
double pop() { //stack에서 pop하는 경우 함수 (stack과 자료형 맞춰주기)
	double value = 0; //pop한 element 담아서 return할 변수 선언 및 초기화
	if (top < 0) {
		printf("stack is empty\n");
		exit(1);
	}
	else {
		value = stack[top];
		top -= 1;
		return value; //원소값 return
	}
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
	return result;
}
//식 입력받아 F_original에 저장할 함수
void gettoArray(char *F_original) {
	printf("수식을 입력하세요(공백없이)\n");
	scanf("%s", F_original); //입력값 문자 하나씩 문자열 infix에 저장 -> F_original
}

//F_original -> F_postfix로 전환 시 사용할 함수 정의
void infix_to_postfix(char *F_original, char *F_postfix) {
	char token;
	int i = 0; //F_postfix의 인덱스로 사용될 i 선언 및 초기화

	for (int j = 0; F_original[j] != '\0';j++) { //F_original의 마지막(=\0)까지 for문 돌려서 요소 하나씩 꺼내서 판단
		token = F_original[j]; //token에 꺼낸 요소 저장

		//token = operand인 경우
		if (isdigit(token)) { //token의 자료형이 숫자일 경우(True)
			F_postfix[i++] = token;} //F_postfix로 곧바로 이동
		//token = operator인 경우
		else {
			//printf("token -> %c\n", token);
			if (token == ')') { //token = ")"이면 
				while (stack[top] != '(') { //'(' 만나기 전까지 반복해서 pop->F_postfix에 저장
					F_postfix[i++] = pop();
					//printf("postfix = %s\n", F_postfix); //**F_postfix 내부 상황 확인용 디버깅 코드**
				}
				//괄호 없애고 그 밑 stack부터 다시 봄 (F_postfix에 괄호는 안들어가므로)
				top--;
			}
			//token = ")"제외한 operator (우선순위 O)
			else {
				if (PIS(stack[top]) >= PIE(token)) { //token의 우선순위가 낮거나 같은 경우 stack의 top을 pop해 F_postfix에 저장
					//printf("%d", PIS(stack[top])); //**stack[top]의 우선순위가 맞게 측정 됐는지 확인용 디버깅 코드**
					F_postfix[i++] = pop();}
				push(token);//token push 해줌 + token의 우선순위가 높은경우 그냥 push만하는 경우에 사용
				}
			}
		
		}
		
	while (top > -1) { // F_original[j]='\0'인 경우 stack안 나머지 값 모두 pop-> F_postfix에 저장
		//printf("F_orignal 끝 ... stack값 pop중\n"); //**F_original 마지막 도달 시 pop 체크용 코드**
		F_postfix[i++] = pop();
		
	}

}

//postfix 연산 함수
int calpostfix(char *F_postfix) {
	char item; //F_postfix로부터 원소 받아올 변수
	double e1 = 0;
	double e2 = 0; //stack에서 pop한 값 저장할 변수 선언 및 초기화
	double result=0; //연산 결과 저장할 변수 선언 및 초기화

	
	for (int i = 0; F_postfix[i] != '\0';i++) { //F_postfix 문자열의 끝에 도달할 때 까지 반복
		item = F_postfix[i]; //char로 받아옴

		if (isdigit(item)) { //받아온 원소가 숫자일 때 (ASCII code 비교시 숫자형 사이에 존재함)
			push(item-'0'); //item을 숫자형으로 전환해 stack에 저장함
		}
		else{ //받아온 item이 연산자일 경우
			e1 = pop();
			e2 = pop(); //stack에서 차례대로 꺼내와서 정의한 변수에 저장
			result = operatorcal(e1, e2, item);
			//printf("%f,%f",e1,e2); //**자료형 불일치로 인한 오류 발견 위한 체크용 코드**
;			push(result);
			//printf("result >> %f\n", result); //**마찬가지로 자료형 불일치로 인한 오류 체크용 코드**
		}
		
	}
	//F_postfix = '\0'인 경우
	return pop(); //최종적으로 결과값 stack[0]에 저장된 상태-> pop해서 결과 도출 (int형으로)
	
}


int main() {
	char F_original[50] = { 0 };
	char F_postfix[50] = { 0 }; //F_original, F_postfix의 선언 및 초기화

	gettoArray(F_original); //표준 입력으로 F_original 수식 array에 넣음
	//printf("%s\n", F_original); //** F_original 입력 완료 확인용 코드**
	infix_to_postfix(F_original, F_postfix);
	//F_original -> F_postfix 전환
	printf("F_postfix = %s\n", F_postfix);
	//도출된 F_postfix 값 확인

	printf("result = %d\n", calpostfix(F_postfix));
	// F_postfix 연산 값 출력

	return 0;


}
