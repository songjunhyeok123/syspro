<img width="544" height="168" alt="image" src="https://github.com/user-attachments/assets/76175d05-7661-4407-af17-721358b9f693" />

1. (echo START1; sleep 10; echo DONE1) & : START1을 출력 후, 10초간 대기하며 echo DONE1를 출력한다. 
2. (echo START2; sleep 10; echo DONE2) & : START2을 출력 후, 10초간 대기하며 echo DONE2를 출력한다.
3. kill -KILL 첫 번째 프로세스 : 첫 번째 프로세스에 강제 종료 시그널을 보내서 해당 프로세스를 즉시 종료시킨다.(echo START1; sleep 10; echo DONE1)
4. kill -STOP 두 번째 프로세스 : 두 번째 프로세스를 정지 시켜 멈춘 상태가 된다.
5. kill -CONT 두 번째 프로세스 : 멈춘 상태인 두 번째 프로세스를 다시 실행 상태로 돌려보낸다.

실습에는 4번과 5번이 첫 번째 프로세스로 되어있지만, 이미 3번에서 첫번째 프로세스가 강제종료가 되기에, 두 번째 프로세스로 작업제어 실습을 진행하였습니다.
