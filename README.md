# TPE1-SO-2025-2
## Trabalho Programação Concorrente em C, Sistemas Operacionais
Este projeto implementa um sistema concorrente em C que utiliza processos e threads para processar 1 milhão de identificadores (IDs).
### Durante a execução:
* O processo P0 cria P1.
* O processo P1 inicia 5 threads.
* Cada thread processa IDs de forma independente e grava as respostas no arquivo logs.txt.
### Como executar: 
```
gcc main.c -o main -lpthread
./main
```
### Tempo para a execução:
#### 50 Mil IDs:
<img width="1647" height="457" alt="image" src="https://github.com/user-attachments/assets/1bd5ae7d-8272-4947-9f38-24e924cf3b28" />

#### 1 Milhão de IDs:
<img width="1647" height="457" alt="image" src="https://github.com/user-attachments/assets/74777b40-d9c1-41ff-9ce7-2ed4206145bf" />

#### 10 Milhlões de IDs:
<img width="1647" height="457" alt="image" src="https://github.com/user-attachments/assets/bd71f3a0-0720-42d5-aeab-a03880ea0cc6" />

### Exemplo de Saída:

```
Thread 0, ID 1, Resposta {"id":1,"status":"ok","valor":113.55}, Data 2025-10-09 14:24:38
Thread 2, ID 2, Resposta {"id":2,"status":"ok","valor":203.27}, Data 2025-10-09 14:24:38
Thread 0, ID 3, Resposta {"id":3,"status":"ok","valor":108.76}, Data 2025-10-09 14:24:38
Thread 2, ID 5, Resposta {"id":5,"status":"ok","valor":714.99}, Data 2025-10-09 14:24:38
Thread 2, ID 8, Resposta {"id":8,"status":"ok","valor":264.64}, Data 2025-10-09 14:24:38
Thread 3, ID 7, Resposta {"id":7,"status":"ok","valor":641.64}, Data 2025-10-09 14:24:38
Thread 0, ID 6, Resposta {"id":6,"status":"ok","valor":121.84}, Data 2025-10-09 14:24:38
Thread 2, ID 9, Resposta {"id":9,"status":"ok","valor":792.23}, Data 2025-10-09 14:24:38
Thread 2, ID 12, Resposta {"id":12,"status":"ok","valor":27.01}, Data 2025-10-09 14:24:38
```
