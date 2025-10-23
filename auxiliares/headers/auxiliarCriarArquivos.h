//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//esse arquivo de cabeçalho .h tem como essência definir as funções, variáveis, etc. que podem ser usadas e implementadas por todos os arquivos .c, portanto são funções, estruturas de dados e variáveis auxiliares
#ifndef AUXILIAR_CRIAR_H
#define AUXILIAR_CRIAR_H

//inclui as principais bibliotecas e as únicas utilizadas neste projeto
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

//FUNÇÃO DE LER O ARQUIVO CSV
void lerCSV(FILE* arquivoEntrada, FILE* arquivoDados, FILE* arquivoIndice);
//função para criar o cabeçalho do arquivo de dados, se der errado retorna 0
void criaCabecalhoArquivoDados(FILE* arquivoDados, char status, int quantidadePessoas, int quantidadeRemovidos, int64_t proxByteoffset);
//função para inserir um registro no arquivo de dados, se der errado retorna 0
void insereRegistro(registro* novoRegistro, FILE* arquivoDados, int quantidadeRemovidos, int64_t proxByteoffset);
//função para inserir os registros de índice no arquivo de índice
void insereRegistroIndice(indice* raizListaIndice, FILE* arquivoIndice);
//FUNÇÃO PARA CRIAR UM NÓ DE REGISTRO E ADICIONÁ-LO A LISTA DE REGISTROS, ESSA FUNÇÃO É CHAMADA DENTRO DE LERCSV()
void criarNoRegistro(registro* novoRegistro, char *campoIdPessoa, char *campoIdadePessoa, char *campoNomePessoa, char *campoNomeUsuario, int tamRegistroBytes);
//Função para criar um nó e adicioná-lo a lista de registros do arquivo de indice, não retorna nada
void criarNoRegistroIndice(indice* novoRegistroIndice, char *campoIdPessoa, int64_t byteoffset);

//função strsep aqui(não roda em windows a função strsep definida direto pelo GNU(se não me engano))
char* meu_strsep(char** , const char* delim);

#endif