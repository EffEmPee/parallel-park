#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

extern pthread_mutex_t gate_queue_mutex;  // Protege o acesso à fila de entrada do portão principal
extern sem_t queue_sem;                   // Sincroniza a liberação dos clientes da fila do portão principal
extern sem_t *toys_sem;                   // Ponteiro para matriz de semáforos, cada semáforo referente a um brinquedo
extern int queue_remain;                  // Variável de controle de quantos clientes ainda estão esperando na fila do portão principal
extern int park_remain;                   // Variável de controle de quantos clientes ainda estão no parque
extern pthread_mutex_t park_remain_mutex; // Protege o acesso a variável park_remain

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;

#endif