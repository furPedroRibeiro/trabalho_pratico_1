//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "./default.h"
#include "./auxiliar.h"

//esse arquivo de cabeçalho .h tem como essência definir as funções, variáveis, etc. que serão usadas e implementadas em create_file.c
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void criarIndice(char *nomeArquivoIndice);
void criarArquivoDados(char *nomeArquivoEntrada, char *nomeArquivoSaida, char *nomeArquivoIndice);
void listarRegistros(char *nomeArquivoEntrada);
void buscarRegistros(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n);

#endif