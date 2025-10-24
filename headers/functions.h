//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "./default.h"
#include "./auxiliar.h"

//esse arquivo é usado para guiar o desenvolvimento de functions.c, que por sua vez é o arquivo que contém as funcionalidades do trabalho prático nomeadas
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//funcionalidade 1
void criarIndice(char *nomeArquivoIndice);

//funcionalidade 2
void criarArquivoDados(char *nomeArquivoEntrada, char *nomeArquivoSaida, char *nomeArquivoIndice);

//funcionalidade 3
void listarRegistros(char *nomeArquivoEntrada);

//funcionalidade 4
void buscarRegistros(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n);

#endif