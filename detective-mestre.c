#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala na mansão.
// Contém nome, pista (estática, pode ser vazia) e ponteiros para caminhos à esquerda e à direita.
struct Sala {
    char nome[50];          // Nome da sala (ex.: "Hall de Entrada").
    char pista[100];        // Pista associada à sala (definida estaticamente).
    struct Sala *esquerda;  // Ponteiro para a sala à esquerda.
    struct Sala *direita;   // Ponteiro para a sala à direita.
};

// Estrutura para a árvore binária de busca (BST) que armazena pistas coletadas.
// Cada nó contém uma pista e ponteiros para filhos à esquerda e à direita.
struct PistaNode {
    char pista[100];          // Conteúdo da pista.
    struct PistaNode *esquerda; // Filho à esquerda na BST.
    struct PistaNode *direita;  // Filho à direita na BST.
};

// Estrutura para a tabela hash que associa pistas a suspeitos.
// Usa chaining (lista ligada) para tratar colisões.
struct HashNode {
    char pista[100];         // Chave: conteúdo da pista.
    char suspeito[50];       // Valor: nome do suspeito associado.
    struct HashNode *next;   // Ponteiro para o próximo nó na cadeia.
};

// Tamanho fixo da tabela hash (número de buckets).
#define HASH_SIZE 10

// Função para criar uma sala dinamicamente.
// Aloca memória, define nome e pista, e inicializa ponteiros como NULL.
// Parâmetros: nome da sala e pista estática (pode ser vazia).
struct Sala* criarSala(const char* nome, const char* pista) {
    struct Sala* nova_sala = (struct Sala*)malloc(sizeof(struct Sala));
    if (nova_sala == NULL) {
        printf("Erro ao alocar memoria para a sala!\n");
        exit(1);
    }
    strncpy(nova_sala->nome, nome, sizeof(nova_sala->nome) - 1); // Copia nome com segurança.
    nova_sala->nome[sizeof(nova_sala->nome) - 1] = '\0'; // Garante terminação.
    strncpy(nova_sala->pista, pista, sizeof(nova_sala->pista) - 1); // Copia pista.
    nova_sala->pista[sizeof(nova_sala->pista) - 1] = '\0';
    nova_sala->esquerda = NULL;
    nova_sala->direita = NULL;
    return nova_sala;
}

// Função para criar um nó da BST de pistas.
// Aloca memória, inicializa a pista e ponteiros como NULL.
struct PistaNode* criarPistaNode(const char* pista) {
    struct PistaNode* novo_no = (struct PistaNode*)malloc(sizeof(struct PistaNode));
    if (novo_no == NULL) {
        printf("Erro ao alocar memoria para o no de pista!\n");
        exit(1);
    }
    strncpy(novo_no->pista, pista, sizeof(novo_no->pista) - 1); // Copia com segurança.
    novo_no->pista[sizeof(novo_no->pista) - 1] = '\0';
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;
    return novo_no;
}

// Função para inserir uma pista na BST.
// Insere de forma ordenada (alfabeticamente) e ignora pistas vazias ou duplicadas.
struct PistaNode* inserirPista(struct PistaNode* raiz, const char* pista) {
    if (strlen(pista) == 0) {
        return raiz; // Ignora pistas vazias.
    }
    if (raiz == NULL) {
        return criarPistaNode(pista); // Novo nó se árvore vazia.
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista); // Insere à esquerda.
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, pista); // Insere à direita.
    }
    // Ignora duplicatas (cmp == 0).
    return raiz;
}

// Função para exibir as pistas em ordem alfabética (percurso em-ordem).
// Exibe cada pista coletada em uma linha.
void exibirPistas(struct PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Função para liberar a memória da BST de pistas.
// Usa pós-ordem para liberar filhos antes do nó pai.
void liberarPistas(struct PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// Função para liberar a memória da árvore binária da mansão.
// Usa pós-ordem para garantir que todos os nós sejam liberados.
void liberarSalas(struct Sala* sala) {
    if (sala == NULL) {
        return;
    }
    liberarSalas(sala->esquerda);
    liberarSalas(sala->direita);
    free(sala);
}

// --- Funções da Tabela Hash ---

// Função hash para strings.
// Usa multiplicador 31 para distribuição uniforme e retorna índice no intervalo [0, HASH_SIZE).
unsigned int hash(const char* key) {
    unsigned int hash_value = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash_value = hash_value * 31 + key[i];
    }
    return hash_value % HASH_SIZE;
}

// Função para inserir uma associação pista-suspeito na tabela hash.
// Usa chaining para tratar colisões, adicionando no início da lista do bucket.
void inserirNaHash(struct HashNode** hash_table, const char* pista, const char* suspeito) {
    unsigned int index = hash(pista);
    struct HashNode* novo_no = (struct HashNode*)malloc(sizeof(struct HashNode));
    if (novo_no == NULL) {
        printf("Erro ao alocar memoria para o no da hash!\n");
        exit(1);
    }
    strncpy(novo_no->pista, pista, sizeof(novo_no->pista) - 1);
    novo_no->pista[sizeof(novo_no->pista) - 1] = '\0';
    strncpy(novo_no->suspeito, suspeito, sizeof(novo_no->suspeito) - 1);
    novo_no->suspeito[sizeof(novo_no->suspeito) - 1] = '\0';
    novo_no->next = hash_table[index]; // Adiciona ao início da cadeia.
    hash_table[index] = novo_no;
}

// Função para encontrar o suspeito associado a uma pista.
// Percorre a cadeia no bucket correspondente e retorna o suspeito ou NULL.
char* encontrarSuspeito(struct HashNode** hash_table, const char* pista) {
    unsigned int index = hash(pista);
    struct HashNode* current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->pista, pista) == 0) {
            return current->suspeito;
        }
        current = current->next;
    }
    return NULL;
}

// Função para liberar a memória da tabela hash.
// Libera cada lista ligada em todos os buckets.
void liberarHash(struct HashNode** hash_table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        struct HashNode* current = hash_table[i];
        while (current != NULL) {
            struct HashNode* temp = current;
            current = current->next;
            free(temp);
        }
        hash_table[i] = NULL;
    }
}

// Função para contar pistas associadas a um suspeito.
// Percorre a BST recursivamente, consulta a hash para cada pista e conta matches.
int contarPistasParaSuspeito(struct PistaNode* raiz, struct HashNode** hash_table, const char* suspeito_acusado) {
    if (raiz == NULL) {
        return 0;
    }
    int count = contarPistasParaSuspeito(raiz->esquerda, hash_table, suspeito_acusado);
    char* suspeito = encontrarSuspeito(hash_table, raiz->pista);
    if (suspeito != NULL && strcmp(suspeito, suspeito_acusado) == 0) {
        count++;
    }
    count += contarPistasParaSuspeito(raiz->direita, hash_table, suspeito_acusado);
    return count;
}

// Função para explorar a mansão interativamente.
// Navega pela árvore binária, coleta pistas na BST e permite sair com 's'.
// Exibe mensagens claras sobre a sala e a pista encontrada.
void explorarSalas(struct Sala* atual, struct PistaNode** pistas_raiz) {
    if (atual == NULL) {
        printf("Erro: Sala atual invalida!\n");
        return;
    }

    while (atual != NULL) {
        printf("\nVoce esta na sala: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *pistas_raiz = inserirPista(*pistas_raiz, atual->pista);
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        // Exibe opções de navegação com base nos caminhos disponíveis.
        printf("Escolha uma direcao:\n");
        if (atual->esquerda != NULL) {
            printf("e - Ir para a esquerda\n");
        }
        if (atual->direita != NULL) {
            printf("d - Ir para a direita\n");
        }
        printf("s - Sair da exploracao\n");

        char escolha;
        printf("Opcao: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Exploracao encerrada.\n");
            return;
        } else {
            printf("Opcao invalida ou caminho nao disponivel. Tente novamente.\n");
        }
    }
}

// Função para o julgamento final.
// Exibe pistas coletadas, solicita a acusação e verifica se há pelo menos 2 pistas contra o suspeito.
void verificarSuspeitoFinal(struct PistaNode* pistas_raiz, struct HashNode** hash_table) {
    printf("\nPistas coletadas (em ordem alfabetica):\n");
    if (pistas_raiz == NULL) {
        printf("Nenhuma pista coletada.\n");
    } else {
        exibirPistas(pistas_raiz);
    }

    // Solicita o suspeito acusado.
    char suspeito_acusado[50];
    printf("\nQuem voce acusa de ser o culpado? (ex: Mordomo, Cozinheiro, Jardineiro): ");
    scanf(" %49[^\n]", suspeito_acusado); // Lê string com espaços.

    // Conta pistas associadas ao suspeito.
    int count = contarPistasParaSuspeito(pistas_raiz, hash_table, suspeito_acusado);

    // Verifica se há evidências suficientes (>= 2 pistas).
    if (count >= 2) {
        printf("Parabens! Voce tem %d pistas apontando para %s. Culpado encontrado!\n", count, suspeito_acusado);
    } else {
        printf("Evidencias insuficientes (%d pistas) contra %s. O misterio continua...\n", count, suspeito_acusado);
    }
}

// Função principal.
// Cria o mapa da mansão, inicializa a tabela hash e a BST, gerencia exploração e julgamento.
int main() {
    // Criação manual da árvore binária da mansão com pistas estáticas.
    struct Sala* hall = criarSala("Hall de Entrada", "Pegadas na entrada");
    hall->esquerda = criarSala("Sala de Estar", "Vela derretida");
    hall->direita = criarSala("Cozinha", "Faca suja");
    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro aberto");
    hall->esquerda->direita = criarSala("Sala de Jantar", "Copo quebrado");
    hall->direita->esquerda = criarSala("Jardim", "Flor amassada");
    hall->direita->direita = criarSala("Quarto de Hospedes", "");
    hall->esquerda->esquerda->esquerda = criarSala("Estudio", "Caneta no chao");
    hall->esquerda->esquerda->direita = criarSala("Sala de Leitura", "");

    // Inicializa a tabela hash vazia.
    struct HashNode* hash_table[HASH_SIZE] = {NULL};

    // Popula a tabela hash com associações fixas pista -> suspeito.
    inserirNaHash(hash_table, "Pegadas na entrada", "Mordomo");
    inserirNaHash(hash_table, "Vela derretida", "Mordomo");
    inserirNaHash(hash_table, "Faca suja", "Cozinheiro");
    inserirNaHash(hash_table, "Livro aberto", "Jardineiro");
    inserirNaHash(hash_table, "Copo quebrado", "Mordomo");
    inserirNaHash(hash_table, "Flor amassada", "Jardineiro");
    inserirNaHash(hash_table, "Caneta no chao", "Mordomo");
    inserirNaHash(hash_table, "Pagina rasgada", "Jardineiro");

    // Inicializa a BST de pistas.
    struct PistaNode* pistas_raiz = NULL;

    printf("Bem-vindo ao Detective Quest - Nivel Mestre!\n");
    printf("Explore a mansao, colete pistas e acuse o culpado.\n");
    printf("Voce comeca no Hall de Entrada.\n");

    // Inicia a exploração, coletando pistas.
    explorarSalas(hall, &pistas_raiz);

    // Inicia o julgamento final.
    verificarSuspeitoFinal(pistas_raiz, hash_table);

    // Libera memória alocada.
    liberarSalas(hall);
    liberarPistas(pistas_raiz);
    liberarHash(hash_table);
    printf("\nMemoria liberada. Programa encerrado.\n");

    return 0;
}