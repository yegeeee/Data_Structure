//
//  main.cpp
//  Programming Report1
//
//  Created by 강예지 on 2017. 10. 6..
//  Copyright © 2017년 강예지. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STACK_SIZE 10000

typedef struct{
    //StackType 구조체에는 데이터를 저장할 배열과, 데이터가 몇개 쌓였는지 알 수 있는 top 변수가 존재한다.
    int stack[MAX_STACK_SIZE];
    int top;
} StackType;

void init(StackType *s){
    //초기에는 top이 -1로 초기화되어있다.
    s->top = -1;
}

int is_empty(StackType *s){
    //top이 -1이면 배열에 데이터가 존재하지 않는다는 뜻이다.
    return (s->top == -1);
}

int is_full(StackType *s){
    //top이 스택의 최대 크기보다 1이 작다면 스택이 꽉 찼다는 뜻이다.
    return (s->top == MAX_STACK_SIZE-1);
}

void push(StackType *s, int item){
    if(is_full(s)){
        //스택이 꽉차면 더 이상 데이터를 배열에 넣을 수 없으므로 에러를 출력한다.
        fprintf(stderr, "스택 포화 에러\n");
        return;
    }
    else s->stack[++(s->top)] = item; //스택이 꽉 차지 않았으면 배열에 데이터를 추가하고 top변수를 1만큼 증가시켜 데이터를 추가했음을 기록한다.
}

int pop(StackType *s){
    if(is_empty(s)){
        //스택이 비어있으면 더 이상 데이터를 출력할 수 없으므로 스택 공백 에러를 출력한다.
        fprintf(stderr, "스택 공백 에러\n");
        exit(1);
    }
    else return s->stack[(s->top)--]; //스택이 비어있지 않다면 스택의 최상단에 있었ㄸ너 데이터를 출력하고 top변수를 1만큼 감소시켜 데이터를 제거했음을 기록한다.
}

int peek(StackType *s){
    //스택의 맨 상단에 위치한 원소를 출력해주는 함수이다.
    if(is_empty(s)){
        fprintf(stderr, "스택 공백 에러\n");
        exit(1);
    }
    else return s->stack[(s->top)];
}

int prec(char c){
    //operator의 우선 순위를 기록한 함수이다.
    int order = 0;
    switch(c){
        case '+':
        case '-':
            order = 1;
            break;
        case '*':
        case '/':
        case '%':
            order = 2;
            break;
        case '^':
            order = 3;
            break;
    }
    return order;
}

char* infix_to_postfix(char exp[]){
    //infix를 postfix로 변환하는 함수이다.
    
    char ch, top_op;
    unsigned int len = strlen(exp);
    StackType s_operator;
    
    char result[MAX_STACK_SIZE] ="";
    int index = 0;
    
    init(&s_operator);
    
    for(int i = 0; i < len; i++){ //문자열을 한 글자씩 읽는다.
        
        ch = exp[i];
        
        switch(ch){
            case '+': case '-': case '*': case '/': case '%': case '^': //연산 기호일 경우, 우선 순위를 확인하고 스택 내에 글자보다 우선 순위가 높을 경우, 출력을 하고 글자를 스택에 저장한다.
                while(!is_empty(&s_operator) && (prec(ch) <= prec(peek(&s_operator)))){
                    result[index++] = pop(&s_operator);
                }
                push(&s_operator, ch);
                break;
            case '(':
                //왼쪽 괄호일 경우, 스택에 저장한다.
                push(&s_operator, ch);
                break;
            case ')':
                //오른쪽 괄호일 경우, 괄호 내의 연산을 표현하기 위해 왼쪽 괄호가 나타날 때까지 스택 내의 연산자를 출력한다.
                top_op = pop(&s_operator);
                
                while(top_op != '('){
                    result[index++] = top_op;
                    top_op = pop(&s_operator);
                }
                break;
            default:
                //숫자의 경우, 바로 출력을 한다.
                result[index++] = ch;
                break;
        }
    }

    while(!is_empty(&s_operator)){
        result[index++] = pop(&s_operator);
    }
    
    return result;
}

int operation(int a, int b, char oper){
    //사칙 연산과 모듈러 연산, 제곱 연산에 대해 연산을 수행하고, 결과를 출력하는 함수
    
    int result = 0;
    
    switch (oper) {
        case '+':
            result = b+a;
            break;
        case '-':
            result = b-a;
            break;
        case '*':
            result = b*a;
            break;
        case '/':
            result = b/a;
            break;
        case '%':
            result = b%a;
            break;
        case '^':
            result = pow(b, a);
            break;
        default:
            break;
    }
    
    return result;
}

int calculation(char exp[]){
    //postfix로 표현된 문자열을 입력받았을 경우, 연산을 수행하고 결과를 출력하는 함수

    StackType s_result; //피연산자들을 저장하는 스택
    
    for(int i = 0; i < strlen(exp); i++){
        switch(exp[i]){
            case '+': case '-': case '*': case '/': case '%': case '^':
                //연산자의 경우, 피연산자 스택 내에 top에 위치한 두 숫자를 가지고 연산을 수행한 다음, 결과를 스택에 저장한다.
                push(&s_result, operation(pop(&s_result), pop(&s_result), exp[i]));
                break;
            default:
                //피연산자의 경우 스택에 저장한다.
                push(&s_result, (int)exp[i]-48);
                break;
        }
    }
    return pop(&s_result);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    FILE *pFile = NULL; //파일을 입력받기 위해 파일 포인터를 선언한다.
    int line = 0;
    
    pFile = fopen("/Users/yeji/Desktop/학교/2017년 2학기/자료구조/과제/Programming Report1/Programming Report1/infix3.txt", "r"); //파일을 입력받는다.
    if(pFile != NULL){
        //파일이 존재하면 파일 내용을 읽어온다.
        fscanf(pFile, "%d\n", &line); //파일의 첫번째 줄은 데이터의 개수이므로 개수를 입력받는다.
        
        for(int i = 0; i < line; i++){
            char s[MAX_STACK_SIZE]; //infix 데이터를 입력받을 char배열 변수 s
            char newarray [MAX_STACK_SIZE] = {}; //infix를 postfix로 바꾼 다음 저장할 char배열 변수 newarray
            
            fscanf(pFile, "%s", s); //infix 데이터를 입력받는다.(끝에 ;포함)
            
            char * array = strtok(s, ";"); //infix데이터를 ;를 delimeter로 사용하여 infix 데이터만을 추출한다.
            strncpy(newarray, infix_to_postfix(array), strlen(infix_to_postfix(array))); //infix를 postfix로 변환한 다음 postfix 데이터를 복사하여 저장한다.
            
            printf("infix notation = %s\n", s); //infix 데이터를 출력한다.
            printf("postfix notation = %s\n", newarray); //postfix 데이터를 출력한다.
            printf("value = %d\n", calculation(newarray)); //postfix 데이터를 이용하여 연산을 수행한 다음 결과값을 출력한다.
            printf("\n"); //가시성을 높이기 위해 데이터 사이에 빈 공백열을 넣어준다.
        }
    }
    else{
        printf("파일 읽기 오류"); //파일이 존재하지 않을 경우, 오류가 떴음을 알린다.
    }
    
    fclose(pFile); //파일을 닫는다.
}
