//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#ifndef READ_BINARY_H
#define READ_BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//função de listar todos os registros, é a funcionalidade 3 do trabalho e tem como objetivo listar todos os registros do arquivo de dados
void listarRegistros(char *nomeArquivoEntrada);
void buscarRegistros(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n);

#endif