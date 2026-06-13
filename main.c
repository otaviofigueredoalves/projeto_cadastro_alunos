#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char nome[51];
    int matricula;
    double notas[3];
} Aluno;
// Função feita para validar se o nome tem caracteres validos: letras com ou sem acento ou espaços
int verificarCaracteresValidosNome(char string[])
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
                return 0;
            }
        }
    }
    return 1;
}
// Verifica se dentro do arquivo ja tem aquela matricula
int verificarDuplicidadeNumeroDaMatricula(int numeroDaMatricula, FILE *arquivo, const char caminho[])
{
    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) return 1;
    int matriculaAtual; 
    while (fscanf(arquivo, "%*[^|]|%d|%*f|%*f|%*f\n", &matriculaAtual) != EOF) {
        if(matriculaAtual == numeroDaMatricula)
        {
            fclose(arquivo);
            return 0;
        }
    }
    fclose(arquivo);
    return 1;
}
// Função auxiliar para limpar o buffer do teclado sempre que usarmos scanf
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void cadastrarAluno(FILE *arquivo, const char caminho[])
{
    Aluno alunoNovo;
    // Inicia as variaveis para caso usuário tente adicionar outro tipo nas variaveis(exemplo: variavel inteiro e o usuário coloca um 'c')
    alunoNovo.matricula = 0;
    for (int i = 0; i < 3; i++)
    {
        alunoNovo.notas[i] = -1;
    }

    printf("Me informe seu nome:");
    // Ler a linha que vc escreveu e se tiver mais de 50 caracteres ou algum caracter inválido ele da erro e pede para escrever o nome novamente
    while (fgets(alunoNovo.nome, sizeof(alunoNovo.nome), stdin) != NULL)
    {
        int precisaLimpar = 0;
        if (strlen(alunoNovo.nome) > 0 && alunoNovo.nome[strlen(alunoNovo.nome) - 1] != '\n')
        {
            precisaLimpar = 1;
        }
        alunoNovo.nome[strcspn(alunoNovo.nome, "\n")] = '\0';
        if (precisaLimpar || strlen(alunoNovo.nome) == 0 || verificarCaracteresValidosNome(alunoNovo.nome) == 0)
        {
            if (precisaLimpar)
            {
                limparBuffer();
            }
            printf("Erro: O nome deve ter entre 1 e 50 caracteres validos.\n");
            printf("Me informe seu nome novamente: ");
        }
        else
        {
            break;
        }
    }

    printf("Me informe sua matricula:");
    scanf("%i", &alunoNovo.matricula);
    while (alunoNovo.matricula / 10000000 == 0 || alunoNovo.matricula / 10000000 >= 10 || verificarDuplicidadeNumeroDaMatricula(alunoNovo.matricula,arquivo,caminho) == 0)
    {
        limparBuffer();
        printf("Erro, sua matricula tem menos ou mais de 8 digitos numéricos ou vc digitou uma matricula ja existente\n");
        printf("Me informe sua matricula novamente:");
        scanf("%i", &alunoNovo.matricula);
    }
    limparBuffer();

    for (int i = 0; i < 3; i++)
    {
        printf("Me informe sua %i° nota:", i + 1);
        scanf("%lf", &alunoNovo.notas[i]);
        while (alunoNovo.notas[i] < 0 || alunoNovo.notas[i] > 10)
        {
            limparBuffer();
            printf("Erro: Permitido apenas notas entre 0 e 10\n");
            printf("Me informe sua %i° nota novamente:", i + 1);
            scanf("%lf", &alunoNovo.notas[i]);
        }
        limparBuffer();
    }

    arquivo = fopen(caminho, "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(arquivo, "%s|%i|%.1f|%.1f|%.1f\n",
            alunoNovo.nome, alunoNovo.matricula, alunoNovo.notas[0], alunoNovo.notas[1], alunoNovo.notas[2]);

    fclose(arquivo);
    printf("\nAluno cadastrado com sucesso!\n");
}

int main()
{
    FILE *alunos = NULL;
    const char caminhoTxT[] = "alunos.txt";

    cadastrarAluno(alunos, caminhoTxT);

    return 0;
}