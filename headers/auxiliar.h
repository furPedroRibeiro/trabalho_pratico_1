#include "default.h"

#ifndef AUXILIAR_H
#define AUXILIAR_H

//estrutura de dados do tipo lista para armazenar os registros antes de escrever no arquivo de dados que será gerado, a lista possui encadeamento simples porque não há necessidade dos dados serem ordenados nem nada do tipo, a única necessidade é que a lista exista e o encadeamento seja feito de maneira correta
typedef struct registro{
  char removido[2];
  int tamRegistro;
  int idPessoa;
  int idadePessoa;
  int tamanhoNomePessoa;
  char nome[100];
  int tamanhoNomeUsuario;
  char nomeUsuario[100];
  struct registro *proxRegistro;
} registro;

//estrutura de dados do tipo lista duplamente encadeada para armazenar os registros do arquivo de índice antes de inseri-lo, é duplamente encadeada pois na hora da inserção é necessário inserir um registro no meio de outros registros, necessitando assim que seja verificado o nó anterior e o próximo da lista(talvez na implementação da inserção seja observado que a lista não tem necessidade de ser duplamente encadeada, mas para maior eficiência no futuro, é duplamente encadeada desde já) 
typedef struct indice{
  int idPessoa;
  int64_t byteOffset;
  struct indice *proxIndice;
  struct indice *antIndice;
} indice;

// Estrutura auxiliar para armazenar os campos de um registro(usada para a funcionalidade 3 e 4)
struct registro_2 {
    int idPessoa, idadePessoa;
    int tamNomePessoa, tamNomeUsuario;
    char nomePessoa[100];
    char nomeUsuario[100];
};

typedef struct indice2{
  int idPessoa;
  int64_t byteOffset;
} indice2;

registro* raizListaPessoas;
indice* raizListaIndice;

//funções para as funcionalidades 1 e 2
//FUNÇÃO DE LER O ARQUIVO CSV
void lerCSV(FILE* arquivoEntrada, FILE* arquivoDados, FILE* arquivoIndice);
void criaCabecalhoArquivoDados(arqDados, status, quantidadePessoas, quantidadeRemovidos, proxByteoffset);
//função para inserir um registro no arquivo de dados, se der errado retorna 0
void insereRegistro(registro* novoRegistro, FILE* arquivoDados, int quantidadeRemovidos, int64_t proxByteoffset);
//função para inserir os registros de índice no arquivo de índice
void insereRegistroIndice(indice* raizListaIndice, FILE* arquivoIndice);
//FUNÇÃO PARA CRIAR UM NÓ DE REGISTRO E ADICIONÁ-LO A LISTA DE REGISTROS, ESSA FUNÇÃO É CHAMADA DENTRO DE LERCSV()
void criarNoRegistro(registro* novoRegistro, char *campoIdPessoa, char *campoIdadePessoa, char *campoNomePessoa, char *campoNomeUsuario, int tamRegistroBytes);
//Função para criar um nó e adicioná-lo a lista de registros do arquivo de indice, não retorna nada
void criarNoRegistroIndice(indice* novoRegistroIndice, char *campoIdPessoa, int64_t byteoffset);

//funções para as funcionalidade 3 e 4
void imprimirRegistro(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario);
int64_t buscaBinariaIndice(indice2 *vetor, int tamanho, int idBuscado);
void imprimirRegistroPorByteOffset(FILE *arqPessoa, int64_t byteOffset, struct registro_2 reg);


//função strsep aqui(não roda em windows a função strsep definida direto pelo GNU(se não me engano))
char* meu_strsep(char** , const char* delim);

#endif