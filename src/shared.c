#include "shared.h"

#include <queue.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

pthread_mutex_t gate_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t *toys_sem;
sem_t queue_sem;
int queue_remain;
int park_remain = 0;
pthread_mutex_t park_remain_mutex = PTHREAD_MUTEX_INITIALIZER;

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;