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

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // 1. Enviar "Are you hearing me?" (mtype = 1)
    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Are you hearing me?");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd paso 1");
        exit(1);
    }
    printf("Process 1 envió: %s\n", sbuf.mtext);

    // 2. Esperar respuesta de Process 2 (mtype = 2)
    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0) {
        perror("msgrcv paso 2");
        exit(1);
    }
    printf("Process 1 recibió: %s\n", rbuf.mtext);

    // 3. Enviar réplica "I can hear you too" (mtype = 3)
    sbuf.mtype = 3;
    strcpy(sbuf.mtext, "I can hear you too");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd paso 3");
        exit(1);
    }
    printf("Process 1 replicó: %s\n", sbuf.mtext);

    return 0;
}