#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <unistd.h>

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

void imprimirNomeEmpresa()
{
    char nome[126];
    FILE *arquivo = fopen("empresa.bin", "rb");

    if (arquivo == NULL)
    {
        perror("Erro ao abrir arquivo para leitura.");
        exit(1);
    }

    fread(nome, sizeof(char), 126, arquivo);
    fclose(arquivo);

    printf("%s", nome);
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

void lerFeedbacks()
{

    FILE *arquivo = fopen("feedbacks.bin", "rb");
    if (arquivo == NULL)
    {
        printf("\t\tVocê nao possui feedbacks cadastrados no sistema.");

        pausarExecucao();
        return;
    }

    Feedback feedback;
    size_t resultado;

    printf("%-20s %-60s %-10s\n", "Autor", "Mensagem", "Avaliacao");
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

void salvarFeedbacks(Feedback feedbacks[], int quantidade)
{
    FILE *arquivo = fopen("feedbacks.bin", "ab+");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir arquivo para escrita.");
        exit(1);
    }

    for (int i = 0; i < quantidade; i++)
    {
        fwrite(&feedbacks[i], sizeof(Feedback), 1, arquivo);
    }

    fclose(arquivo);
}

void menuCadastroFeedback()
{
    Feedback feedbacks[100];

    int i = 0;
    while (1)
    {
        Feedback feedback;

        char resposta;
        char continuar;

        printf("[1]. Deseja se identificar? (s/n) ");
        do
        {
            scanf(" %c", &resposta);
            resposta = tolower(resposta);

            if (resposta != 's' && resposta != 'n')
            {
                printf("Entrada invalida. Deseje uma das opções acima: ");
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

        printf("[2]. Conte-nos pontos a serem melhorados ou pontos positivos de na empresa \'");
        imprimirNomeEmpresa();
        printf("\': ");

        fflush(stdin);
        gets(feedback.mensagem);

        printf("[3]. De 0 a 10, o quanto você recomendaria nossa empresa? ");
        do
        {
            scanf("%d", &feedback.avaliacao);

            if (feedback.avaliacao < 0 || feedback.avaliacao > 10)
            {
                printf("Entrada invalida. Digite uma avaliacao de 0 a 10: ");
            }

        } while (feedback.avaliacao < 0 || feedback.avaliacao > 10);

        limparConsole();

        printf("\t\tNos da empresa \'");
        imprimirNomeEmpresa();
        printf("\' agradecemos sua opiniao!");
        sleep(5);
        limparConsole();

        printf("Deseja continuar no modo cliente (s/n)? ");
        do
        {
            scanf(" %c", &continuar);
            continuar = tolower(continuar);

            if (continuar != 's' && continuar != 'n')
            {
                printf("Entrada invalida. Deseje uma das opções acima: ");
            }
        } while (continuar != 's' && continuar != 'n');

        feedbacks[i] = feedback;

        if (continuar == 'n')
            break;
        i++;
    }

    salvarFeedbacks(feedbacks, i + 1);
}

void menuNomeEmpresa()
{
    FILE *arquivoEmpresa = fopen("empresa.bin", "rb");
    if (arquivoEmpresa == NULL)
    {
        char nome[126];

        imprimirCabecalho();
        printf("Bem vindo ao nosso sistema! Para começar digite o nome de sua empresa: ");
        fflush(stdin);
        gets(nome);

        salvarNomeEmpresa(nome);
    }
}

void menuAlterarNomeEmpresa()
{
    char nome[126];

    FILE *arquivoEmpresa = fopen("empresa.bin", "rb");
    if (arquivoEmpresa == NULL)
    {
        perror("Erro ao abrir arquivo para leitura");
        exit(1);
    }

    printf("Digite o novo nome para alteracao: ");
    fflush(stdin);
    gets(nome);

    salvarNomeEmpresa(nome);
}

void menuLoja()
{
    imprimirCabecalho();

    int navegacao;

    printf("[1] Visualizar Feedbacks\n");
    printf("[2] Modo Cliente\n");
    printf("[3] Alterar nome da empresa\n");
    printf("[4] Sair do Programa\n");

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

    case 2:
        menuCadastroFeedback();
        break;

    case 3:
        menuAlterarNomeEmpresa();
        break;

    case 4:
        printf("Desligando o programa.");
        exit(0);
        break;

    default:
        break;
    }
}

int main()
{
    setlocale(LC_ALL, "");
    system("chcp 1252 > nul");

    menuNomeEmpresa();

    while (1)
    {
        limparConsole();
        menuLoja();
    }

    return 0;
}
