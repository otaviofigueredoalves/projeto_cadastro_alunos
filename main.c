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
    if (arquivo == NULL) return 0;
    int matriculaAtual;
    // Corro todo o arquivo pegando o valor da matricula
    while (fscanf(arquivo, "%*[^|]|%i|%*f|%*f|%*f\n", &matriculaAtual) != EOF)
    {
        // Se a matricula que eu encontrei for igual a entregada na função significa que é uma matricula duplicada
        if (matriculaAtual == numeroDaMatricula)
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
    /* Caso o usuario digite um valor errado, exemplo: numero muito grande para um inteiro ou digitar um 'd'para 
    uma variavel do tipo inteiro é necessario limpar o buffer para evitar loops infinitos
    */
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
// Função para cadastrar nome, numero da matricula e notas
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
        // Verifica se o ultimo caracter da linha é '\n' se não for, significa que o usuario escreveu mais de 50 caracteres e vai ser necessario limpar o buffer
        if (strlen(alunoNovo.nome) > 0 && alunoNovo.nome[strlen(alunoNovo.nome) - 1] != '\n')
        {
            precisaLimpar = 1;
        }
        // Acresenta '\0' na posição em que o '\n' está
        alunoNovo.nome[strcspn(alunoNovo.nome, "\n")] = '\0';
        // Se o usuário escreveu mais de 50 caracteres ou caracteres invalidos ou não escreveu nada, é solicitado que ele informe o nome novamente
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
    // Se a matricula tiver menos ou mais de 8 digitos ou se ja houver essa matricula dentro do arquivo, é solicitado que ele digite outra matricula
    while (alunoNovo.matricula / 10000000 == 0 || alunoNovo.matricula / 10000000 >= 10 || verificarDuplicidadeNumeroDaMatricula(alunoNovo.matricula, arquivo, caminho) == 0)
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
        // Se a nota digitada for menor ou maior que 10 é uma nota invalida e vai ser solicitado digitar novamente a nota
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
    printf("Aluno cadastrado com sucesso!\n");
}
// Função feita para buscar aluno por matricula
void buscarAlunoPorMatricula(FILE *arquivo, const char caminho[], int buscaNumeroDaMatricula)
{
    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) printf("Erro, arquivo não abriu");
    int matriculaAtual;
    char auxNome[51];
    // Ele corre todo o arquivio e pega o nome e o numero da matricula do aluno
    while (fscanf(arquivo, "%[^|]|%i|%*f|%*f|%*f\n", auxNome, &matriculaAtual) != EOF)
    {
        // Se o número da matriculaAtual for igual ao buscaNumeroDaMatricula apresenta o aluno na tela que tem tal matricula
        if (matriculaAtual == buscaNumeroDaMatricula)
        {
            printf("O aluno portador da matricula %i é o %s", buscaNumeroDaMatricula, auxNome);
            fclose(arquivo);
            return;
        }
    }
    fclose(arquivo);
    // Se ele percorrer o arquivo todo e não encontrar o número da matricula la dentro, siginifica que não existe aquela matricula dentro do arquivo
    printf("O aluno portador da matricula %i não foi encontrado", buscaNumeroDaMatricula);
}
int main()
{
    FILE *alunos = NULL;
    const char caminhoTxT[] = "alunos.txt";
    cadastrarAluno(alunos,caminhoTxT);
    return 0;
}