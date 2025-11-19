<img width="544" height="168" alt="image" src="https://github.com/user-attachments/assets/76175d05-7661-4407-af17-721358b9f693" />

1. (echo START1; sleep 10; echo DONE1) & : START1을 출력 후, 10초간 대기하며 echo DONE1를 출력한다. 
2. (echo START2; sleep 10; echo DONE2) & : START2을 출력 후, 10초간 대기하며 echo DONE2를 출력한다.
3. kill -KILL 첫 번째 프로세스 : 첫 번째 프로세스에 강제 종료 시그널을 보내서 해당 프로세스를 즉시 종료시킨다.(echo START1; sleep 10; echo DONE1)
4. kill -STOP 첫 번째 프로세스 : 

