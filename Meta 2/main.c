#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

typedef struct
{
    char mensagem[50];
    char autor[50];
    int avaliacao;
} Feedback;

void imprimirCabecalho()
{
    printf(" ___       ___          \n");
    printf("/ __> _ _ | . > ___ __  \n");
    printf("\\__ \\| | || . \\/ . \\\\/\\/\n");
    printf("<___/`___||___/\\___//\\_\\\n");
    printf("                        \n");
    printf("---------------------------------------\n");
}

void limparConsole()
{
    system("cls");
}

void pausarExecucao()
{
    printf("\n\nPressione qualquer tecla para retornar...");
    _getch();
}

void lerFeedbacks()
{

    FILE *arquivo = fopen("feedbacks.bin", "rb");
    if (arquivo == NULL)
    {
        printf("\t\tVocê não possui feedbacks cadastrados no sistema.");

        pausarExecucao();
        return;
    }

    Feedback feedback;
    size_t resultado;

    printf("%-20s %-60s %-10s\n", "Autor", "Mensagem", "Avaliação");
    printf("%-20s %-60s %-10s\n", "--------------------", "------------------------------------------------------------", "----------");

    int i = 0;
    while ((resultado = fread(&feedback, sizeof(Feedback), 1, arquivo)) == 1)
    {
        printf("%-20s %-60s %-10d\n", feedback.autor, feedback.mensagem, feedback.avaliacao);

        i++;
    }

    if (ferror(arquivo))
    {
        perror("Erro ao ler o arquivo.\n");
    }

    fclose(arquivo);

    pausarExecucao();
}

void salvarFeedback(Feedback feedback)
{
    FILE *arquivo = fopen("feedbacks.bin", "ab+");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir arquivo para escrita.");
        exit(1);
    }

    fwrite(&feedback, sizeof(Feedback), 1, arquivo);
    fclose(arquivo);
}

void menuCadastroFeedback()
{
    setlocale(LC_ALL, "Portuguese");

    Feedback feedback;

    char resposta;
    printf("Deseja se identificar? (s/n) ");
    do
    {
        scanf(" %c", &resposta);
        resposta = tolower(resposta);

        if (resposta != 's' && resposta != 'n')
        {
            printf("Entrada inválida. Deseje uma das opções acima: ");
        }

    } while (resposta != 's' && resposta != 'n');

    if (resposta == 'n')
        strcpy(feedback.autor, "Anonimo");
    else
    {
        printf("Digite seu nome: ");
        fflush(stdin);
        gets(feedback.autor);
    }

    printf("Conte-nos pontos a serem melhorados ou pontos positivos de nossa empresa: ");
    fflush(stdin);
    gets(feedback.mensagem);

    printf("De 0 a 10, o quanto você recomendaria nossa empresa? ");
    do
    {
        scanf("%d", &feedback.avaliacao);

        if (feedback.avaliacao < 0 || feedback.avaliacao > 10)
        {
            printf("Entrada inválida. Digite uma avaliação de 0 a 10: ");
        }

    } while (feedback.avaliacao < 0 || feedback.avaliacao > 10);

    salvarFeedback(feedback);
}

void menuLoja()
{
    imprimirCabecalho();

    int navegacao;

    printf("[1] Visualizar Feedbacks\n");
    printf("[2] Cadastrar Perguntas de Feedback\n");
    printf("[3] Modo Cliente\n");
    printf("[4] Configurações\n");
    printf("[5] Sair do Programa\n");

    scanf("%i", &navegacao);

    while (navegacao < 1 || navegacao > 5)
    {
        limparConsole();
        menuLoja(&navegacao);
    }
    limparConsole();

    switch (navegacao)
    {
    case 1:
        lerFeedbacks();
        break;

    case 3:
        menuCadastroFeedback();
        break;

    case 5:
        printf("Desligando o programa.");
        exit(0);
        break;

    default:
        break;
    }
}

void salvarNomeEmpresa(char nome[])
{
    FILE *arquivo = fopen("empresa.bin", "wb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    size_t length = strlen(nome) + 1;
    fwrite(nome, sizeof(char), length, arquivo);

    fclose(arquivo);
}

int main()
{
    setlocale(LC_ALL, "");

    FILE *arquivoEmpresa = fopen("empresa.bin", "rb");
    if (arquivoEmpresa == NULL)
    {
        char nome[126];
        printf("Bem vindo ao nosso sistema! Para começar digite o nome de sua empresa: ");
        fflush(stdin);
        gets(nome);

        salvarNomeEmpresa(nome);
    }

    while (1)
    {
        limparConsole();
        menuLoja();
    }

    printf("Congratulações.");

    return 0;
}
