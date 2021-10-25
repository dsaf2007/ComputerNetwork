#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

#define maxFork 8 // 전체 fork

int main(void)
{
    int pipe_[2]; //파이프
    int buffer[8]; //전송 받은 값
    int sum = 0 , bit = 0; // 합, 전송비트.

    if(pipe(pipe_) == -1)// 파이프 생성 오류
    {
        perror("pipe Fail");
        exit(1);
    }

    int ** chipSeq = (int**)malloc(sizeof(int*)*4);//칩시퀀스 저장할 4*8 배열
    for(int i = 0; i < 8; i++)
    {
        chipSeq[i] = (int*)malloc(sizeof(int*)*8);
    }

    int * sumOfChipSeq = (int*)malloc(sizeof(int)*8);//칩 시퀀스의 합
    for(int i = 0; i < 8; i++)
    {
        sumOfChipSeq[i] = 0 ; //initialize to 0
    }

    for(int i = 0; i < 4; i++)
    {
        printf("enter %dth chip sequence : ", i);
        for(int j = 0; j < 8; j++)
        {
            scanf("%d",&chipSeq[i][j]);
        }
        printf("\n");
    }

    pid_t pid[maxFork];

    for(int i =0; i < 4; i++)
    {
        pid[i] = fork(); // create child process

        if(pid[i] < 0)
        {
            perror("Fork Failed\n"); //child process 생성 실패시
            exit(1);
        }
        else if(pid[i] == 0) //if child process
        {
            printf("enter %dth bit to send : ", i);
            scanf("%d", &bit); printf("\n");

            if(bit ==0)
            {
                //* -1 to chip sequence
                for(int j = 0; j < 8; j++)
                {
                    chipSeq[i][j] *= -1;
                }
            }

            write(pipe_[1],chipSeq[i],32); // send chip sequnce to parent process
            exit(1);//end process
        }
        else // parent process
        {
            read(pipe_[0],buffer,32);
            sum = 0;

            for(int j = 0; j < 8; j++)
            {
                sumOfChipSeq[j] += buffer[j]; // sum of chip sequence
            }
            wait(NULL); // wait untill all child process ends
        }
    }

    //Joiner에서 계산한 sum 출력
    printf("sum calculated by Joiner : ");
    for(int i = 0; i < 8; i++)
    {
        printf("%d\t", sumOfChipSeq[i]);
    }printf("\n\n");

    for(int i = 4; i < 8; i++)
    {
        pid[i] = fork(); // create child process

        if(pid[i] < 0) // child process error
        {
            perror("Fork Failed\n");
            exit(1);
        }
        else if(pid[i] == 0)//child process
        {
            read(pipe_[0], buffer, 32);
            sum = 0;
            for(int j = 0; j < 8; j++)
            {
                buffer[j] *= chipSeq[i-4][j]; // sum * sequence
                sum += buffer[j]; // add sequence
            }
            printf("%dth recieved bit : %d\n",i-4, sum / 8);
            printf("\n");
            exit(1); // end process
        }
        else //parent process
        {
            write(pipe_[1], sumOfChipSeq, 32);
            wait(NULL); // wait untill child process ends;
        }
    }
    return 0;
}

