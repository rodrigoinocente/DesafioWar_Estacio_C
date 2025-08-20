#include <stdio.h>
#include <string.h>

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
    while ((c = getchar()) != '\n' && c != EOF);
}

int main()
{

    // Vetor para armazenar 5 territórios
    Territorio territorios[5];
    int i;

    // Cadastro dos territórios
    printf("=== Cadastro de Territórios do Jogo War ===\n\n");
    for (i = 0; i < 5; i++)
    {
        printf("Cadastro do %dº território:\n", i + 1);

        printf("Nome do território: ");
        fgets(territorios[i].nome, MAX_NOME, stdin);

        printf("Cor do exército: ");
        fgets(territorios[i].cor, MAX_COR, stdin);
        
        //Retirar o "\n" inserido ao final do char
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &territorios[i].tropas);    
        limparBufferEntrada();

        printf("\n");
    }

    // Exibir os dados
    printf("\n===============================\n");
    printf("\n==> Territórios Cadastrados <==\n");
    printf("\n===============================\n");

    for (i = 0; i < 5; i++)
    {
        printf("Território %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
        printf("-----------------------------\n");
    }

    return 0;
}