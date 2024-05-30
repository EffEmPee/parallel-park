/**
 * Esse arquivo tem como objetivo a implementação do cliente em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "client.h"
#include "queue.h"
#include "shared.h"


void *enjoy(void *arg);
void buy_coins(client_t *self);
void wait_ticket(client_t *self);
void queue_enter(client_t *self);

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){

    client_t* client = (client_t*) arg;

    queue_enter(client);

    // TODO

    debug("[EXIT] - O turista saiu do parque.\n");
    pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    // Adiciona um número aleatório de moedas, repeitando o máximo
    self->coins = rand() % MAX_COINS;
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    // sem_wait(&queue_sem);
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    // Protege o acesso a fila para adicionar um client
    pthread_mutex_lock(&gate_queue_mutex);
    enqueue(gate_queue, self->id);
    pthread_mutex_unlock(&gate_queue_mutex);

    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);

    wait_ticket(self);

    // Sua lógica aqui.
    buy_coins(self);

    // Sua lógica aqui.
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    // Inicialização do mutex de fila e criação das threads de ca
    pthread_t id_thread[args->n];
    for (int i = 0; i < args->n; ++i)
    {
        pthread_create(&id_thread[i], NULL, enjoy, (void *) args->clients[i]);
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
    // TODO
}
