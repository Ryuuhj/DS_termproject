#include <stdio.h>
#define _CRT_SECURERE_NO_WARNINGS //scanf 보안 경고로 인한 컴파일 에러 방지


//정렬은 오름차순 기준
int main()
{
	int ArrL[12]= {0}; //dummy variable 방지 위한 배열 선언 및 초기화
	int temp; //selection sort중 배열 원소 swap시 임시 저장할 변수 선언 및 초기화
	int sum = 0;
	float avg = 0; // 변수 선언 및 초기화

	//(1) 배열 입력 + 갱신 상태 표시
	for (int k = 0; k < 12; k++) { //12단계로 진행되므로 12번 반복되는 for문 실행
		printf("숫자 12개를 입력하시오 (%d / 12)\n", k+1); //카운팅을 위한 안내문
		scanf("%d", &ArrL[k]); //인수를 해당하는 배열 원소의 주소에 저장 (주소연산자 이용)

		printf("ArrL = {");
		for (int num = 0; num < k + 1;num++) {
			printf("%d ",ArrL[num]);
		}
		printf("}\n"); //배열 출력 
	}

	//(2)sorting 수행, 변경 되는 경우만 현재상태 출력

	for (int n1 = 0; n1 < 11; n1++) {
		for (int n2 = (n1 + 1);n2 < 12; n2++) { //앞 원소 다음 원소부터 비교함
			if (ArrL[n1] <= ArrL[n2]) { //비교하는 앞 원소가 뒤 원소보다 작거나 같은 경우 -> 유지
				continue;
			}
			else //비교하는 앞 원소가 뒤 원소보다 큰 경우-> 순서 바꿔서 저장해야 함
			{	
				//값 스왑
				temp = ArrL[n1]; //값 임시 저장 변수 temp이용해 앞 원소 값 보관
				ArrL[n1] = ArrL[n2];
				ArrL[n2] = temp; 

				//변경 됐으므로 현재 상태 출력
				printf("배열의 현재 상태 프린트: ArrL = {");
				for (int j = 0; j < 12;j++) {
					printf("%d ", ArrL[j]);
				}
				printf("}\n"); 
			}
		}
	}

	//(3) 배열 역순 출력, 12개 합, 평균 구하기

	printf("ArrL = {");
	for (int i = 11; i >-1; i--) { //역순으로 배열 출력
		printf("%d ", ArrL[i]);
		sum += ArrL[i]; //sum에 누적시켜 최종 합 구하기
	}
	avg = sum / (sizeof(ArrL) / 4); //int배열이므로 한 칸당 4byte -> /4해줘야 함
	printf("}\n");
	printf("합 : %d, 평균 : %0.1f", sum, avg); //평균 -> 소수점이하 1자리까지 출력

	return 0;
}
