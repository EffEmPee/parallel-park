#include "shared.h"

#include <queue.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

pthread_mutex_t gate_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t queue_sem;

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;