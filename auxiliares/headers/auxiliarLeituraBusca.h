//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#ifndef AUXILIAR_LEITURA_H
#define AUXILIAR_LEITURA_H

//inclui as principais bibliotecas e as únicas utilizadas neste projeto
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Estrutura auxiliar para armazenar os campos de um registro(usada para a funcionalidade 3 e 4)
struct registro_2 {
    int idPessoa, idadePessoa;
    int tamNomePessoa, tamNomeUsuario;
    char nomePessoa[100];
    char nomeUsuario[100];
};

typedef struct indice{
  int idPessoa;
  int64_t byteOffset;
} indice;

void imprimirRegistro(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario);
int64_t buscaBinariaIndice(indice *vetor, int tamanho, int idBuscado);
void imprimirRegistroPorByteOffset(FILE *arqPessoa, int64_t byteOffset, struct registro_2 reg);

#endif