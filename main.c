#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define NUM_THREADS 5
#define MAX_IDS 1000000 

int lista_ids[MAX_IDS];
int NUM_IDS = 0;
int indice_atual = 0;

pthread_mutex_t mutex_ids;
pthread_mutex_t mutex_log;

// Função que simula a API
void consulta_api_simulada(int id, char *saida) {
    sprintf(saida, "{\"id\":%d,\"status\":\"ok\",\"valor\":%d}", id, rand() % 1000);
}

// Função para escrever no arquivo de log
void grava_log(int thread_id, int id, const char *json) {
    pthread_mutex_lock(&mutex_log);
    FILE *f = fopen("logs.txt", "a");
    if (f) {
        time_t agora = time(NULL);
        struct tm *t = localtime(&agora);
        fprintf(f, "Thread %d, ID %d, Resposta %s, Data %02d/%02d/%d %02d:%02d:%02d\n",
                thread_id, id, json,
                t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
                t->tm_hour, t->tm_min, t->tm_sec);
        fclose(f);
    }
    pthread_mutex_unlock(&mutex_log);
}

// Função executada pelas threads
void *processa_ids(void *arg) {
    int thread_id = *(int *)arg;
    free(arg);

    while (1) {
        int id;
        pthread_mutex_lock(&mutex_ids);
        if (indice_atual >= NUM_IDS) {
            pthread_mutex_unlock(&mutex_ids);
            break;
        }
        id = lista_ids[indice_atual++];
        pthread_mutex_unlock(&mutex_ids);

        char json[200];
        consulta_api_simulada(id, json);
        grava_log(thread_id, id, json);
    }

    pthread_exit(NULL);
}

void processo_P1() {
    pthread_t threads[NUM_THREADS];

    pthread_mutex_init(&mutex_ids, NULL);
    pthread_mutex_init(&mutex_log, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        int *tid = malloc(sizeof(int));
        *tid = i + 1;
        pthread_create(&threads[i], NULL, processa_ids, tid);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex_ids);
    pthread_mutex_destroy(&mutex_log);
}

// Lê os IDs do arquivo lista_ids.txt
void ler_arquivo_ids(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro ao abrir lista_ids.txt");
        exit(1);
    }

    while (fscanf(f, "%d", &lista_ids[NUM_IDS]) == 1) {
        NUM_IDS++;
        if (NUM_IDS >= MAX_IDS) break;
    }

    fclose(f);
    printf("Foram carregados %d IDs do arquivo.\n", NUM_IDS);
}

int main() {
    ler_arquivo_ids("lista_ids.txt");

    pid_t pid = fork();
    if (pid < 0) {
        perror("Erro no fork");
        exit(1);
    } else if (pid == 0) {
        processo_P1();
        exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Processo P1 terminou com sucesso.\n");
        } else {
            printf("Processo P1 terminou com erro.\n");
        }
    }

    return 0;
}
