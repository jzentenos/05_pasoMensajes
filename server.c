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
    message_buf rbuf, sbuf;

    // Crea la cola compartida
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget server");
        exit(1);
    }
    printf("Servidor iniciado. Esperando clientes...\n");

    // Atiende peticiones de dos clientes
    for (int i = 0; i < 2; i++) {
        // Lee del canal común de peticiones (mtype = 1)
        if (msgrcv(msqid, &rbuf, sizeof(message_buf) - sizeof(long), 1, 0) < 0) {
            perror("msgrcv server");
            exit(1);
        }

        printf("Servidor: Recibido de Cliente %d: '%s'\n", rbuf.sender_id, rbuf.mtext);

        // Define canal de respuesta privado según el cliente
        if (rbuf.sender_id == 1) {
            sbuf.mtype = 10;
            sprintf(sbuf.mtext, "Hola Cliente 1, tu respuesta privada");
        } else {
            sbuf.mtype = 20;
            sprintf(sbuf.mtext, "Hola Cliente 2, tu respuesta privada");
        }
        sbuf.sender_id = 0; // 0 = Servidor

        if (msgsnd(msqid, &sbuf, sizeof(message_buf) - sizeof(long), 0) < 0) {
            perror("msgsnd server");
            exit(1);
        }
    }

    printf("Servidor terminó de atender a ambos clientes.\n");
    return 0;
}