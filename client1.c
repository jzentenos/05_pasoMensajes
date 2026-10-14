#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    int sender_id;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 5678;
    message_buf sbuf, rbuf;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget client1");
        exit(1);
    }

    // Enviar solicitud al canal del servidor (mtype = 1)
    sbuf.mtype = 1;
    sbuf.sender_id = 1;
    strcpy(sbuf.mtext, "Mensaje desde Cliente 1");
    msgsnd(msqid, &sbuf, sizeof(message_buf) - sizeof(long), 0);
    printf("Cliente 1 envió petición.\n");

    // Esperar respuesta exclusivamente en su canal privado (mtype = 10)
    msgrcv(msqid, &rbuf, sizeof(message_buf) - sizeof(long), 10, 0);
    printf("Cliente 1 recibió respuesta: %s\n", rbuf.mtext);

    return 0;
}