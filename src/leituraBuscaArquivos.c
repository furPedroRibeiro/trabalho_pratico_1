// Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
// Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//nesse arquivo estão as funcionalidades 3 e 4, que dizem respeito a busca nos arquivos de índice e de dados

//incluindo arquivos de cabeçalho utilizados nesse arquivo de implementação
#include "../headers/leituraBuscaArquivos.h"
#include "../auxiliares/headers/auxiliarLeituraBusca.h"
#include "../headers/utilidades.h"

// Definição da variável global
struct registro_2 reg;
//aproveitando a struct que ja foi criada em auxiliar.h
typedef struct indice indice;

//Funcionalidade 3:
void listarRegistros(char *nomeArquivoEntrada){
    // abrindo o caminho em que o arquivo está
    char caminho_2[100] = "./";
    strcat(caminho_2, nomeArquivoEntrada);
    FILE *arqPessoa = fopen(caminho_2, "rb");

    if (arqPessoa == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }

    //Leitura do cabeçalho do arquivo binário - leitura do status
    //criação de variáveis para armazenar cabeçalho
    char status;
    //lendo status
    if (fread(&status, sizeof(status), 1, arqPessoa) != 1){
        // Se o status for diferente de 1 o arquivo de dados está inconsistente
        puts("Falha no processamento do arquivo");
        fclose(arqPessoa);
        return;
    }
    // Tamanho do arquivo
    fseek(arqPessoa, 0, SEEK_END);
    long size = ftell(arqPessoa);
    // Ajustar inicio de arquivo
    fseek(arqPessoa, 17, SEEK_SET); // pula o cabeçalho

    // Ler arquivo
    while (ftell(arqPessoa) < size){
        char removido;
        fread(&removido, sizeof(char), 1, arqPessoa);
        int tamRegistro;
        fread(&tamRegistro, sizeof(int), 1, arqPessoa);
        if(removido == '1'){
            fseek(arqPessoa, tamRegistro, SEEK_CUR);
            continue;
        }
        // Lê cada campo do registro
        fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
        fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
        fread(&reg.tamNomePessoa, sizeof(int), 1, arqPessoa);
        fread(&reg.nomePessoa, sizeof(char), reg.tamNomePessoa, arqPessoa);
        reg.nomePessoa[reg.tamNomePessoa] = '\0';
        fread(&reg.tamNomeUsuario, sizeof(int), 1, arqPessoa);
        fread(&reg.nomeUsuario, sizeof(char), reg.tamNomeUsuario, arqPessoa);
        reg.nomeUsuario[reg.tamNomeUsuario]='\0';

        // Exibe o registro formatado
        // Usa função auxiliar para imprimir
        imprimirRegistro(reg.idPessoa, reg.idadePessoa, reg.tamNomePessoa, reg.nomePessoa, reg.tamNomeUsuario, reg.nomeUsuario);
    }
    //fechando arquivo
    fclose(arqPessoa);
}

//Funcionalidade 4
void buscarRegistros(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n){
    //abertura dos arquivos
    char caminho[100] = "./";
    strcat(caminho, nomeArquivoPessoa);
    FILE *arqPessoa = fopen(caminho, "rb");

    char caminho_2[100] = "./";
    strcat(caminho_2, nomeArquivoIndice);
    FILE *arquivoIndice = fopen(caminho_2, "rb");

    if(arqPessoa == NULL || arquivoIndice == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }

    //leitura do status dos arquivos
    char statusPessoa, statusIndice;
    //Leitura do status do arquivo pessoa
    if (fread(&statusPessoa, sizeof(statusPessoa), 1, arqPessoa) != 1){
        // Se o status for diferente de 1 o arquivo de dados está inconsistente
        puts("Falha no processamento do arquivo");
        fclose(arqPessoa);
        return;
    }
    //leitura do status do arquivo de indice
    if (fread(&statusIndice, sizeof(statusIndice), 1, arquivoIndice) != 1){
        // Se o status for diferente de 1 o arquivo de dados está inconsistente
        puts("Falha no processamento do arquivo");
        fclose(arquivoIndice);
        return;
    }

    //carrega o arquivo de índice em um vetor
    fseek(arquivoIndice, 0, SEEK_END);
    long sizeIndice = ftell(arquivoIndice) - 12; // remove os bytes do cabeçalho

    //calcula quantos registros de índice existem no arquivo
    int qtdIndice = sizeIndice / (sizeof(int) + sizeof(int64_t));

    //posiciona o ponteiro depois do cabeçalho
    fseek(arquivoIndice, 12, SEEK_SET);

    //aloca memória
    indice *vetorIndice = malloc(qtdIndice * sizeof(indice));
    
    //carrega o índice completo no vetor
    for(int i = 0; i < qtdIndice; i++){
        fread(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fread(&vetorIndice[i].byteOffset, sizeof(int64_t), 1, arquivoIndice);
    }
    fclose(arquivoIndice);

    //obtem o tamanho do arquivo pessoa e posiciona o ponteiro apos o cabecalho
    fseek(arqPessoa,0, SEEK_END);
    long sizeDados = ftell(arqPessoa);

    //loop de buscas
    for(int i = 0; i < n; i++){
        int entrada;
        char nomeCampo[100], valorCampo[100];
        int find = 0;

        //le a linha de busca no formato: número nomeCampo=valorCampo
        scanf("%d", &entrada);
        scanf(" %[^=]", nomeCampo);
        getchar();
        scan_quote_string(valorCampo);

        //caso 1:busca por idPessoa usando indice
        if(strcmp(nomeCampo, "idPessoa") == 0){
            int idBusca = atoi(valorCampo);
            int64_t offset = buscaBinariaIndice(vetorIndice, qtdIndice, idBusca);
            if(offset != -1){
                imprimirRegistroPorByteOffset(arqPessoa, offset, reg);
                find = 1;
            }
        } 
        //caso 2:busca sequencial por outros campos
        else {

            fseek(arqPessoa, 17, SEEK_SET); // pula o cabeçalho

            while(ftell(arqPessoa) < sizeDados){
                char removido;
                fread(&removido, sizeof(char), 1, arqPessoa);
                
                int tamRegistro;
                fread(&tamRegistro, sizeof(int), 1, arqPessoa);
                
                //pula os registros removidos
                if(removido == '1'){
                    fseek(arqPessoa, tamRegistro, SEEK_CUR);
                    continue;
                }

                //leitura dos campos do registro
                fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
                fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
                fread(&reg.tamNomePessoa, sizeof(int), 1, arqPessoa);
                if(reg.tamNomePessoa > 0){
                    fread(reg.nomePessoa, sizeof(char), reg.tamNomePessoa, arqPessoa);
                    reg.nomePessoa[reg.tamNomePessoa] = '\0';
                } else {
                    reg.nomePessoa[0] = '\0';
                }
                fread(&reg.tamNomeUsuario, sizeof(int), 1, arqPessoa);
                fread(reg.nomeUsuario, sizeof(char), reg.tamNomeUsuario, arqPessoa);
                reg.nomeUsuario[reg.tamNomeUsuario] = '\0';

                if ((strcmp(nomeCampo, "idadePessoa") == 0 && reg.idadePessoa == atoi(valorCampo)) ||
                    (strcmp(nomeCampo, "nomePessoa") == 0 && strcmp(reg.nomePessoa, valorCampo) == 0) ||
                    (strcmp(nomeCampo, "nomeUsuario") == 0 && strcmp(reg.nomeUsuario, valorCampo) == 0)) {
                    imprimirRegistro(reg.idPessoa, reg.idadePessoa, reg.tamNomePessoa, reg.nomePessoa, reg.tamNomeUsuario, reg.nomeUsuario);
                    find = 1;
            }
            }
        }
        if(find == 0){
        printf("Registro inexistente.\n\n");
        }
    }

    free(vetorIndice);
    fclose(arqPessoa);
}