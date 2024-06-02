/**
 * Esse arquivo tem como objetivo a implementação da bilheteria em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS.
 */
#include <stdio.h>
#include <stdlib.h>
#include <tickets.h>
#include <unistd.h>

#include <queue.h>
#include "shared.h"

static tickets_args *tickets_data = NULL;

// Thread que implementa uma bilheteria
void *sell(void *args)
{
    ticket_t *ticket = (ticket_t *)args;
    debug("[INFO] - Bilheteria [%d] Abriu!\n", ticket->id);

    while (1)
    {
        pthread_mutex_lock(&gate_queue_mutex);
        if (is_queue_empty(gate_queue))
        {
            pthread_mutex_unlock(&gate_queue_mutex);
            if (queue_remain <= 0) break;
            continue;
        }

        dequeue(gate_queue);
        queue_remain--;
        pthread_mutex_unlock(&gate_queue_mutex);

        sem_post(&queue_sem);
    }

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args)
{
    tickets_data = args;
    for (int i = 0; i < args->n; ++i)
    {
        pthread_create(&args->tickets[i]->thread, NULL, sell, (void *)args->tickets[i]);
    }
}

// Essa função deve finalizar a bilheteria
void close_tickets()
{
    for (int i = 0; i < tickets_data->n; ++i)
    {
        pthread_join(tickets_data->tickets[i]->thread, NULL);
    }
}