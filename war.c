#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes Globais
#define MAX_NOME 30
#define MAX_COR 10

// Definição da struct Territorio
typedef struct
{
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

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
        // Função para limpar o buffer de entrada
        limparBufferEntrada();

        printf("\n");
    }
}

// Função para inicar ataque
void atacar(Territorio *atacante, Territorio *defensor)
{
    if (atacante == NULL || defensor == NULL)
        return;

    if (strcmp(atacante->cor, defensor->cor) == 0)
    {
        printf("Ataque inválido: não é permitido atacar território da mesma cor.\n");
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

int main()
{
    int totalTerritorios;
    printf("Quantos territórios deseja cadastrar? ");
    while (scanf("%d", &totalTerritorios) != 1 || totalTerritorios <= 0)
    {
        printf("Entrada inválida. Digite um número inteiro positivo: ");
        limparBufferEntrada();
    }
    limparBufferEntrada();

    // Alocação dinâmica do vetor de territórios
    Territorio *mapa = (Territorio *)calloc(totalTerritorios, sizeof(Territorio));
    if (mapa == NULL)
    {
        fprintf(stderr, "Erro: falha na alocação de memória.\n");
        return 1;
    }

    // Cadastro
    printf("\n=== Cadastro de Territórios do Jogo ===\n\n");
    cadastrarTerritorios(mapa, totalTerritorios);

    // Exibição inicial do mapa
    exibirTerritorios(mapa, totalTerritorios);

    // Loop principal: pede diretamente o atacante (ou 0 para sair)
    while (1)
    {
        int idxAtq;

        printf("Escolha o território atacante (1 a %d, ou 0 para sair): ", totalTerritorios);
        if (scanf("%d", &idxAtq) != 1)
        {
            printf("Entrada inválida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }
        limparBufferEntrada();

        if (idxAtq == 0)
        {
            // sair do loop principal
            break;
        }
        if (idxAtq < 1 || idxAtq > totalTerritorios)
        {
            printf("Índice inválido. Tente novamente.\n\n");
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
            printf("Índice inválido. Voltando ao início da rodada.\n\n");
            continue;
        }

        // Ajuste dos índices
        Territorio *atacante = &mapa[idxAtq - 1];
        Territorio *defensor = &mapa[idxDef - 1];

        if (atacante == defensor)
        {
            printf("Você selecionou o mesmo território. Escolha atacante e defensor diferentes.\n\n");
            continue;
        }

        if (strcmp(atacante->cor, defensor->cor) == 0)
        {
            printf("Ataque inválido: os territórios pertencem à mesma cor.\n\n");
            continue;
        }

        // Realiza o ataque
        atacar(atacante, defensor);

        // Após mostrar o resultado, pede para o usuário pressionar Enter para continuar
        printf("\nPressione Enter para continuar para o proximo turno...");

        limparBufferEntrada();

        // Ao iniciar próxima rodada, exibe o mapa atualizado
        exibirTerritorios(mapa, totalTerritorios);
        // E o loop recomeça pedindo o atacante novamente
    }
    // Libera memória
    free(mapa);
    printf("Memória liberada. Programa encerrado.\n");

    return 0;
}