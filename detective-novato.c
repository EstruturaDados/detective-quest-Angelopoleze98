#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala na mansão.
// Cada sala tem um nome e ponteiros para os caminhos à esquerda e à direita.
struct Sala {
    char nome[50]; // Nome da sala (máximo de 50 caracteres).
    struct Sala *esquerda; // Ponteiro para a sala à esquerda.
    struct Sala *direita; // Ponteiro para a sala à direita.
};

// Função para criar uma sala dinamicamente.
// Aloca memória para a sala, define o nome e inicializa os ponteiros como NULL.
struct Sala* criarSala(const char* nome) {
    struct Sala* nova_sala = (struct Sala*) malloc(sizeof(struct Sala));
    if (nova_sala == NULL) {
        printf("Erro ao alocar memoria para a sala!\n");
        exit(1); // Encerra o programa em caso de falha na alocação.
    }
    strcpy(nova_sala->nome, nome); // Copia o nome para a sala.
    nova_sala->esquerda = NULL; // Inicializa caminho à esquerda como nulo.
    nova_sala->direita = NULL; // Inicializa caminho à direita como nulo.
    return nova_sala;
}

// Função para explorar a mansão de forma interativa.
// Permite ao jogador navegar pela árvore binária até chegar a uma sala sem caminhos (folha).
void explorarSalas(struct Sala* atual) {
    if (atual == NULL) {
        printf("Erro: Sala atual invalida!\n");
        return;
    }

    // Loop principal para exploração até alcançar uma sala sem caminhos.
    while (atual != NULL) {
        printf("\nVoce esta na sala: %s\n", atual->nome);
        
        // Verifica se é uma sala sem caminhos (folha).
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Esta sala nao tem mais caminhos. Exploracao encerrada.\n");
            return;
        }

        // Exibe opções disponíveis com base nos caminhos existentes.
        printf("Escolha uma direcao:\n");
        if (atual->esquerda != NULL) {
            printf("e - Ir para a esquerda\n");
        }
        if (atual->direita != NULL) {
            printf("d - Ir para a direita\n");
        }
        printf("s - Sair da exploracao\n");

        // Lê a escolha do jogador.
        char escolha;
        printf("Opcao: ");
        scanf(" %c", &escolha);

        // Processa a escolha do jogador.
        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda; // Move para a sala à esquerda.
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita; // Move para a sala à direita.
        } else if (escolha == 's') {
            printf("Exploracao encerrada pelo usuario.\n");
            return;
        } else {
            printf("Opcao invalida ou caminho nao disponivel. Tente novamente.\n");
        }
    }
}

// Função para liberar a memória alocada para a árvore.
// Usa pós-ordem (libera filhos antes do nó pai) para evitar vazamentos.
void liberarArvore(struct Sala* sala) {
    if (sala == NULL) {
        return;
    }
    liberarArvore(sala->esquerda); // Libera subárvore à esquerda.
    liberarArvore(sala->direita); // Libera subárvore à direita.
    free(sala); // Libera a sala atual.
}

// Função principal.
// Cria a árvore binária representando o mapa da mansão e inicia a exploração.
int main() {
    // Criação manual da árvore binária (mapa da mansão).
    struct Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Cozinha");
    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita = criarSala("Sala de Jantar");
    hall->direita->esquerda = criarSala("Jardim");
    hall->direita->direita = criarSala("Quarto de Hospedes");
    hall->esquerda->esquerda->esquerda = criarSala("Estudio");
    hall->esquerda->esquerda->direita = criarSala("Sala de Leitura");

    printf("Bem-vindo ao Detective Quest!\n");
    printf("Voce comeca no Hall de Entrada da mansao.\n");

    // Inicia a exploração da mansão a partir do Hall.
    explorarSalas(hall);

    // Libera a memória alocada para a árvore.
    liberarArvore(hall);
    printf("Memoria liberada. Programa encerrado.\n");

    return 0;
}