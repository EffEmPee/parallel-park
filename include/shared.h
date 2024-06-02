#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

extern pthread_mutex_t gate_queue_mutex;
extern sem_t queue_sem;
extern sem_t *toys_sem;
extern int queue_remain;
extern int park_remain;
extern pthread_mutex_t park_remain_mutex;

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;

#endif