//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//createFiles.c tem como principal função usar e implementar funções e variáveis definidas em createFiles.h e algumas outras funções e variáveis vindas de outro arquivo
//nesse arquivo estão as funcionalidades 1 e 2, que dizem respeito a criação de arquivo de índice e escrita nos arquivos de índice e de dados 
#include "../headers/criarArquivos.h"
#include "../auxiliares/headers/auxiliarCriarArquivos.h"

//inclui utilidades.h para usar a função binário na tela
#include "../headers/utilidades.h"

//função para criar o arquivo de indice
void criarIndice(char *nomeArquivoIndice){
  //caminho onde os binário devem ser criados:
  char caminho[100] = "./";
  //concatena o caminho com o nome do arquivo
  strcat(caminho, nomeArquivoIndice);
  //o que deve ser feito aqui é a abertura(criação) do arquivo de índice, que nesse momento deve conter apenas o cabeçalho
  //o arquivo em sua criação, tem status e ele está consistente
  char status = '1'; 
  //devemos adicionar o lixo ao cabeçalho também
  char lixo[] = "$$$$$$$$$$$";

  //cria o arquivo com o caminho e nome concatenados em strcat
  FILE *arqIndice = fopen(caminho, "wb");
  if(arqIndice == NULL){
    printf("Falha no processamento do arquivo.");
    return;
  }
  //escreve o cabeçalho no arquivo
  fwrite(&status, sizeof(char), 1, arqIndice);
  fwrite(lixo, sizeof(char), strlen(lixo), arqIndice);

  //fecha o arquivo
  fclose(arqIndice);

  //usa binario na tela como especificado no trabalho
  binarioNaTela(nomeArquivoIndice);
}

void criarArquivoDados(char *nomeArquivoEntrada, char *nomeArquivoDados, char *nomeArquivoIndice){
  //testando existência dos arquivos:
  //arquivo de dados será criado aqui
  char caminho[100] = "./";
  strcat(caminho, nomeArquivoDados);
  //cria e abre arquivo pra escrita
  FILE* arqDados = fopen(caminho, "wb");
  if(arqDados == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  } else{
    
  }
  //primeiro já vamos reescrever o status para 0
  char statusInconsistente = '0';
  fseek(arqDados, 0, SEEK_SET);
  fwrite(&statusInconsistente, sizeof(char), 1, arqDados);

  //agora testando se o arquivo de índice existe:
  char caminho_1[100] = "./";
  strcat(caminho_1, nomeArquivoIndice);
  //abre arquivo pra escrita permitindo fopen
  FILE* arqIndice = fopen(caminho_1, "rb+");
  if(arqIndice == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  } else{
    
  }
  //testando se csv existe
  char caminho_2[] = "./";
  strcat(caminho_2, nomeArquivoEntrada);
  FILE *arqEntrada = fopen(caminho_2, "r");
  //se der erro no processamento do arquivo, aparece a seguinte mensagem:
  if(arqEntrada == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  } else{
    
  }

  //chama uma função para criar uma estrutura de dados com todos os registros do arquivo de entrada, e cria também a estrutura de dados do indice. A função já faz as inserções no arquivo de dados e no arquivo de índice
  lerCSV(arqDados, arqIndice, arqEntrada);

  // //cria e abre arquivo pra escrita para verificar o byteoffset que está no cabeçalho
  // FILE* arqDadosLeitura = fopen(caminho, "rb");
  // if(arqDadosLeitura == NULL){
  //   puts("Falha no processamento do arquivo.");
  //   return;
  // } else{
    
  // }
  // int64_t proxByteoffset;
  // fseek(arqDadosLeitura, 1, SEEK_SET);
  // int quantidadePessoas = 0;
  // fread(&quantidadePessoas, sizeof(int), 1, arqDados);
  // int quantidadeRemovidos = 0;
  // fread(&quantidadeRemovidos, sizeof(int), 1, arqDados);
  // fread(&proxByteoffset, sizeof(int64_t), 1, arqDados);
  // printf("Proximo Byteoffset disponivel: %ld\n", proxByteoffset);
  // printf("Qtd pessoas: %d\n", quantidadePessoas);
  // printf("Qtd removidos: %d\n", quantidadeRemovidos);
  // fclose(arqDadosLeitura);

  //usa função binário na tela como especificado no trabalho
  binarioNaTela(nomeArquivoDados);
  binarioNaTela(nomeArquivoIndice);
}