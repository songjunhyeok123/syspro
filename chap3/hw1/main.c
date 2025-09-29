#include <stdio.h>
#include <string.h>
#include "copy.h"

#define MAXLINE 100
#define NUM 5

int main(){
    char str[NUM][MAXLINE];    // 문자열 5개 저장 배열
        char temp[MAXLINE];        // 복사용 임시 배열
	    int i, j;

	        // 5개의 문자열 입력
		    printf("문자열 5개를 입력하세요:\n");
		        for(i = 0; i < NUM; i++){
			        fgets(str[i], MAXLINE, stdin);

				        // fgets로 입력받은 문자열 끝에 개행문자('\n') 제거
					        size_t len = strlen(str[i]);
						        if(len > 0 && str[i][len - 1] == '\n'){
							            str[i][len - 1] = '\0';
								            }
									        }

										    // 길이가 큰 순서대로 정렬 (버블 정렬)
										        for(i = 0; i < NUM -1; i++){
											        for(j = 0; j < NUM - 1 - i; j++){
												            if(strlen(str[j]) < strlen(str[j+1])){
													                    // 두 문자열 위치 교환
															                    copy(str[j], temp);
																	                    copy(str[j+1], str[j]);
																			                    copy(temp, str[j+1]);
																					                }
																							        }
																								    }

																								        // 정렬 결과 출력
																									    printf("\n문자열 길이가 큰 순서대로 출력:\n");
																									        for(i = 0; i < NUM; i++){
																										        printf("%s\n", str[i]);
																											    }

																											        return 0;
																												}
