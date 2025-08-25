#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes Globais
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_MISSAO 100

// Definição da struct Territorio
typedef struct
{
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// strdup local (para compatibilidade)
char *strdup_local(const char *s)
{
    if (s == NULL)
        return NULL;
    size_t len = strlen(s) + 1;
    char *p = (char *)malloc(len);
    if (p != NULL)
        memcpy(p, s, len);
    return p;
}

// Função para limpar o buffer de entrada
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Exibe todos os territórios do mapa
void exibirTerritorios(Territorio *mapa, int n)
{
    printf("\n=========================================\n");
    printf("=============> TERRITORIOS <=============\n");
    printf("=========================================\n");
    for (int i = 0; i < n; i++)
    {
        printf("%2d.%s (Exercito: %s | Tropas: %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=========================================\n");
}

// Função de cadastro territoriais
void cadastrarTerritorios(Territorio *mapa, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Cadastro do %dº território:\n", i + 1);

        printf("Nome do território: ");
        if (fgets(mapa[i].nome, MAX_NOME, stdin) == NULL)
        {
            mapa[i].nome[0] = '\0';
        }
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        if (fgets(mapa[i].cor, MAX_COR, stdin) == NULL)
        {
            mapa[i].cor[0] = '\0';
        }
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas <= 0)
        {
            printf("Entrada inválida. Digite um número inteiro maior que 0: ");
            limparBufferEntrada();
        }
        limparBufferEntrada();

        printf("\n");
    }
}

// Missões possíveis
const char *missoesPossiveis[] = {
    "Possuir pelo menos 3 territorios.",
    "Ter pelo menos 15 tropas no total.",
    "Possuir 3 territorios adjacentes no mapa.",
    "Possuir o maior numero de territorios entre os jogadores.",
    "Ter um territorio com pelo menos 10 tropas."};
int totalMissoesDisponiveis = sizeof(missoesPossiveis) / sizeof(missoesPossiveis[0]);

// Atribui uma missão
void atribuirMissao(char *destino, const char *missoes[], int totalMissoes)
{
    if (destino == NULL || missoes == NULL || totalMissoes <= 0)
        return;
    int idx = rand() % totalMissoes;
    strncpy(destino, missoes[idx], MAX_MISSAO - 1);
    destino[MAX_MISSAO - 1] = '\0';
}

// Exibe missão
void exibirMissao(const char *missao, const char *corJogador)
{
    printf("\n--- MISSÃO DO JOGADOR (Exercito %s) ---\n", corJogador);
    printf("%s\n", missao);
    printf("----------------------------------------\n\n");
}

// FUNÇÕES AUXILIARES
int contarTerritoriosPorCor(Territorio *mapa, int tamanho, const char *cor)
{
    int cont = 0;
    for (int i = 0; i < tamanho; i++)
        if (strcmp(mapa[i].cor, cor) == 0)
            cont++;
    return cont;
}

int somarTropasPorCor(Territorio *mapa, int tamanho, const char *cor)
{
    int soma = 0;
    for (int i = 0; i < tamanho; i++)
        if (strcmp(mapa[i].cor, cor) == 0)
            soma += mapa[i].tropas;
    return soma;
}

int temTresAdjacentes(Territorio *mapa, int tamanho, const char *cor)
{
    if (tamanho < 3)
        return 0;
    for (int i = 0; i <= tamanho - 3; i++)
    {
        if (strcmp(mapa[i].cor, cor) == 0 &&
            strcmp(mapa[i + 1].cor, cor) == 0 &&
            strcmp(mapa[i + 2].cor, cor) == 0)
            return 1;
    }
    return 0;
}

int temTerritorioComTropasMin(Territorio *mapa, int tamanho, const char *cor, int minimo)
{
    for (int i = 0; i < tamanho; i++)
    {
        if (strcmp(mapa[i].cor, cor) == 0 && mapa[i].tropas >= minimo)
            return 1;
    }
    return 0;
}

int ehMaiorNumeroTerritorios(Territorio *mapa, int tamanho, const char *corJogador, char **coresJogadores, int numJogadores)
{
    int contJogador = contarTerritoriosPorCor(mapa, tamanho, corJogador);
    for (int i = 0; i < numJogadores; i++)
    {
        if (strcmp(coresJogadores[i], corJogador) == 0)
            continue;
        int cont = contarTerritoriosPorCor(mapa, tamanho, coresJogadores[i]);
        if (contJogador <= cont)
            return 0;
    }
    return 1;
}

// Verificando missão
int verificarMissao(const char *missao, Territorio *mapa, int tamanho, const char *corJogador, char **coresJogadores, int numJogadores)
{
    if (missao == NULL || mapa == NULL || corJogador == NULL)
        return 0;

    if (strcmp(missao, "Possuir pelo menos 3 territorios.") == 0)
        return contarTerritoriosPorCor(mapa, tamanho, corJogador) >= 3;
    else if (strcmp(missao, "Ter pelo menos 15 tropas no total.") == 0)
        return somarTropasPorCor(mapa, tamanho, corJogador) >= 15;
    else if (strcmp(missao, "Possuir 3 territorios adjacentes no mapa.") == 0)
        return temTresAdjacentes(mapa, tamanho, corJogador);
    else if (strcmp(missao, "Possuir o maior numero de territorios entre os jogadores.") == 0)
        return ehMaiorNumeroTerritorios(mapa, tamanho, corJogador, coresJogadores, numJogadores);
    else if (strcmp(missao, "Ter um territorio com pelo menos 10 tropas.") == 0)
        return temTerritorioComTropasMin(mapa, tamanho, corJogador, 10);

    return 0; // desconhecida
}

// Ataque
void atacar(Territorio *atacante, Territorio *defensor)
{
    if (atacante == NULL || defensor == NULL)
        return;

    if (strcmp(atacante->cor, defensor->cor) == 0)
    {
        printf("Ataque inválido: não é permitido atacar território da mesma cor.\n");
        return;
    }

    if (atacante->tropas <= 1)
    {
        printf("Ataque inválido: atacante precisa ter mais de 1 tropa para atacar.\n");
        return;
    }

    // Jogando os dados
    int dadoAtq = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("\n==========> RESULTADO DA JOGADA <=========\n");
    printf("%s (Atacante) jogou o dado e tirou: %d\n", atacante->nome, dadoAtq);
    printf("%s (Defensor) jogou o dado e tirou: %d\n", defensor->nome, dadoDef);

    // Vitória do atacante
    if (dadoAtq > dadoDef)
    {
        // Atacante vence: transfere 1 tropa do defensor para o atacante
        printf("\n>>>> VITÓRIA DO ATACANTE! %s vence o confronto contra %s. <<<<\n",
               atacante->nome, defensor->nome);

        // Realiza a transferência de tropas
        defensor->tropas -= 1;
        atacante->tropas += 1;

        // Se defensor chegou a 0, ele é dominado: ganha 1 tropa e troca de cor
        if (defensor->tropas <= 0)
        {
            defensor->tropas = 1; // segurança: não deixar 0
            strncpy(defensor->cor, atacante->cor, MAX_COR - 1);
            defensor->cor[MAX_COR - 1] = '\0';
            printf("%s foi dominado! Agora pertence ao exercito %s!\nReceberá 1 tropa para defender o territorio consquistado.\n",
                   defensor->nome, defensor->cor);
        }
    }
    // Vitória do defensor
    else
    {
        // Defensor vence ou empata: atacante perde 1 tropa
        printf("\n>>>> DEFENSOR RESISTIU! %s defendeu com sucesso. <<<<\n", defensor->nome);

        // Realiza a transferência de tropas
        atacante->tropas -= 1;
        defensor->tropas += 1;

        // Se atacante chegou a 0, ele é dominado: ganha 1 tropa e troca de cor
        if (atacante->tropas <= 0)
        {
            atacante->tropas = 1; // segurança: não deixar 0
            strncpy(atacante->cor, defensor->cor, MAX_COR - 1);
            atacante->cor[MAX_COR - 1] = '\0';
            printf("%s foi dominado! Agora pertence ao exercito %s!\nReceberá 1 tropa para defender o territorio consquistado.\n",
                   atacante->nome, atacante->cor);
        }
    }

    // Exibe o estado atualizado dos territórios envolvidos
    printf("\n===========================================\n");
    printf("============= MAPA DA BATALHA =============\n");
    printf("===========================================\n");
    printf("%s (Exercito %s | Tropas = %d)\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("%s (Exercito %s | Tropas = %d)\n", defensor->nome, defensor->cor, defensor->tropas);
    printf("===========================================\n");
}

// Liberar memoria
void liberarMemoria(Territorio *mapa, char *missaoJogador, char **coresJogadores, int numJogadores)
{
    if (mapa != NULL)
        free(mapa);
    if (missaoJogador != NULL)
        free(missaoJogador);
    if (coresJogadores != NULL)
    {
        for (int i = 0; i < numJogadores; i++)
            if (coresJogadores[i] != NULL)
                free(coresJogadores[i]);
        free(coresJogadores);
    }
}

int main()
{
    srand((unsigned int)time(NULL));

    int totalTerritorios;
    printf("Quantos territórios deseja cadastrar? ");
    while (scanf("%d", &totalTerritorios) != 1 || totalTerritorios <= 0)
    {
        printf("Entrada inválida. Digite um número inteiro positivo: ");
        limparBufferEntrada();
    }
    limparBufferEntrada();

    // Alocação do mapa
    Territorio *mapa = (Territorio *)calloc(totalTerritorios, sizeof(Territorio));
    if (mapa == NULL)
    {
        fprintf(stderr, "Erro: falha na alocação de memória.\n");
        return 1;
    }

    // Cadastro dos territórios
    printf("\n=== Cadastro de Territórios do Jogo ===\n\n");
    cadastrarTerritorios(mapa, totalTerritorios);

    // Exibe mapa inicial
    exibirTerritorios(mapa, totalTerritorios);

    // Detecta cores/jogadores existentes
    char **coresJogadores = NULL;
    int numJogadores = 0;

    for (int i = 0; i < totalTerritorios; i++)
    {
        const char *corAtual = mapa[i].cor;
        int achou = 0;
        for (int j = 0; j < numJogadores; j++)
        {
            if (strcmp(corAtual, coresJogadores[j]) == 0)
            {
                achou = 1;
                break;
            }
        }
        if (!achou)
        {
            coresJogadores = (char **)realloc(coresJogadores, (numJogadores + 1) * sizeof(char *));
            coresJogadores[numJogadores] = strdup_local(corAtual);
            numJogadores++;
        }
    }

    if (numJogadores == 0)
    {
        printf("Nenhum jogador detectado (sem cores). Encerrando.\n");
        free(mapa);
        return 0;
    }

    // Usuário escolhe qual cor é o seu exército
    printf("\nCores detectadas no mapa (cada cor = um exército):\n");
    for (int i = 0; i < numJogadores; i++)
    {
        printf("%d. %s\n", i + 1, coresJogadores[i]);
    }
    int escolhaCor = 0;
    while (1)
    {
        printf("Escolha o número correspondente à cor do SEU exército: ");
        if (scanf("%d", &escolhaCor) != 1)
        {
            printf("Entrada inválida.\n");
            limparBufferEntrada();
            continue;
        }
        limparBufferEntrada();
        if (escolhaCor >= 1 && escolhaCor <= numJogadores)
            break;
        printf("Escolha inválida. Tente novamente.\n");
    }
    const char *corDoJogador = coresJogadores[escolhaCor - 1];

    // Atribui uma missão ao jogador
    char *missaoJogador = (char *)malloc(MAX_MISSAO);
    if (missaoJogador == NULL)
    {
        fprintf(stderr, "Erro: falha na alocação da missão.\n");
        liberarMemoria(mapa, NULL, coresJogadores, numJogadores);
        return 1;
    }
    atribuirMissao(missaoJogador, missoesPossiveis, totalMissoesDisponiveis);

    // Exibe a missão do jogador imediatamente (apenas uma vez)
    exibirMissao(missaoJogador, corDoJogador);

    // --- Loop principal com menu de ações ---
    while (1)
    {
        printf("\n=== MENU DE AÇÕES ===\n");
        printf("1. Atacar\n");
        printf("2. Verificar Missão (exibir missão e status)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");

        int opc;
        if (scanf("%d", &opc) != 1)
        {
            printf("Entrada inválida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }
        limparBufferEntrada();

        if (opc == 0)
        {
            printf("Encerrando o jogo...\n");
            break;
        }
        else if (opc == 2)
        {
            // Exibe missão e se foi cumprida
            exibirMissao(missaoJogador, corDoJogador);
            int cumprida = verificarMissao(missaoJogador, mapa, totalTerritorios, corDoJogador, coresJogadores, numJogadores);
            if (cumprida)
                printf("Status: MISSÃO CUMPRIDA! Parabéns, Exército %s.\n", corDoJogador);
            else
                printf("Status: MISSÃO NÃO CUMPRIDA ainda. Continue jogando.\n");
            continue;
        }
        else if (opc == 1)
        {
            exibirTerritorios(mapa, totalTerritorios);

            // Fluxo de ataque
            int idxAtq;
            printf("Escolha o território atacante (1 a %d): ", totalTerritorios);
            if (scanf("%d", &idxAtq) != 1)
            {
                printf("Entrada inválida. Tente novamente.\n");
                limparBufferEntrada();
                continue;
            }
            limparBufferEntrada();

            if (idxAtq < 1 || idxAtq > totalTerritorios)
            {
                printf("Índice inválido. Voltando ao menu.\n");
                continue;
            }

            int idxDef;
            printf("Escolha o território defensor (1 a %d): ", totalTerritorios);
            if (scanf("%d", &idxDef) != 1)
            {
                printf("Entrada inválida. Tente novamente.\n");
                limparBufferEntrada();
                continue;
            }
            limparBufferEntrada();

            if (idxDef < 1 || idxDef > totalTerritorios)
            {
                printf("Índice inválido. Voltando ao menu.\n");
                continue;
            }

            Territorio *atacante = &mapa[idxAtq - 1];
            Territorio *defensor = &mapa[idxDef - 1];

            if (atacante == defensor)
            {
                printf("Você selecionou o mesmo território. Operação cancelada.\n");
                continue;
            }

            if (strcmp(atacante->cor, defensor->cor) == 0)
            {
                printf("Ataque inválido: territórios da mesma cor.\n");
                continue;
            }

            // Faz o ataque
            atacar(atacante, defensor);

            // Após ataque, verifica se missão foi cumprida
            int cumprida = verificarMissao(missaoJogador, mapa, totalTerritorios, corDoJogador, coresJogadores, numJogadores);
            if (cumprida)
            {
                printf("\n=========================================\n");
                printf(">>> MISSÃO CUMPRIDA! VENCEDOR: %s <<<\n", corDoJogador);
                printf("Missão: %s\n", missaoJogador);
                printf("=========================================\n\n");
                break; // encerra o jogo/vitória
            }

            // Continua o jogo
            printf("\nPressione Enter para continuar...");
            limparBufferEntrada();
            exibirTerritorios(mapa, totalTerritorios);
        }
        else
        {
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    // Libera memória e finaliza
    liberarMemoria(mapa, missaoJogador, coresJogadores, numJogadores);
    printf("Memória liberada. Programa encerrado.\n");
    return 0;
}