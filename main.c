#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define NUM_THREADS 5
#define TOTAL_IDS 1000000

int ids[TOTAL_IDS];
int pos_atual = 0;
pthread_mutex_t mutex;

// API falsa
void simular_api(int id, char *saida_json) {
    double valor = (rand() % 100000) / 100.0;
    sprintf(saida_json, "{\"id\":%d,\"status\":\"ok\",\"valor\":%.2f}", id, valor);
}

void *processar_ids(void *arg) {
    long tid = (long)arg;
    FILE *logfile = fopen("logs.txt", "a");
    if (!logfile) {
        perror("Erro ao abrir logs.txt");
        pthread_exit(NULL);
    }

    while (1) {
        int id_atual;

        pthread_mutex_lock(&mutex);
        if (pos_atual >= TOTAL_IDS) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        id_atual = ids[pos_atual++];
        pthread_mutex_unlock(&mutex);

        char resposta[200];
        simular_api(id_atual, resposta);

        time_t agora = time(NULL);
        struct tm *t = localtime(&agora);
        char data[40];
        strftime(data, sizeof(data), "%Y-%m-%d %H:%M:%S", t);

        fprintf(logfile, "Thread %ld, ID %d, Resposta %s, Data %s\n",
                tid, id_atual, resposta, data);
        fflush(logfile);
    }

    fclose(logfile);
    pthread_exit(NULL);
}
//Função P1
void executar_processo_p1() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);
    srand(time(NULL));

    for (int i = 0; i < TOTAL_IDS; i++) {
        ids[i] = i + 1;
    }

    printf("P1: %d IDs gerados. Iniciando threads...\n", TOTAL_IDS);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, processar_ids, (void *)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    printf("P1: processamento concluído.\n");
}

//Processo P0
int main() {
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    pid_t pid = fork();

    if (pid < 0) {
        perror("Erro ao criar processo P1");
        exit(1);
    }

    if (pid == 0) {
        // Processo filho (P1)
        executar_processo_p1();
        exit(0);
    } else {
        // Processo pai (P0)
        printf("P0: aguardando término de P1...\n");
        wait(NULL);
        printf("P0: processo P1 finalizado com sucesso.\n");
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_total = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("Tempo total: %.2f segundos\n", tempo_total);

    return 0;
}
