#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

void addq(struct node *head, struct node *tail, int data);
int delete(struct node *head, struct node *tail);
void print_queue(struct node *head);

int main() {
    // 1. 더미 헤드 노드 생성 (데이터 저장 안 함, 리스트 시작점)
    struct node *head = (struct node *)malloc(sizeof(struct node));
    head->next = NULL;

    // 2. 더미 테일 관리 노드 생성 (데이터 저장 안 함, 마지막 노드 위치를 기억함)
    // 초기 상태: 큐가 비었으므로 마지막 노드는 head 자신임
    struct node *tail = (struct node *)malloc(sizeof(struct node));
    tail->next = head; 

    int input;
    // 정수가 입력되는 동안 계속 addq 실행, 문자 입력 시 종료
    while (scanf("%d", &input) == 1) {
        addq(head, tail, input);
    }

    // 문자(예: x) 입력 등으로 루프 종료 시 큐 출력
    // 참고: 스크린샷에서는 마지막 입력값 '1'이 출력에 빠져 있는 것으로 보이나,
    // 정상적인 로직이라면 입력된 모든 값이 큐에 있어야 합니다.
    // 이 코드는 입력된 모든 값을 정확히 출력합니다.
    char trash;
    scanf("%c", &trash); // 버퍼에 남은 문자 처리
    
    printf("Print queue\n");
    print_queue(head);

    // 메모리 해제
    while (head->next != NULL) {
        delete(head, tail);
    }
    free(head);
    free(tail);

    return 0;
}

void addq(struct node *head, struct node *tail, int data) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->next = NULL;

    // tail->next는 '현재의 마지막 노드'를 가리킴
    struct node *last = tail->next;
    
    // 연결 리스트 연결
    last->next = new_node;
    
    // tail 정보 갱신 (새 노드가 이제 마지막 노드임)
    tail->next = new_node;
}

int delete(struct node *head, struct node *tail) {
    if (head->next == NULL) return -1;

    struct node *temp = head->next;
    int data = temp->data;

    head->next = temp->next;

    // 중요: 큐가 비게 되면 tail 포인터가 다시 head를 가리키도록 리셋
    if (head->next == NULL) {
        tail->next = head;
    }

    free(temp);
    return data;
}

void print_queue(struct node *head) {
    struct node *curr = head->next;
    while (curr != NULL) {
        printf("%d\n", curr->data);
        curr = curr->next;
    }
}
