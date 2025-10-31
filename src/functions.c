//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/functions.h"
#include "../headers/utilidades.h"



//FUNCIONALIDADE 1:

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



//FUNCIONALIDADE 2:

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



//FUNCIONALIDADE 3:

// Definição da variável global
struct registro_2 reg;

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



//FUNCIONALIDADE 4:

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
    indice2 *vetorIndice = malloc(qtdIndice * sizeof(indice2));
    
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



//FUNCIONALIDADE 6

void inserirUnicoRegistro(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n){
    //a inserção será feita no final do arquivo, mas precisamos atualizar também o cabeçalho, então o arquivo será aberto com rb+
    FILE *arqDados = fopen(nomeArquivoPessoa, "rb+");
    if(arqDados == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }
    //definindo status como inconsistente:
    char statusInconsistente = '0';
    fseek(arqDados, 0, SEEK_SET);
    fwrite(&statusInconsistente, 1, sizeof(char), arqDados);
    fseek(arqDados, 0, SEEK_SET);

    //é necessário ler o cabeçalho do arquivo para atualizar o mesmo depois 
    cabecalhoPessoa* cabecalho = lerCabecalho(arqDados);

    //abrindo arquivo de índice, que também deve ser atualizado:
    FILE *arqIndice = fopen(nomeArquivoIndice, "rb+");
    if(arqIndice == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }
    int numRegAtivos = cabecalho->quantidadePessoas;
    //definindo status como inconsistente:
    fseek(arqIndice, 0, SEEK_SET);
    fwrite(&statusInconsistente, 1, sizeof(char), arqIndice);
    fseek(arqIndice, 0, SEEK_SET);
    //lê o arquivo de índice para memória primária
    noIndice *indices = lerArquivoIndice(arqIndice, numRegAtivos, n);
    
    //devemos captar agora n entradas do usuário, e a cada entrada é feita uma inserção no arquivo de dados pessoa
    //um while é usado
    int i = 0;
    int nAtual = n;
    while(i < n){
        //é necessário ler o cabeçalho do arquivo para utilizar as informações contidas nele 
        cabecalhoPessoa* cabecalhoAtual = lerCabecalho(arqDados);
        //vamos captar agora mais uma entrada do usuário chamando uma função, que devolve um array nó com os dados a serem inseridos 
        noRegistroUnico *regUnico = lerEntradaInsercaoUnica();
        //depois da entrada ser captada, devemos fazer a inserção no arquivo de dados pessoa
        insereRegistroUnicoPessoa(arqDados, regUnico, cabecalhoAtual);
        //é necessário também inserir no vetor de índices o novo registro
        insereRegistroUnicoVetorIndice(indices, numRegAtivos + i, regUnico->idPessoa, cabecalhoAtual->proxByteoffset);
        //desalocando cabecalho atual para que ele seja lido de novo no começo do while
        free(cabecalhoAtual);
        free(regUnico->nomePessoa);
        free(regUnico->nomeUsuario);
        free(regUnico);
        i++;
    }
    //puts("\nsaiu do loop\n");
    //essa função armazena novamente o arquivo de índice, agora atualizado
    insereIndice(indices, arqIndice, cabecalho->quantidadePessoas + n);
    //depois de toda a funcionalidade ser executada, basta voltar o status consistente para os 2 arquivos e desalocar o que foi usado de memória, além de fechar os 2 arquivos
    free(cabecalho);
    free(indices);

    char statusConsistente = '1';

    fseek(arqDados, 0, SEEK_SET);
    fwrite(&statusConsistente, 1, sizeof(char), arqDados);
    fseek(arqDados, 0, SEEK_SET);
    
    fseek(arqIndice, 0, SEEK_SET);
    fwrite(&statusConsistente, 1, sizeof(char), arqIndice);
    fseek(arqIndice, 0, SEEK_SET);

    fclose(arqDados);
    fclose(arqIndice);
    
    binarioNaTela(nomeArquivoPessoa);
    binarioNaTela(nomeArquivoIndice);
}



// FUNCIONALIDADE 8:

void criaArquivoSegue(char *nomeArquivoEntradaSegue, char *nomeArquivoSaidaSegue){
    //para cada linha lida no arquivo csv, temos uma inserção no arquivo segue
    FILE *arqEntrada = fopen(nomeArquivoEntradaSegue, "r");
    if(arqEntrada == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }
    //abrindo arquivo segue para escrita+leitura, já que precisamos atualizar o cabeçalho a cada inserção:
    FILE *arqSegue = fopen(nomeArquivoSaidaSegue, "wb+");
    if(arqSegue == NULL){
        puts("Falha no processamento do arquivo.");
        fclose(arqEntrada);
        return;
    }

    char bufferLinha[1024];
    //a leitura no csv vai começar:
    fgets(bufferLinha, sizeof(bufferLinha), arqEntrada); //pula a primeira linha, que é: idPessoaQueSegue,idPessoaQueESeguida,dataInicioQueSegue,dataFimQueSegue,grauAmizade

    //definindo variáveis do cabeçalho:
    char status = '0';
    int quantidadeRegistros = 0;
    int proxRRN = 0;
    int proxByteOffset = 0;

    modificaCabecalhoArquivoSegue(arqSegue, status, quantidadeRegistros, proxByteOffset); //função que escreve cabeçalho do arquivo segue

    //pulando o cabeçalho
    proxByteOffset = 9;
    //proxRRN começa em 0:
    proxRRN = 0;

    //lê até a última linha do arquivo csv, o formato é: 186,117,03/05/2020,23/09/2020,0
    while(fgets(bufferLinha, sizeof(bufferLinha), arqEntrada) != NULL){
        noSegue *noAtual = malloc(sizeof(noSegue)); //criando um nó para o registro lido
        noAtual->dataInicioQueSegue = malloc(11 * sizeof(char)); //alocando memória para a string de data de inicio
        noAtual->dataFimQueSegue = malloc(11 * sizeof(char)); //alocando memória para a string de data de fim

        //removendo \n do fim da string
        bufferLinha[strcspn(bufferLinha, "\n")] = '\0';

        //criando ponteiro que aponta para o buffer que armazena a linha(necessário para strsep)
        char *str = bufferLinha;

        noAtual->removido[0] = '0';
        noAtual->removido[1] = '\0';
        char *idQueSegue = meu_strsep(&str, ",");
        char *idQueSeguida = meu_strsep(&str, ",");
        char *dataInicio = meu_strsep(&str, ",");
        char *dataFim = meu_strsep(&str, ",");
        char *grau = meu_strsep(&str, ",");

        noAtual->idPessoaQueSegue = atoi(idQueSegue);
        noAtual->idPessoaQueESeguida = atoi(idQueSeguida);

        if(dataInicio != NULL && dataInicio[0] != '\0'){
            strcpy(noAtual->dataInicioQueSegue, dataInicio);
        } else{
            strcpy(noAtual->dataInicioQueSegue, "$$$$$$$$$$");
        }

        if(dataFim != NULL && dataFim[0] != '\0'){
            strcpy(noAtual->dataFimQueSegue, dataFim);
        } else{
            strcpy(noAtual->dataFimQueSegue, "$$$$$$$$$$");
        }

        if(grau != NULL && grau[0] != '\0'){
            // Remove espaços e \n
            while(*grau && isspace((unsigned char)*grau)) grau++;
            noAtual->grauAmizade[0] = grau[0];
        } else{
            noAtual->grauAmizade[0] = '$';
        }
        noAtual->grauAmizade[1] = '\0';

        // printf("DEBUG - Registro %d:\n", quantidadeRegistros + 1);
        // printf("  removido: '%c'\n", noAtual->removido[0]);
        // printf("  idQueSegue: %d\n", noAtual->idPessoaQueSegue);
        // printf("  idQueSeguida: %d\n", noAtual->idPessoaQueESeguida);
        // printf("  dataInicio: '%.10s'\n", noAtual->dataInicioQueSegue);
        // printf("  dataFim: '%.10s'\n", noAtual->dataFimQueSegue);
        // printf("  grau: '%c'\n", noAtual->grauAmizade[0]);
        // printf("  proxByteOffset: %d\n\n", proxByteOffset);

        fseek(arqSegue, proxByteOffset, SEEK_SET); //posiciona o ponteiro no byteoffset disponível
        //chama a função de inserir no arquivo, passando o nó e o arquivo
        insereArquivoSegue(arqSegue, noAtual);

        //feitas as verificações e leituras:
        quantidadeRegistros++;
        proxRRN++;
        proxByteOffset += 30;

        //agora atualiza o cabeçalho:
        fseek(arqSegue, 1, SEEK_SET); //posiciona o ponteiro em qtdpessoas
        fwrite(&quantidadeRegistros, sizeof(int), 1, arqSegue);
        fwrite(&proxRRN, sizeof(int), 1, arqSegue);
        fflush(arqSegue);

        //a cada loop, desaloca a memória do nó e das strings
        free(noAtual->dataInicioQueSegue);
        free(noAtual->dataFimQueSegue);
        free(noAtual);
    }

    //quando acabar, atualiza status, fecha o arquivo e executa binário na tela:
    //printando variáveis do cabeçalho pra ver se deu certo:
    // printf("\n\nquantidade pessoas: %d\nproxRRN: %d\n", quantidadeRegistros, proxRRN);

    char statusConsistente = '1';
    fseek(arqSegue, 0, SEEK_SET);
    fwrite(&statusConsistente, sizeof(char), 1, arqSegue);

    //print para debug
    fseek(arqSegue, 0, SEEK_SET);
    char statusVerifica;
    int qtdVerifica;
    int proxByteOffsetVerifica;

    fread(&statusVerifica, sizeof(char), 1, arqSegue);
    fread(&qtdVerifica, sizeof(int), 1, arqSegue);
    fread(&proxByteOffsetVerifica, sizeof(int), 1, arqSegue);

    // printf("\n\n=== VERIFICAÇÃO DO CABEÇALHO ===\n");
    // printf("Status escrito: '1', lido: '%c' %s\n", 
    //     statusVerifica, 
    //     statusVerifica == '1' ? "OK" : "NOK");

    // printf("Qtd escrita: %d, lida: %d %s\n", 
    //     quantidadeRegistros, 
    //     qtdVerifica,
    //     qtdVerifica == quantidadeRegistros ? "OK" : "NOK");

    // printf("proxByteOffset escrito: %d, lido: %d %s\n", 
    //     proxByteOffset, 
    //     proxByteOffsetVerifica,
    //     proxByteOffsetVerifica == proxByteOffset ? "OK" : "NOK");

    fclose(arqSegue);

    binarioNaTela(nomeArquivoSaidaSegue);
}



//FUNÇÕES PARA FUNCIONALIDADE 9:

void ordenaArquivoSegue(char *nomeArquivoDesordenado, char *nomeArquivoOrdenado){
    //primeiro é necessário trazer o arquivo todo para memória primária
    //abre o arquivo desordenado:
    FILE *arqDesordenado = fopen(nomeArquivoDesordenado, "rb+");
    if(arqDesordenado == NULL){
        puts("Falha no processamento do arquivo.");
        free(arqDesordenado);
        return;
    }
    //escreve o status pra 0
    char statusInconsistente = '0';
    fseek(arqDesordenado, 0, SEEK_SET);
    fwrite(&statusInconsistente, sizeof(char), 1, arqDesordenado);

    //lê a quantidade de registros para alocar um vetor com o tamanho certo
    int qtdRegistros;
    fseek(arqDesordenado, 1, SEEK_SET);
    fread(&qtdRegistros, sizeof(int), 1, arqDesordenado);

    //debug ->printa se leu a quantidade de registros certa:
    // printf("Qtd registros lido: %d\n", qtdRegistros);
    
    //será criado um vetor da estrutura de dados do tipo noSegue
    noSegue *registros = (noSegue*)calloc(qtdRegistros, sizeof(noSegue));
    
    //o nó tem essa estrutura:
    // char removido[2];
    // int idPessoaQueSegue;
    // int idPessoaQueESeguida;
    // char *dataInicioQueSegue;
    // char *dataFimQueSegue;
    // char grauAmizade[2];

    //loop para ler todos os registros do arquivo segue

    fseek(arqDesordenado, 9, SEEK_SET); // coloca o byteoffset no primeiro registro
    for(int i = 0; i < qtdRegistros; i++){
        registros[i].dataInicioQueSegue = malloc(11 * sizeof(char));
        registros[i].dataFimQueSegue = malloc(11 * sizeof(char));

        fread(registros[i].removido, sizeof(char), 1, arqDesordenado);
        fread(&registros[i].idPessoaQueSegue, sizeof(int), 1, arqDesordenado);
        fread(&registros[i].idPessoaQueESeguida, sizeof(int), 1, arqDesordenado);
        fread(registros[i].dataInicioQueSegue, sizeof(char), 10, arqDesordenado);
        registros[i].dataInicioQueSegue[10] = '\0';
        fread(registros[i].dataFimQueSegue, sizeof(char), 10, arqDesordenado);
        registros[i].dataFimQueSegue[10] = '\0';
        fread(registros[i].grauAmizade, sizeof(char), 1, arqDesordenado);
    
        // //printa um nó para ver se deu certo:
        // printf("\nRemovido: '%c'", registros[i].removido[0]);
        // printf("\nId pessoa que segue: %d", registros[i].idPessoaQueSegue);
        // printf("\nId pessoa que e seguida: %d", registros[i].idPessoaQueESeguida);
        // printf("\nData inicio que segue: '%s'", registros[i].dataInicioQueSegue);
        // printf("\nData fim que segue: '%s'", registros[i].dataFimQueSegue);
        // printf("\nRemovido: '%c'", registros[i].grauAmizade[0]);
    }

    //depois de tudo lido, é preciso que a ordenação seja feita para que seja escrito tudo no arquivo de novo
    qsort(registros, qtdRegistros, sizeof(noSegue), comparaParaOrdenar);

    //abre arquivo segue ordenado para escrita:
    FILE *arqOrdenado = fopen(nomeArquivoOrdenado, "wb+");
    if(arqOrdenado == NULL){
        puts("Falha no processamento do arquivo.");
        free(arqOrdenado);
        return;
    }
    escreveSegueOrdenado(arqOrdenado, qtdRegistros, registros);//função que escreve tudo no arquivo de dados

    //atualiza status
    fseek(arqOrdenado, 0, SEEK_SET);
    char statusConsistente = '1';
    fwrite(&statusConsistente, sizeof(char), 1, arqOrdenado);

    //fecha arquivos e mostra binário na tela:
    fclose(arqDesordenado); //fecha arquivo desordenado
    fclose(arqOrdenado); //fecha arquivo ordenado
    binarioNaTela(nomeArquivoOrdenado);


    //desalocando memória usada para os nós
    for (int i = 0; i < qtdRegistros; i++)
    {
        free(registros[i].dataInicioQueSegue);
        free(registros[i].dataFimQueSegue);
    }
    free(registros);
}