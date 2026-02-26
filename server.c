#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_prodcons.h"
#include "header_msg.h"

void server(int id_queue_server) {

    /* TBD: Creare e inizializzare l'oggetto monitor */

    BufferProdCons * b = (BufferProdCons *) malloc(sizeof(BufferProdCons));
    if(b == NULL){
        perror("errore malloc");
        exit(1);
    }

    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->prod_ok, NULL);
    pthread_cond_init(&b->cons_ok, NULL);

    b->valore = 0;
    b->stato = VUOTO;

    pthread_t visualizzatore;

    /* TBD: Creare un thread, 
            e fargli eseguire la funzione "visualizza()",
            passandogli in ingresso l'oggetto monitor
     */

    int err;

    err = pthread_create(&visualizzatore, NULL, visualizza, (void *)m);
    if(err != 0){
        perror("errore create visualizzatore");
        exit(1);
    }


    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        int somma_valori = 0;
        int media = 0;

        for(int j=0; j<NUM_COLLETTORI; j++) {

            int id_collettore = j+1;

            messaggio_collettore m;

            /* TBD: Ricevere un messaggio da un collettore, con ricezione selettiva */

            err = msgrcv(id_collettore, &m, sizeof(messaggio_collettore) - sizeof(messaggio_collettore), 2, 0);
            if(err < 0){
                perror("errore msgrcv collettore");
                exit(1);
            }

            int valore = m.valore;

            printf("[SERVER] Ricevuto valore=%d\n", valore);

            somma_valori += valore;

        }

        media = somma_valori / NUM_COLLETTORI;

        /* TBD: Chiamare la funzione "produci_valore()",
                passandogli la variabile "media"
        */

        produci_valore(b, media);

    }


    /* TBD: Attendere la terminazione del thread, de-allocare il monitor */

    pthread_join(visualizzatore, NULL);

    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->prod_ok);
    pthread_cond_destroy(&b->cons_ok);

    free(m);
}

void * visualizza(void * p) {

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        /* TBD: Utilizzare la funzione "consuma_valore()" */

        int valore = consuma_valore(p);

        printf("[VISUALIZZA] Valore=%d\n", valore);
    }

    return NULL;
}


void produci_valore(BufferProdCons * b, int valore) {

    /* TBD: Completare questa funzione introducendo la sincronizzazione */

    pthread_mutex_lock(&b->mutex);

    while(b->stato == PIENO){
        pthread_cond_wait(&b->prod_ok, &b->mutex);
    }

    b->valore = valore;
    b->stato = PIENO;

    pthread_cond_signal(&b->cons_ok);
    pthread_mutex_unlock(&b->mutex);

}

int consuma_valore(BufferProdCons * b) {

    /* TBD: Completare questa funzione introducendo la sincronizzazione */

    pthread_mutex_lock(&b->mutex);

    while(b->stato == VUOTO){
        pthread_cond_wait(&b->cons_ok, &b->mutex);
    }

    int valore = b->valore;
    b->stato = VUOTO;

    pthread_cond_signal(&b->prod_ok);
    pthread_mutex_unlock(&b->mutex);

    return valore;
}
