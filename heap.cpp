//
//  main.cpp
//  Programming Report2
//
//  Created by 강예지 on 2017. 11. 1..
//  Copyright © 2017년 강예지. All rights reserved.
//

#include <iostream>
#include <stdlib.h>

#define MAX_ELEMENT 30

typedef struct {
    //각 원소는 key값을 갖는다.
    int key;
} element;

typedef struct{
    //원소로 이루어진 힙은 배열로 구현한다.
    element heap[MAX_ELEMENT+1];
    int heap_size; //힙의 사이즈
} HeapType;

void print_heap(HeapType *h); //힙을 힙의 모형으로 출력하는 함수

void init(HeapType *h){
    //힙 초기화
    h -> heap_size=0;
}

void order_min_heap(HeapType *h, int parent){
    //힙을 재귀적으로 반정렬시키는 데에 사용하는 함수
    //자식 노드와 비교할 원소의 index를 매개변수를 parent라는 매개변수로 입력받아 자식노드와 값을 비교한 후에, 자식노드가 자신보다 더 값이 작을 경우, 자식노드와 위치를 바꾼다.

    int leftchild = 2*parent; //왼쪽 자식노드의 index
    int rightchild = 2*parent+1; //오른쪽 자식노드의 index
    
    if(leftchild <= h->heap_size && rightchild <= h->heap_size && h->heap[parent].key <= h->heap[leftchild].key && h->heap[parent].key <= h->heap[rightchild].key){
        //왼쪽 자식노드와 오른쪽 자식노드가 있고,
        //두 자식노드보다 해당 부모 노드의 값이 작을 경우는 그대로 return
        return;
    }
    else if(h->heap[parent].key > h->heap[leftchild].key && leftchild <= h->heap_size){
        //왼쪽 자식노드가 존재하고 값이 부모노드보다 클 경우
        //왼쪽 자식노드와 부모노드의 값을 교환
        int temp = h->heap[leftchild].key;
        h->heap[leftchild].key = h->heap[parent].key;
        h->heap[parent].key = temp;
        
        order_min_heap(h, leftchild); //내려간 노드를 leat node가 아닐 때까지 해당 함수를 재귀적으로 반복
        order_min_heap(h, parent); //바뀐 부모노드의 원소 값이 왼족 자식노드와 오른쪽 자식노드보다 작은지 재귀 함수를 호출하여 확인
    }
    else if(h->heap[parent].key > h->heap[rightchild].key &&  rightchild <= h->heap_size){
        //오른쪽 자식노드가 존재하고 값이 부모노드보다 클 경우
        //오른쪽 자식노드와 부모노드의 값을 교환
        int temp = h->heap[rightchild].key;
        h->heap[rightchild].key = h->heap[parent].key;
        h->heap[parent].key = temp;
        
        order_min_heap(h, rightchild); //내려간 노드를 leat node가 아닐 때까지 해당 함수를 재귀적으로 반복
        order_min_heap(h, parent); //바뀐 부모노드의 원소 값이 왼족 자식노드와 오른쪽 자식노드보다 작은지 재귀 함수를 호출하여 확인
    }
    else return; //이 외의 경우에는 그대로 return
}

element delete_min_heap(HeapType *h){
    //부모노드와
    int parent, child;
    element item, temp;
    
    item = h->heap[1]; //min 값
    temp = h->heap[(h->heap_size)--]; //마지막 ㅣeaf node의 값
    
    parent = 1; child = 2;
    
    while(child <= h->heap_size){ //자식 노드가 존재할 경우
        if( (child < h->heap_size) && (h->heap[child].key > h->heap[child+1].key)) child++; //왼쪽 자식노드보다 오른쪽 자식노드가 작을 경우 자식 노드를 오른쪽 자식노드로 변경
        if(temp.key <= h->heap[child].key) break; //마지막 leaf node의 값이 자식 노드의 값보다 작을 경우
        h->heap[parent] = h->heap[child]; //자식 노드를 부모 노드로 옮긴다
        parent = child; //노드를 하나씩 내려가면서 해당 작업 반복
        child *= 2;
    }
    h->heap[parent] = temp;
    
    return item; //최솟값 출력
}

void print_heap(HeapType *h){
    //힙을 힙의 모형으로 출력하는 함수
    
    int i = 1; // 각 높이별 출력해야 할 원소의 개수
    int print_index = 1; //각 높이별 시작하는 원소의 배열에서의 index
    int rest_num = h->heap_size; //출력하고 남은 노드의 수
    
    while(i < rest_num){ //출력해야할 남은 노드의 수가 각 높이별 출력해야할 원소의 개수보다 많은 경우는 각 높이에서 출력해야할 원소를 그대로 출력하면 된다
        
        for(int j = print_index; j < print_index + i; j++){
            //출력을 시작할 위치부터 출력하기 시작
            printf("%d  ", h->heap[j].key);
        }
        printf("\n");
        
        rest_num -= i; //출력한 수만큼 남은 노드의 수에서 뺀다
        print_index += i; //출력한 수만큼 출력해야할 index에 더해준다
        i *= 2;
    }
    
    for(int j = 0; j < rest_num; j++ ){
        printf("%d  ", h->heap[j+print_index].key);
        //출력하지 못한 남은 노드들의 값을 출력한다.
    }
    printf("\n");
}

int main(int argc, const char * argv[]) {
    // insert code here...

    HeapType h;
    init(&h);
    srand(time(NULL));
    
    element e ;
    e.key = 0;
    h.heap[0] = e; //힙의 index 0위치의 값은 빈 원소로 간주
    
    for(int i = 1; i <= MAX_ELEMENT; i++){
        //힙을 난수로 채워넣는다.
        element e;
        e.key= rand()%100 + 1;
        h.heap[i] = e;
        h.heap_size++;
    }
    
    printf("Input : "); //힙에 집어넣은 원소들을 출력
    for(int i = 1; i < MAX_ELEMENT; i++){
        printf("%d  ", h.heap[i].key);
    }
    printf("\n");
    
    printf("\n초기 생성된 완전 이진트리\n"); //힙을 힙 형태로 출력
    print_heap(&h);
    printf("\n");
    
    for(int i = (h.heap_size/2); i >= 1; i--){
        //힙 배열의 반은 leaf node이므로 leaf node가 아닌 배열의 index부터 root node까지 down heap 시킨다
        order_min_heap(&h, i);
    }
    
    printf("\n구성된 HEAP\n"); //반정렬한 힙 출력
    print_heap(&h);
    printf("\n");
    
    printf("\nresult : ");
    for(int i = 1; i < MAX_ELEMENT; i++){
        //힙정렬을 이용한 결과 출력
        element e = delete_min_heap(&h);
        printf("%d  ", e.key);
    }
    printf("\n");
    
    return 0;
}
