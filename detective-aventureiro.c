#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala na mansão.
// Cada sala tem um nome, uma pista (opcional) e ponteiros para caminhos à esquerda e à direita.
struct Sala {
    char nome[50];          // Nome da sala (ex.: "Hall de Entrada").
    char pista[100];        // Pista associada à sala (pode ser vazia).
    struct Sala *esquerda;  // Ponteiro para a sala à esquerda.
    struct Sala *direita;   // Ponteiro para a sala à direita.
};

// Estrutura para a árvore binária de busca (BST) que armazena as pistas coletadas.
// Cada nó contém uma pista e ponteiros para filhos à esquerda e à direita.
struct PistaNode {
    char pista[100];          // Conteúdo da pista.
    struct PistaNode *esquerda; // Filho à esquerda na BST.
    struct PistaNode *direita;  // Filho à direita na BST.
};

// Função para criar uma sala dinamicamente.
// Aloca memória, define nome e pista, e inicializa ponteiros como NULL.
struct Sala* criarSala(const char* nome, const char* pista) {
    struct Sala* nova_sala = (struct Sala*)malloc(sizeof(struct Sala));
    if (nova_sala == NULL) {
        printf("Erro ao alocar memoria para a sala!\n");
        exit(1);
    }
    strcpy(nova_sala->nome, nome); // Copia o nome da sala.
    strcpy(nova_sala->pista, pista); // Copia a pista (pode ser vazia).
    nova_sala->esquerda = NULL;
    nova_sala->direita = NULL;
    return nova_sala;
}

// Função para criar um nó da árvore BST de pistas.
// Aloca memória e inicializa a pista e os ponteiros.
struct PistaNode* criarPistaNode(const char* pista) {
    struct PistaNode* novo_no = (struct PistaNode*)malloc(sizeof(struct PistaNode));
    if (novo_no == NULL) {
        printf("Erro ao alocar memoria para o no de pista!\n");
        exit(1);
    }
    strcpy(novo_no->pista, pista);
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;
    return novo_no;
}

// Função para inserir uma pista na árvore BST.
// Insere de forma ordenada (alfabeticamente) com base no conteúdo da pista.
struct PistaNode* inserirPista(struct PistaNode* raiz, const char* pista) {
    // Se a pista está vazia, não insere.
    if (strlen(pista) == 0) {
        return raiz;
    }
    // Se a árvore está vazia, cria um novo nó.
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    // Compara a pista com a raiz para decidir onde inserir.
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista); // Insere à esquerda.
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, pista); // Insere à direita.
    }
    // Se cmp == 0, não insere duplicatas.
    return raiz;
}

// Função para exibir as pistas em ordem alfabética (percurso em-ordem).
void exibirPistas(struct PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    exibirPistas(raiz->esquerda); // Visita subárvore esquerda.
    printf("- %s\n", raiz->pista); // Exibe pista atual.
    exibirPistas(raiz->direita); // Visita subárvore direita.
}

// Função para liberar a memória da árvore BST de pistas.
// Usa pós-ordem para liberar os filhos antes do nó pai.
void liberarPistas(struct PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// Função para liberar a memória da árvore binária da mansão.
void liberarSalas(struct Sala* sala) {
    if (sala == NULL) {
        return;
    }
    liberarSalas(sala->esquerda);
    liberarSalas(sala->direita);
    free(sala);
}

// Função para explorar a mansão interativamente.
// Coleta pistas, insere na BST e permite navegação até o usuário sair.
void explorarSalasComPistas(struct Sala* atual, struct PistaNode** pistas_raiz) {
    if (atual == NULL) {
        printf("Erro: Sala atual invalida!\n");
        return;
    }

    // Loop principal para exploração.
    while (atual != NULL) {
        printf("\nVoce esta na sala: %s\n", atual->nome);
        
        // Verifica se há uma pista na sala e a adiciona à BST.
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *pistas_raiz = inserirPista(*pistas_raiz, atual->pista);
        } else {
            printf("Nenhuma pista encontrada nesta sala.\n");
        }

        // Exibe opções de navegação.
        printf("Escolha uma direcao:\n");
        if (atual->esquerda != NULL) {
            printf("e - Ir para a esquerda\n");
        }
        if (atual->direita != NULL) {
            printf("d - Ir para a direita\n");
        }
        printf("s - Sair da exploracao\n");

        // Lê a escolha do usuário.
        char escolha;
        printf("Opcao: ");
        scanf(" %c", &escolha);

        // Processa a escolha.
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

// Função principal.
// Cria o mapa da mansão, inicia a exploração e exibe as pistas coletadas.
int main() {
    // Criação manual da árvore binária da mansão com pistas.
    struct Sala* hall = criarSala("Hall de Entrada", "Pegadas na entrada");
    hall->esquerda = criarSala("Sala de Estar", "Vela derretida");
    hall->direita = criarSala("Cozinha", "");
    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro aberto");
    hall->esquerda->direita = criarSala("Sala de Jantar", "Copo quebrado");
    hall->direita->esquerda = criarSala("Jardim", "Flor amassada");
    hall->direita->direita = criarSala("Quarto de Hospedes", "");
    hall->esquerda->esquerda->esquerda = criarSala("Estudio", "Caneta no chao");
    hall->esquerda->esquerda->direita = criarSala("Sala de Leitura", "");

    // Inicializa a árvore BST de pistas como vazia.
    struct PistaNode* pistas_raiz = NULL;

    printf("Bem-vindo ao Detective Quest!\n");
    printf("Voce comeca no Hall de Entrada da mansao.\n");

    // Inicia a exploração, coletando pistas.
    explorarSalasComPistas(hall, &pistas_raiz);

    // Exibe as pistas coletadas em ordem alfabética.
    printf("\nPistas coletadas (em ordem alfabetica):\n");
    if (pistas_raiz == NULL) {
        printf("Nenhuma pista coletada.\n");
    } else {
        exibirPistas(pistas_raiz);
    }

    // Libera a memória alocada.
    liberarSalas(hall);
    liberarPistas(pistas_raiz);
    printf("\nMemoria liberada. Programa encerrado.\n");

    return 0;
}