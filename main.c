#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

typedef struct
{
    char nome[51];
    int matricula;
    double notas[3];
} Aluno;
// Função feita para escrever uma string sem limite de caracteres.
void colocarValorString(char **string, const size_t tamanhoMax)
{
    size_t auxTamanhoMax = tamanhoMax;
    size_t posicaoString = 0;
    char caracterAtual;
    /* O codigo abaixo ler caracter por caracter ate o usuario clicar "enter" ou o terminal for fechado. Se não tiver
    o "caracterAtual != EOF" pode acontecer do codigo rodar em segundo plano por um breve momento na hora que fechar
    o terminal, podendo estourar a memória RAM do computador.
    */
    while ((caracterAtual = (char)getchar()) != '\n' && caracterAtual != EOF)
    {
        // Se a posição atual da minha string + 1, ultrapasar o meu tamanho maximo da minha string, o tamanho da minha string vai ser duplicado
        if (posicaoString + 1 >= auxTamanhoMax)
        {
            auxTamanhoMax *= 2;
            char *temporario = (char *)realloc(*string, auxTamanhoMax * sizeof(char));
            if (temporario == NULL)
            {
                printf("Erro ao realocar memória\n");
                free(*string);
                return;
            }
            *string = temporario;
        }
        (*string)[posicaoString] = caracterAtual;
        posicaoString++;
    }
    (*string)[posicaoString] = '\0';
}
// Função feita para validar se o nome tem so caracteres válidos (letras com e sem acento e espaço).
int verificarNome(char string[])
{
    for (size_t i = 0; i < strlen(string); i++)
    {
        // Se a minha string tiver caracteres diferentes de letras sem assentos entra aqui.
        if (((string[i] < 65 || (string[i] > 90 && string[i] < 97)) || string[i] > 122) && string[i] != 32)
        {
            // Guardo todas as letras com acentos.
            const char *acentos_validos = "áàãâéêíóôõúçÁÀÃÂÉÊÍÓÔÕÚÇ";
            char *temAcento;
            // Valida se a "string[i]" esta presente no vetor "acentos_validos" , se tiver significa que ela é uma letra com acento.
            temAcento = strchr(acentos_validos, string[i]);
            // Se o caracter não for uma letra com acento, ele para o codigo na hora e retorna 1.
            if (temAcento == NULL)
            {
                return 1;
            }
        }
    }
    return 0;
}
// Função feita para validar se o numero da matricula têm somente numeros
int verificarMatricula(char string[])
{
    for (size_t i = 0; i < strlen(string); i++)
    {
        // Se a minha string tiver caracteres diferentes de numeros ela para e retorna 1.
        if (string[i] < 48 || string[i] > 57)
        {
            return 1;
        }
    }
    return 0;
}
int verificarNota(char string[])
{
    int virgulaApareceu = 0;
    for (size_t i = 0; i < strlen(string); i++)
    {
        if (string[i] < 48 || string[i] > 57)
        {
            if (string[i] == ',' && virgulaApareceu == 0)
            {
                string[i] = '.';
                virgulaApareceu = 1;
            }
            else
            {
                return 1;
            }
        }
    }
    double teste = atof(string);
    if (teste < 0 || teste > 10)
    {
        return 1;
    }
    return 0;
}
void cadastrarAluno(FILE *arquivo, const char caminhoDoArquivo[])
{
    Aluno alunoNovo;
    // const size_t tamanhoMaxNome = 51;
    // char *auxNome = (char *)calloc(tamanhoMaxNome, sizeof(char));
    // printf("Me informe seu nome:");
    // colocarValorString(&auxNome, tamanhoMaxNome);
    // while (strlen(auxNome) > 50 || verificarNome(auxNome) == 1)
    // {
    //     printf("\nErro,no nome ultrapassou o limite maximo de caracteres (50 caracteres) ou foi colocado um caracter invalido");
    //     printf("\nCaracteres validos: letras com ou sem acento e espaço");
    //     printf("\nMe informe seu nome novamente:");
    //     colocarValorString(&auxNome, tamanhoMaxNome);
    // }
    // strcpy(alunoNovo.nome, auxNome);
    // free(auxNome);

    // const size_t tamanhoMaxMatricula = 10;
    // // Crio uma variavel auxiliar para caso os usuarios tente add letras ou outros caracteres especiais.
    // char *auxMatricula = (char *)calloc(tamanhoMaxMatricula, sizeof(char));
    // printf("Me informe sua matricula:");
    // colocarValorString(&auxMatricula, tamanhoMaxMatricula);
    // while (strlen(auxMatricula) != 10 || verificarMatricula(auxMatricula) == 1)
    // {
    //     printf("Erro, voçê digitou um numéro menor ou maior que 10 digitos ou digitos invalidos");
    //     printf("\nNa matricula so é aceito números de 0 a 9");
    //     printf("\nMe informe sua matricula novamente:");
    //     colocarValorString(&auxMatricula, tamanhoMaxMatricula);
    // }
    // // Apos garantir que minha variavel aux tem 10 digitos e so tem numeros eu transformo ela em int.
    // alunoNovo.matricula = atoi(auxMatricula);
    // free(auxMatricula);

    const size_t tamanhoMaxNota = 4;
    for (int i = 0; i < 3; i++)
    {
        char *auxNota = (char *)calloc(tamanhoMaxNota, sizeof(char));
        printf("Me informe sua %i° nota:", i + 1);
        colocarValorString(&auxNota, tamanhoMaxNota);
        while (strlen(auxNota) > 4 || verificarNota(auxNota) == 1)
        {
            printf("Erro, só é aceito notas com no maximo duas casas decimais e notas de 0 a 10");
            printf("\nMe informe sua %i° nota novamente:", i + 1 );
            colocarValorString(&auxNota, tamanhoMaxNota);
        }
        alunoNovo.notas[i] = atof(auxNota);
        printf("%f\n",alunoNovo.notas[i]);
    }
    // printf("%s|%i\n", alunoNovo.nome, alunoNovo.matricula);
    // arquivo = fopen(caminhoDoArquivo, "a");
    // if (arquivo == NULL)
    // {
    //     printf("ERRO AO ABRIR");
    //     return;
    // }
    // fclose(arquivo);
}

int main()
{
    /* os metodos "SetConsoleCP" e "SetConsoleOutputCP" foram colocados para garantir que a entrada e saida tenham
    os mesmos valores com relação na tabela ASCII e o metodo "setlocale" foi para garantir que o algoritimo vai
    ficar no padão do portugues */
    setlocale(LC_ALL, "Portuguese");
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);
    FILE *alunos;
    const char caminhoTxt[] = "alunos.txt";
    cadastrarAluno(alunos, caminhoTxt);
    return 0;
}