/**
 * Esse arquivo tem como objetivo a implementação de um brinquedo em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "toy.h"
#include "shared.h"

static toy_args *toys_data = NULL;

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args)
{
    // Typecasting
    toy_t *toy = (toy_t *)args;

    debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id);

    int waiting_for_toy;

    while (TRUE)
    {

        // Verifica se tem alguem no parque
        pthread_mutex_lock(&park_remain_mutex);
        if (park_remain < 0)
        {
            pthread_mutex_unlock(&park_remain_mutex);
            break;
        }
        pthread_mutex_unlock(&park_remain_mutex);

        // Espera por clientes chegarem
        sleep(3);

        // Verifica se tem alguem na fila do brinquedo
        sem_wait(&toy->waiting_for_toy_sem);

        // brinquedo passa quantidade de turistas na fila do brinquedo para a variável local
        waiting_for_toy = toy->waiting_for_toy;

        // Limita a capacidade do brinquedo
        if (waiting_for_toy > toy->capacity)
            waiting_for_toy = toy->capacity;

        // Se não tiver ninguém na fila, libera o semáforo e continua esperando
        if (!waiting_for_toy)
        {
            sem_post(&toy->waiting_for_toy_sem);
            continue;
        }

        debug("Brinquedo %d funcionando com %d turistas.\n", toy->id, waiting_for_toy);

        // Libera os turistas do brinquedo
        for (int i = 0; i < waiting_for_toy; i++)
        {
            sem_post(&toy->enjoy_sem);
            sem_wait(&toy->waiting_for_toy_sem);
        }

        // tira os turistas da fila do brinquedo
        toy->waiting_for_toy -= waiting_for_toy;
        sem_post(&toy->waiting_for_toy_sem);

        debug("Término execução brinquedo [%d].\n", toy->id);
    }
    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id);

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args)
{
    // Args em uma variável global para poder ser usada em outras funções
    toys_data = args;

    // Criação das threads dos brinquedos
    for (int i = 0; i < args->n; ++i)
    {
        pthread_create(&args->toys[i]->thread, NULL, turn_on, (void *)args->toys[i]);
    }
}

// Desligando os brinquedos
void close_toys()
{
    // Join nas threads dos brinquedos
    for (int i = 0; i < toys_data->n; ++i)
    {
        pthread_join(toys_data->toys[i]->thread, NULL);
    }
}
