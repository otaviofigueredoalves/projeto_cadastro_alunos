# Documentação do projeto de Sistema de Cadastro de Alunos
<p>Avaliação final do semestre 1 de ciência da computação</p>

> 🚧 **Status do Projeto:** Em Desenvolvimento 

Este projeto consiste em um sistema de gerenciamento escolar desenvolvido na linguagem **C**. O objetivo principal é consolidar os conceitos de estruturas de dados (`struct`), persistência de dados em arquivos de texto, manipulação de fluxos de entrada/saída (`I/O`) e técnicas avançadas de validação e programação defensiva.

---
## 📋 Descrição do Projeto

O sistema permite gerenciar o ciclo de vida dos dados de alunos, realizando o cadastro em tela, a validação rigorosa dos inputs do usuário e o salvamento persistente em um arquivo de texto estruturado. Os dados manipulados por estudante são:
* **Nome:** Texto de até 50 caracteres (com suporte a espaços e acentuação).
* **Matrícula:** Código numérico único de exatamente 8 dígitos.
* **Notas:** Três avaliações com valores reais entre 0.0 e 10.0.

---
## 🚀 Funcionalidades Implementadas

* **Cadastro Avançado de Alunos (`cadastrarAluno`):** Coleta as informações tratando erros de preenchimento em tempo de execução.
* **Busca por Matrícula (`buscarAlunoPorMatricula`):** Varre o arquivo de texto em busca de uma chave numérica única e exibe o nome correspondente.
* **Prevenção de Duplicidade (`verificarDuplicidadeNumeroDaMatricula`):** Impede que duas pessoas sejam registradas com o mesmo número de matrícula.
* **Validação Algorítmica de Nomes (`verificarCaracteresValidosNome`):** Filtro personalizado que aceita apenas letras (com ou sem acentuação) e espaços estruturados, rejeitando caracteres especiais ou números no campo de identificação.

---
## 💾 Estrutura de Dados e Armazenamento

Os dados são organizados na memória através de uma estrutura (`struct`) e salvos no arquivo `alunos.txt` utilizando o formato delimitado por pipes (`|`). Esse padrão funciona de forma semelhante a um arquivo `.csv`, otimizando o espaço em disco e facilitando a leitura estruturada.

### Modelo da Struct (C)
```c
typedef struct {
    char nome[51];
    int matricula;
    double notas[3];
} Aluno;
```
### Layout do Arquivo Físico (alunos.txt)
```md
João Pedro|94732156|9.0|7.0|6.0
Inácio|12345678|8.0|7.0|6.0
```

---
## 🛠️ Destaques Técnicos e Boas Práticas

O código foi construído focando na estabilidade do software (programação defensiva). Abaixo estão listadas as principais técnicas que tornam o programa robusto:

### 1. Tratamento Extremo de Estouro de Buffer (Buffer Overflow)
Em vez do perigoso scanf("%s") para strings, o projeto utiliza o fgets(). Caso o usuário digite um nome maior do que o limite de 50 caracteres da struct, o programa detecta a ausência do caractere de quebra de linha (\n), ativa uma flag de limpeza, limpa o buffer do teclado com a função limparBuffer() e solicita o dado novamente, evitando o corrompimento da memória.

### 2. Tratamento de Tipos Inválidos no Teclado
Se uma função espera um número inteiro (como na matrícula) e o usuário digita uma letra, o buffer do C costuma travar em um loop infinito. A arquitetura implementada limpa o fluxo do stdin sistematicamente com getchar() após falhas, reestabelecendo o estado padrão do teclado.

### 3. Extração Cirúrgica de Dados com Supressão de Atribuição
Para buscar matrículas ou validar duplicidades sem gastar memória alocando variáveis desnecessárias (como nomes e notas), o projeto utiliza o recurso de asterisco (*) no formato do fscanf:

#### Modelo de coleta de dados da matricula (C)
```c
fscanf(arquivo, "%*[^|]|%i|%*f|%*f|%*f\n", &matriculaAtual);
```
💡 Nota técnica: O "%*[^|]" e o "%*f" dizem ao compilador para ler o nome e as notas do arquivo para mover o ponteiro de leitura adiante, mas descartar os valores imediatamente da memória, isolando apenas o inteiro %i correspondente à matrícula.

### 4. Validação Matemática de Dígitos
A validação do tamanho exato da matrícula (8 dígitos) é feita através de aritmética de divisão inteira, dispensando a conversão do dado para string:

#### Modelo de validação númerica (C)
```c
while (alunoNovo.matricula / 10000000 == 0 || alunoNovo.matricula / 10000000 >= 10)
```

---
## 🗺️ Próximos Passos (Roadmap de Desenvolvimento)

O projeto encontra-se em desenvolvimento ativo. As seguintes funcionalidades e melhorias estão planejadas para as próximas iterações:

### 🎯 Funcionalidades Principais
- [X] **Menu Interativo:** Implementação de uma interface textual estruturada via terminal no método `main` para navegação do usuário.
- [ ] **Exibição Geral do Sistema:** Criação de uma função para ler o arquivo e apresentar a listagem completa de todos os alunos registrados juntamente com suas respectivas matrículas.
- [/] **Métricas de Desempenho e Médias:** Desenvolver o cálculo automático da média aritmética das 3 notas de cada estudante. O formato de exibição desta métrica deve ser de forma individual, buscando o aluno por matrícula,  e de forma geral apresentando todos os alunos.
  * [ ] Exibição da média de forma coletiva (junto à listagem geral de todos os alunos);
  * [X] Consulta individualizada (através de uma função de busca por matrícula ou por nome);
- [ ] **Destaque Acadêmico:** Desenvolver um algoritmo de busca para identificar e exibir em tela o aluno que possui a **maior média geral** do sistema.

### ⚙️ Escalabilidade e Otimizações(Se for realmente necessário)
- [ ] **Tratamento de Paginação:** Adaptar a leitura do arquivo para trabalhar em blocos de dados (ex: exibir de 10 em 10 alunos), garantindo estabilidade e baixo consumo de memória RAM caso a base de dados ultrapasse a meta inicial de 100 alunos.

### ⏳ Funcionalidades Secundárias (Se houver tempo hábil)
- [ ] **Módulo de Edição/Remoção:** Criação de rotinas para atualizar notas/nomes ou deletar o registro de alunos diretamente no arquivo `.txt`.