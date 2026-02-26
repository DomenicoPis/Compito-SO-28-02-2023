#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_msg.h"


void collettore(int id_collettore, int id_queue_collettore, int id_queue_server) {

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        int somma_valori = 0;
        int media = 0;

        for(int j=0; j<NUM_SENSORI_PER_COLLETTORE; j++) {

            int id_sensore = j+1;

            messaggio_sensore m;

            /* TBD: Ricevere un messaggio da un sensore, con ricezione selettiva */

            int err;

            err = msgrcv(id_sensore, &m, sizeof(messaggio_sensore) - sizeof(long), 1, 0);
            if(err < 0){
                perror("errore msgrcv");
                exit(1);
            }

            int valore = m.valore;

            printf("[COLLETTORE] Ricevuto valore=%d\n", valore);

            somma_valori += valore;
        }


        media = somma_valori / NUM_SENSORI_PER_COLLETTORE;

        messaggio_collettore m;

        /* TBD: Inviare "media" al processo server */

        m.mtype = 2;
        m.valore = media;

        int err;

        err = msgsnd(id_collettore, &m, sizeof(messaggio_collettore) - sizeof(long), 0);
        if(err < 0){
            perror("errore msgsnd");
            exit(1);
        }

        printf("[COLLETTORE] Inviato valore=%d\n", media);


    }
}
