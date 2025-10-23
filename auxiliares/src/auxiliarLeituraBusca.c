//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/auxiliarLeituraBusca.h"

// Funcao auxiliar para imprimir um registro
void imprimirRegistro(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario){
    printf("Dados da pessoa de codigo %d\n", idPessoa);
    
    if (tamNomePessoa > 0) {
        printf("Nome: %s\n", nomePessoa);
    } else {
        printf("Nome: -\n");
    }
    
    if (idadePessoa == -1) {
        printf("Idade: -\n");
    } else {
        printf("Idade: %d\n", idadePessoa);
    }
    
    if (tamNomeUsuario > 0) {
        printf("Usuario: %s\n\n", nomeUsuario);
    } else {
        printf("Usuario: -\n\n");
    }
}

//busca binaria para encontrar o byteOffset do registro com o id buscado
int64_t buscaBinariaIndice(indice *vetor, int tamanho, int idBuscado){
    int begin = 0;
    int end = tamanho - 1;

    while (begin <= end){
        int meio = (begin + end)/2;

        if(vetor[meio].idPessoa == idBuscado){
            return vetor[meio].byteOffset;
        } else if (vetor[meio].idPessoa < idBuscado){
            begin = meio + 1;
        } else{
            end = meio - 1;
        }
    }
    //se nao encontrar o indice fornecido
    return -1;
}

//imprimir registro encontrado por byteOffset
void imprimirRegistroPorByteOffset(FILE *arqPessoa, int64_t byteOffset, struct registro_2 reg){
    //posiciona o ponteiro no registro 
    fseek(arqPessoa, byteOffset, SEEK_SET);

    char removido;
    fread(&removido, sizeof(char), 1, arqPessoa);
    if (removido == '1'){
        return; // registro marcado como removido
    }

    //le o tamanho do registro
    int tamRegistro;
    fread(&tamRegistro, sizeof(int), 1, arqPessoa);

    //lê cada campo do registro
    fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
    fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
    fread(&reg.tamNomePessoa, sizeof(int), 1, arqPessoa);
    fread(&reg.nomePessoa, sizeof(char), reg.tamNomePessoa, arqPessoa);
    reg.nomePessoa[reg.tamNomePessoa] = '\0';
    fread(&reg.tamNomeUsuario, sizeof(int), 1, arqPessoa);
    fread(&reg.nomeUsuario, sizeof(char), reg.tamNomeUsuario, arqPessoa);
    reg.nomeUsuario[reg.tamNomeUsuario] = '\0';

    //usa função auxiliar para imprimir
    imprimirRegistro(reg.idPessoa, reg.idadePessoa, reg.tamNomePessoa, reg.nomePessoa, reg.tamNomeUsuario, reg.nomeUsuario);
}