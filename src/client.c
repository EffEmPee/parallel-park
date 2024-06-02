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

static client_args *client_data = NULL;

void *enjoy(void *arg);
void buy_coins(client_t *self);
void wait_ticket(client_t *self);
void queue_enter(client_t *self);

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg)
{

    client_t *client = (client_t *)arg;

    queue_enter(client);

    while (client->coins > 0)
    {
        toy_t *toy = client->toys[rand() % client->number_toys];

        sem_post(&toy->queue_sem);
        debug("Turista [%d] entrou na fila do brinquedo [%d].\n", client->id, toy->id);
        sem_wait(&toy->enjoy_sem);
        debug("Turista [%d] brincou no brinquedo [%d].\n", client->id, toy->id);
        client->coins--;
    }

    pthread_mutex_lock(&park_remain_mutex);
    park_remain--;
    pthread_mutex_unlock(&park_remain_mutex);
    debug("[EXIT] - O turista [%d] saiu do parque.\n", client->id);
    pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self)
{
    // Adiciona um número aleatório de moedas, repeitando o máximo
    self->coins = rand() % MAX_COINS;
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self)
{
    sem_wait(&queue_sem);
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self)
{
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
void open_gate(client_args *args)
{
    client_data = args;
    queue_remain = args->n;
    park_remain = args->n - 1;
    // Inicialização do mutex de fila e criação das threads de ca
    for (int i = 0; i < args->n; ++i)
    {
        pthread_create(&args->clients[i]->thread, NULL, enjoy, (void *)args->clients[i]);
    }
}

// Essa função deve finalizar os clientes
void close_gate()
{
    for (int i = 0; i < client_data->n; ++i)
    {
        pthread_join(client_data->clients[i]->thread, NULL);
    }
}
