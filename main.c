#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char nome[51];
    int matricula;
    double notas[3];
} Aluno;

void limpaTela()
{
    printf("\nPressione ENTER para continuar...");
    getchar();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
int verificarDuplicidadeNumeroDaMatricula(int numeroDaMatricula, const char caminho[])
{
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL)
        return 0;

    int matriculaAtual;
    while (fscanf(arquivo, "%*[^|]|%i|%*f|%*f|%*f\n", &matriculaAtual) != EOF)
    {
        if (matriculaAtual == numeroDaMatricula)
        {
            fclose(arquivo);
            return 0;
        }
    }
    fclose(arquivo);
    return 1;
}
int contarAlunos(const char caminho[])
{
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL)
        return 0;

    int total = 0;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL)
    {
        total++;
    }
    fclose(arquivo);
    return total;
}

// Função para cadastrar nome, numero da matricula e notas
void cadastrarAluno(const char caminho[])
{
    if (contarAlunos(caminho) >= 100)
    {
        printf("Erro: Limite maximo de 100 alunos atingido!\n");
        return;
    }

    Aluno alunoNovo;

    printf("Me informe seu nome: ");
    while (fgets(alunoNovo.nome, sizeof(alunoNovo.nome), stdin) != NULL)
    {
        int precisaLimpar = 0;
        if (strlen(alunoNovo.nome) > 0 && alunoNovo.nome[strlen(alunoNovo.nome) - 1] != '\n')
        {
            precisaLimpar = 1;
        }
        alunoNovo.nome[strcspn(alunoNovo.nome, "\n")] = '\0';
        if (precisaLimpar || strlen(alunoNovo.nome) == 0)
        {
            if (precisaLimpar) limparBuffer();
            printf("Erro: O nome deve ter entre 1 e 50 caracteres.\n");
            printf("Me informe seu nome novamente: ");
        }
        else
        {
            break;
        }
    }

    int leituraValida;
    printf("Me informe sua matricula (numero positivo): ");
    while (1)
    {
        leituraValida = scanf("%i", &alunoNovo.matricula);
        if (leituraValida != 1 || alunoNovo.matricula <= 0 || verificarDuplicidadeNumeroDaMatricula(alunoNovo.matricula, caminho) == 0)
        {
            limparBuffer();
            printf("Erro: Matricula invalida, nula, negativa, nao-numerica ou ja existente.\n");
            printf("Me informe sua matricula novamente: ");
        }
        else
        {
            break;
        }
    }
    limparBuffer();

    for (int i = 0; i < 3; i++)
    {
        printf("Me informe sua %i° nota: ", i + 1);
        while (1)
        {
            leituraValida = scanf("%lf", &alunoNovo.notas[i]);
            if (leituraValida != 1 || alunoNovo.notas[i] < 0 || alunoNovo.notas[i] > 10)
            {
                limparBuffer();
                printf("Erro: Permitido apenas notas numericas entre 0 e 10.\n");
                printf("Me informe sua %i° nota novamente: ", i + 1);
            }
            else
            {
                break;
            }
        }
        limparBuffer();
    }

    FILE *arquivo = fopen(caminho, "a");
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
void buscarAlunoPorMatricula(const char caminho[], int buscaNumeroDaMatricula)
{
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL)
    {
        printf("Erro: Banco de dados nao existe ainda.\n");
        return;
    }
    Aluno aluno;
    while (fscanf(arquivo, "%[^|]|%i|%lf|%lf|%lf\n", aluno.nome, &aluno.matricula, &aluno.notas[0], &aluno.notas[1], &aluno.notas[2]) != EOF)
    {
        if (aluno.matricula == buscaNumeroDaMatricula)
        {
            printf("\nAluno da matricula %i encontrado\n", buscaNumeroDaMatricula);
            printf("Aluno: %s\n", aluno.nome);
            printf("Média do aluno: %.1lf\n", (aluno.notas[0] + aluno.notas[1] + aluno.notas[2]) / 3);
            fclose(arquivo);
            return;
        }
    }
    fclose(arquivo);
    printf("Nao existe a matricula %i no nosso banco de dados.\n", buscaNumeroDaMatricula);
}
// Função feita para mostrar a média de todos os alunos
void apresentarMediaDeTodosAlunos(const char caminho[])
{
    int tamanhoPagina = 10;
    int paginaAtual = 1;
    char opcao;

    do
    {
        FILE *arquivo = fopen(caminho, "r");
        if (arquivo == NULL)
        {
            printf("Nenhum registro encontrado.\n");
            return;
        }

        Aluno aluno;
        int totalRegistros = 0;
        int exibidosNaPagina = 0;

        int linhaInicial = (paginaAtual - 1) * tamanhoPagina;
        int linhaFinal = paginaAtual * tamanhoPagina;

        printf("\n===== LISTAGEM DE ALUNOS (PAGINA %i) =====\n", paginaAtual);

        while (fscanf(arquivo, "%[^|]|%i|%lf|%lf|%lf\n", aluno.nome, &aluno.matricula, &aluno.notas[0], &aluno.notas[1], &aluno.notas[2]) != EOF)
        {
            if (totalRegistros >= linhaInicial && totalRegistros < linhaFinal)
            {
                double media = (aluno.notas[0] + aluno.notas[1] + aluno.notas[2]) / 3.0;
                printf("Aluno: %-20s | Matricula: %-8i | Media: %.1f\n", aluno.nome, aluno.matricula, media);
                exibidosNaPagina++;
            }
            totalRegistros++;
        }
        fclose(arquivo);

        if (exibidosNaPagina == 0 && paginaAtual > 1)
        {
            printf("Nenhum aluno encontrado nesta pagina.\n");
        }

        printf("\n[N] Proxima Pagina | [A] Pagina Anterior | [S] Sair do Menu: ");
        scanf(" %c", &opcao);
        limparBuffer();

        if ((opcao == 'N' || opcao == 'n') && (linhaFinal < totalRegistros))
        {
            paginaAtual++;
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        }
        else if ((opcao == 'A' || opcao == 'a') && paginaAtual > 1)
        {
            paginaAtual--;
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        }
        else if (opcao != 'S' && opcao != 's')
        {
            printf("Opcao invalida ou nao ha mais paginas nessa direcao!\n");
        }

    } while (opcao != 'S' && opcao != 's');
}
// Função feita para mostrar o aluno com maior média
void destaqueAcademico(const char caminho[])
{
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL)
    {
        printf("Banco de dados vazio!\n");
        return;
    }

    Aluno aluno;
    double maiorMedia = -1.0;
    char nomeMaiorMedia[51];
    int matriculaMaiorMedia = 0;

    while (fscanf(arquivo, "%[^|]|%i|%lf|%lf|%lf\n",
                  aluno.nome, &aluno.matricula, &aluno.notas[0], &aluno.notas[1], &aluno.notas[2]) != EOF)
    {
        double media = (aluno.notas[0] + aluno.notas[1] + aluno.notas[2]) / 3.0;

        if (media > maiorMedia)
        {
            maiorMedia = media;
            matriculaMaiorMedia = aluno.matricula;
            strcpy(nomeMaiorMedia, aluno.nome);
        }
    }
    fclose(arquivo);

    if (maiorMedia >= 0)
    {
        printf("\n=== DESTAQUE ACADEMICO ===\n");
        printf("Nome: %s\n", nomeMaiorMedia);
        printf("Matricula: %i\n", matriculaMaiorMedia);
        printf("Media: %.2f\n", maiorMedia);
    }
    else
    {
        printf("Nenhum aluno encontrado no arquivo.\n");
    }
}
int main()
{
    const char caminhoTxT[] = "alunos.txt";
    int opcao;
    int matriculaBusca = 0;

    do
    {
        opcao = -1;
        printf("\n===== MENU =====\n");
        printf("1 - Cadastrar aluno\n");
        printf("2 - Buscar aluno por matricula\n");
        printf("3 - Apresentar media de todos os alunos\n");
        printf("4 - Destaque academico\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; 
        }
        limparBuffer();

        if (opcao == 1)
        {
            cadastrarAluno(caminhoTxT);
            limpaTela();
        }
        else if (opcao == 2)
        {
            printf("Digite a matricula que deseja buscar: ");
            if (scanf("%d", &matriculaBusca) != 1 || matriculaBusca <= 0)
            {
                printf("Matricula invalida!\n");
            }
            else
            {
                buscarAlunoPorMatricula(caminhoTxT, matriculaBusca);
            }
            limparBuffer();
            limpaTela();
        }
        else if (opcao == 3)
        {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            apresentarMediaDeTodosAlunos(caminhoTxT);
            limpaTela();
        }
        else if (opcao == 4)
        {
            destaqueAcademico(caminhoTxT);
            limpaTela();
        }
        else if (opcao != 0)
        {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);
    printf("Saindo...\n");
    return 0;
}