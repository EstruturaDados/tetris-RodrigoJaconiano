#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char tipo; 
    int id;
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

char gerarTipo() {
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    return tipos[rand() % 7];
}

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->qtd = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

bool filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

bool filaVazia(Fila *f) {
    return f->qtd == 0;
}

bool pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

bool pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void enqueue(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->itens[f->tras] = p;
        f->tras = (f->tras + 1) % TAM_FILA;
        f->qtd++;
    }
}

Peca dequeue(Fila *f) {
    Peca vazio = {'-', -1};
    if (filaVazia(f)) return vazio;
    Peca p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

void push(Pilha *p, Peca pec) {
    if (!pilhaCheia(p)) {
        p->topo++;
        p->itens[p->topo] = pec;
    }
}

Peca pop(Pilha *p) {
    Peca vazio = {'-', -1};
    if (pilhaVazia(p)) return vazio;
    Peca peca = p->itens[p->topo];
    p->topo--;
    return peca;
}

void mostrarFila(Fila *f) {
    printf("Fila: ");
    for (int i = 0; i < f->qtd; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c%d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\n");
}

void mostrarPilha(Pilha *p) {
    printf("Pilha: ");
    for (int i = 0; i <= p->topo; i++) {
        printf("[%c%d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

void trocarFrenteTopo(Fila *f, Pilha *p) {
    if (!filaVazia(f) && !pilhaVazia(p)) {
        int frenteIdx = f->frente;
        Peca temp = f->itens[frenteIdx];
        f->itens[frenteIdx] = p->itens[p->topo];
        p->itens[p->topo] = temp;
        printf("Troca realizada: frente da fila <-> topo da pilha\n");
    } else {
        printf("Não é possível trocar: fila ou pilha vazia\n");
    }
}

void trocarTres(Fila *f, Pilha *p) {
    if (f->qtd >= 3 && p->topo == 2) {
        for (int i = 0; i < 3; i++) {
            int filaIdx = (f->frente + i) % TAM_FILA;
            Peca temp = f->itens[filaIdx];
            f->itens[filaIdx] = p->itens[i];
            p->itens[i] = temp;
        }
        printf("Troca realizada: 3 primeiros da fila <-> 3 da pilha\n");
    } else {
        printf("Não é possível trocar: fila ou pilha não possuem 3 peças\n");
    }
}

Peca gerarPeca(int id) {
    Peca p;
    p.tipo = gerarTipo();
    p.id = id;
    return p;
}

void preencherFilaInicial(Fila *f, int *idCounter) {
    while (!filaCheia(f)) {
        enqueue(f, gerarPeca((*idCounter)++));
    }
}


void mostrarMenu() {
    printf("\n=== Tetris Stack - Nível Mestre ===\n");
    printf("1 - Jogar peça (remover da frente da fila)\n");
    printf("2 - Reservar peça (enviar da fila para pilha)\n");
    printf("3 - Usar peça da reserva (pop da pilha)\n");
    printf("4 - Trocar peça da frente da fila com topo da pilha\n");
    printf("5 - Trocar 3 primeiros da fila com 3 da pilha\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    int idCounter = 1;
    int opcao;
    Peca temp;

    inicializarFila(&fila);
    inicializarPilha(&pilha);
    preencherFilaInicial(&fila, &idCounter);

    do {
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        mostrarMenu();
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                temp = dequeue(&fila);
                printf("Peça jogada: [%c%d]\n", temp.tipo, temp.id);
                enqueue(&fila, gerarPeca(idCounter++));
                break;
            case 2: 
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    temp = dequeue(&fila);
                    push(&pilha, temp);
                    printf("Peça [%c%d] enviada para a pilha\n", temp.tipo, temp.id);
                    enqueue(&fila, gerarPeca(idCounter++));
                } else {
                    printf("Não é possível reservar: fila vazia ou pilha cheia\n");
                }
                break;
            case 3:
                if (!pilhaVazia(&pilha)) {
                    temp = pop(&pilha);
                    printf("Peça usada da pilha: [%c%d]\n", temp.tipo, temp.id);
                } else {
                    printf("Pilha vazia\n");
                }
                break;
            case 4: 
                trocarFrenteTopo(&fila, &pilha);
                break;
            case 5: 
                trocarTres(&fila, &pilha);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida\n");
        }
    } while(opcao != 0);

    return 0;
}
