#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 1234;
    message_buf sbuf, rbuf;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // 1. Recibir mensaje inicial (mtype = 1)
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv paso 1");
        exit(1);
    }
    printf("Process 2 recibió: %s\n", rbuf.mtext);

    // 2. Responder "Loud and clear" (mtype = 2)
    sbuf.mtype = 2;
    strcpy(sbuf.mtext, "Loud and clear");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd paso 2");
        exit(1);
    }
    printf("Process 2 respondió: %s\n", sbuf.mtext);

    // 3. Recibir réplica final (mtype = 3)
    if (msgrcv(msqid, &rbuf, MSGSZ, 3, 0) < 0) {
        perror("msgrcv paso 3");
        exit(1);
    }
    printf("Process 2 recibió réplica: %s\n", rbuf.mtext);

    return 0;
}