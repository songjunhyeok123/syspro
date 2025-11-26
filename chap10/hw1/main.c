#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

// 함수 프로토타입 선언
void push(struct node *top, int data);
int pop(struct node *top);
void print_stack(struct node *top);

int main() {
    // 더미 헤드 노드 생성 및 초기화
    struct node *head = (struct node *)malloc(sizeof(struct node));
    head->next = NULL;

    int input;
    // scanf가 정수를 정상적으로 읽으면 1을 반환, 실패하면(문자, 소수점 등) 0 반환
    while (scanf("%d", &input) == 1) {
        push(head, input);
    }

    // 입력 루프가 끝나면 스택 출력
    printf("Print stack\n");
    print_stack(head);

    // 메모리 해제 (선택 사항이지만 권장됨)
    while (head->next != NULL) {
        pop(head);
    }
    free(head);

    return 0;
}

void push(struct node *top, int data) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->next = top->next;
    top->next = new_node;
}

int pop(struct node *top) {
    if (top->next == NULL) {
        return -1;
    }
    struct node *temp = top->next;
    int data = temp->data;
    top->next = temp->next;
    free(temp);
    return data;
}

// 스택 전체 출력 함수 (재귀 또는 반복문 사용)
void print_stack(struct node *top) {
    struct node *curr = top->next; // 더미 노드 다음부터 출력
    while (curr != NULL) {
        printf("%d\n", curr->data);
        curr = curr->next;
    }
}
