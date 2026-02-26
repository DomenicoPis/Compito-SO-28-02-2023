#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_msg.h"

void sensore(int id_sensore, int id_queue_collettore) {

    srand(getpid());

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        messaggio_sensore m;

        int valore = (rand() % 10) + 10;

        /* TBD: Inviare "valore" mediante messaggio */

        m.mtype = id_sensore;
        m.valore = valore;

        int err;

        err = msgsnd(id_queue_collettore, &m, sizeof(messaggio_sensore) - sizeof(long), 0);
        if(err < 0){
            perror("errore msgnd sensore");
            exit(1);
        }

        printf("[SENSORE] Inviato valore=%d\n", valore);


        sleep(3);
    }
}