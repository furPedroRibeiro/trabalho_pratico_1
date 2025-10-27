//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221
//aqui estão todas as funções que serão usadas para que as funcionalidades 1 a 10 sejam implementadas

#include "../headers/auxiliar.h"
#include "../headers/utilidades.h"

// Definição(atribuição) das variáveis globais (declaradas em auxiliar.h)
registro* raizListaPessoas = NULL;
indice* raizListaIndice = NULL;

// Variáveis globais internas (static = visíveis só neste arquivo)
static registro* ultimoElemento = NULL;
static char status = '0';
static int quantidadePessoas = 0;
static int quantidadeRemovidos = 0;
static int64_t proxByteoffset = 17;


//essa função lê o arquivo csv, faz a inserção no arquivo de dados a cada linha lida do csv, e vai montando a lista duplamente encadeada para o índice ser inserido depois no arquivo de índice
void lerCSV(FILE* arquivoDados, FILE* arquivoIndice, FILE* arquivoEntrada){
  //criando buffer para ler uma linha do arquivo
  char bufferLinha[1024];

  //fgets para ignorar a primeira linha do arquivo csv, que contém os nomes dos campos
  fgets(bufferLinha, sizeof(bufferLinha), arquivoEntrada);

  //função para definir o cabeçalho do arquivo de dados é chamada antes da leitura começar
  criaCabecalhoArquivoDados(arquivoDados, status, quantidadePessoas, quantidadeRemovidos, proxByteoffset);

  //aqui começa a leitura a partir da segunda linha do arquivo de dados
  while(fgets(bufferLinha, sizeof(bufferLinha), arquivoEntrada) != NULL){
    //declarando variável para obter o byteoffset do registro que será inserido nessa iteração
    int64_t byteoffsetAtual = proxByteoffset;
    //contador de tamanho de registro, tem no mínimo 21 bytes por registro por causa dos campos de tamanho fixo
    int tamRegistroBytes = 16;
    //removendo \n do fim da string
    bufferLinha[strcspn(bufferLinha, "\r\n")] = '\0';

    //criando ponteiro que aponta para o buffer que armazena a linha
    char *str = bufferLinha;

    //a partir daqui a leitura dos campos é feita usando a função meu_strsep, explicada e declarada em auxiliar.h
    char* campoIdPessoa = meu_strsep(&str, ",");  //a leitura de um campo é feita observando o conteúdo de str, ou seja, a string que está em buffer linha    
    char* campoNomePessoa = meu_strsep(&str, ",");
    char* campoIdadePessoa = meu_strsep(&str, ",");
    char* campoNomeUsuario = meu_strsep(&str, ",");

      
    //criando ponteiro para um novo registro
    registro* novoRegistro = calloc(1, sizeof(registro));
    //INSERÇÃO NA LISTA DE REGISTROS: chama função
    criarNoRegistro(novoRegistro, campoIdPessoa, campoIdadePessoa, campoNomePessoa, campoNomeUsuario, tamRegistroBytes);

    //agora é preciso fazer uma inserção no arquivo de dados, para cada leitura é necessário fazer uma inserção no arquivo de dados
    insereRegistro(novoRegistro, arquivoDados, quantidadeRemovidos, proxByteoffset);

    //depois que o registro é inserido, podemos adicionar um nó para o índice na lista de índice, que vai ser criada inteira antes de adicionarmos ela para o arquivo de índice

    //criando ponteiro para um novo registro de indice
    indice* novoRegistroIndice = calloc(1, sizeof(indice));
    //INSERÇÃO NA LISTA DE ÍNDICE: chama função
    criarNoRegistroIndice(novoRegistroIndice, campoIdPessoa, byteoffsetAtual);
  }
  //acabou a leitura do arquivo e a inserção de registros no arquivo de dados, agora é necessário fazer a inserção no arquivo de índice. A lista de índices já estará ordenada e com os byteoffsets certos, já que fizemos uma inserção a cada leitura
  //o arquivo csv pode ser fechado:
  fclose(arquivoEntrada);
  //com o cabeçalho todo reescrito e todos os registros cadastrados, basta atualizar o status novamente para 1, que é consistente
  char statusConsistente = '1';
  fseek(arquivoDados, 0, SEEK_SET);
  fwrite(&statusConsistente, sizeof(char), 1, arquivoDados);

  fclose(arquivoDados);

  insereRegistroIndice(raizListaIndice, arquivoIndice);

  //função acabou
}

//função que cria cabeçalho do arquivo de dados
void criaCabecalhoArquivoDados(FILE* arquivoDados, char status, int quantidadePessoas, int quantidadeRemovidos, int64_t proxByteoffsetAtual){
  //garante que o ponteiro está posicionado no começo do arquivo:
  fseek(arquivoDados, 0, SEEK_SET);
  //escreve cabeçalho
  //variável para contar se o tamanho do cabeçalho ta certo:
  int tamCabecalho = 0;
  fwrite(&status, sizeof(char), 1, arquivoDados);
  fwrite(&quantidadePessoas, sizeof(int), 1, arquivoDados);
  fwrite(&quantidadeRemovidos, sizeof(int), 1, arquivoDados);
  fwrite(&proxByteoffsetAtual, sizeof(int64_t), 1, arquivoDados);
}

//função para inserir um registro no arquivo de dados
void insereRegistro(registro* novoRegistro, FILE* arquivoDados, int quantidadeRemovidos, int64_t proxByteoffsetAtual){
  //a cada inserção, o cabeçalho é atualizado com a quantidade de pessoas, o número de pessoas removidas e o próximo byte offset disponível, além disso, o status é definido pra 1 no começo da inserção para indicar que o arquivo está inconsistente e depois da inserção ele é 0 para mostrar que está consistente

  //agora reposicionamos o ponteiro do arquivo para escrever o registro, aproveitando que proxByteOffset ainda não foi atualizado
  fseek(arquivoDados, proxByteoffsetAtual, SEEK_SET);

  //inserção:
  int tamanhoRegistro = novoRegistro->tamRegistro;
  int idPessoa = novoRegistro->idPessoa;
  int idadePessoa = novoRegistro->idadePessoa;
  int tamNomePessoa = novoRegistro->tamanhoNomePessoa;
  char *nomePessoa = novoRegistro->nome;
  int tamNomeUsuario = novoRegistro->tamanhoNomeUsuario;
  char *nomeUsuario = novoRegistro->nomeUsuario;

  fwrite(&novoRegistro->removido[0], sizeof(char), 1, arquivoDados);
  fwrite(&tamanhoRegistro, sizeof(int), 1, arquivoDados);
  fwrite(&idPessoa, sizeof(int), 1, arquivoDados);
  fwrite(&idadePessoa, sizeof(int), 1, arquivoDados);
  fwrite(&tamNomePessoa, sizeof(int), 1, arquivoDados);
  if(tamNomePessoa == 0){
    //não escreve nome da pessoa
  } else{
    fwrite(nomePessoa, sizeof(char), tamNomePessoa, arquivoDados);
  }
  fwrite(&tamNomeUsuario, sizeof(int), 1, arquivoDados);
  if(tamNomeUsuario == 0){
    //não escreve nome do usuário
  } else{
    fwrite(nomeUsuario, sizeof(char), tamNomeUsuario, arquivoDados);
  }

  //atualizando byteoffset de modo que o próximo byteoffset livre seja depois do registro que acabou de ser inserido
  proxByteoffset += (novoRegistro->tamRegistro)+5;
  //agora devemos atualizar o registro de cabeçalho:
  quantidadePessoas++;
  //movendo ponteiro do arquivo para o byte 1:
  fseek(arquivoDados, 1, SEEK_SET);
  //para atualizar a quantidade de pessoas, vamos escrever um inteiro
  fwrite(&quantidadePessoas, sizeof(int), 1, arquivoDados);
  fwrite(&quantidadeRemovidos, sizeof(int), 1, arquivoDados);
  fwrite(&proxByteoffset, sizeof(int64_t), 1, arquivoDados);
  
  if(novoRegistro->proxRegistro != NULL){
    fseek(arquivoDados, proxByteoffset, SEEK_SET);
  } else if(novoRegistro->proxRegistro == NULL){
    //não faz fseek para o prox byte offset disponível
  }
}

void insereRegistroIndice(indice* raizListaIndice, FILE* arquivoIndice){
  //mudando status no cabeçalho:
  char statusInconsistente = '0';
  fseek(arquivoIndice, 0, SEEK_SET);
  fwrite(&statusInconsistente, sizeof(char), 1, arquivoIndice);
  //volta ponteiro para escrever o primeiro registro de índice
  fseek(arquivoIndice, 12, SEEK_SET);
  //noAuxiliarIndice para receber a raíz da lista de registros de indice
  indice* noAuxiliarIndice = raizListaIndice;

  //loop para inserir todos os nós da lista no arquivo
  while(noAuxiliarIndice != NULL){
    //obtendo informações
    int idPessoa = noAuxiliarIndice->idPessoa;
    int64_t byteoffset = noAuxiliarIndice->byteOffset;

    //print pra ver se deu certo:
    // printf("\n\nId pessoa: %d byteoffset: %ld ", idPessoa, byteoffset);


    //escreve no arquivo de índice
    fwrite(&idPessoa, sizeof(int), 1, arquivoIndice);
    fwrite(&byteoffset, sizeof(int64_t), 1, arquivoIndice);

    //atualiza o nó atual
    noAuxiliarIndice = noAuxiliarIndice->proxIndice;
  }

  char statusConsistente = '1';
  fseek(arquivoIndice, 0, SEEK_SET);
  fwrite(&statusConsistente, sizeof(char), 1, arquivoIndice);

  //fechando arquivo de índice
  fclose(arquivoIndice);
}

//essa função cria o nó e adiciona esse nó novo a lista encadeada de registros do arquivo de dados
void criarNoRegistro(registro* novoRegistro, char *campoIdPessoa, char *campoIdadePessoa, char *campoNomePessoa, char *campoNomeUsuario, int tamRegistroBytes){
  //definindo id, não precisa de tratamento pois id não pode ser nulo
  novoRegistro->idPessoa = atoi(campoIdPessoa);
  
  //definindo idade da pessoa
  if(campoIdadePessoa == NULL || campoIdadePessoa[0] == '\0'){
    novoRegistro->idadePessoa = -1;
  } else{
    novoRegistro->idadePessoa = atoi(campoIdadePessoa);
  }

  //definindo nome da pessoa
  if(campoNomePessoa != NULL && campoNomePessoa[0] != '\0'){
    strcpy(novoRegistro->nome, campoNomePessoa);
    //para esse campo, é necessário também armazenar o tamanho do nome da pessoa
    novoRegistro->tamanhoNomePessoa = strlen(campoNomePessoa);
    //incrementa os bytes do nome para o contador do tamanho do registro
    tamRegistroBytes += strlen(campoNomePessoa);
  } else{
    strcpy(novoRegistro->nome, "");
    novoRegistro->tamanhoNomePessoa = 0;
  }

  //definindo nome de usuário da pessoa
  if(campoNomeUsuario != NULL && campoNomeUsuario[0] != '\0'){
    strcpy(novoRegistro->nomeUsuario, campoNomeUsuario);
    //para esse campo, é necessário também armazenar o tamanho do nome do usuario
    novoRegistro->tamanhoNomeUsuario = strlen(campoNomeUsuario);
    //incrementa os bytes do nome para o contador do tamanho do registro
    tamRegistroBytes += strlen(campoNomeUsuario);
  } else{
    strcpy(novoRegistro->nomeUsuario, "");
    novoRegistro->tamanhoNomeUsuario = 0;
  }

  //caso para a raiz da lista ser nula
  if(raizListaPessoas == NULL){
    novoRegistro->removido[0] = '0'; // 0 é não removido
    novoRegistro->tamRegistro = tamRegistroBytes;
    raizListaPessoas = novoRegistro;
    novoRegistro->proxRegistro = NULL;
    ultimoElemento = novoRegistro;

    return;
  }

  //definindo dados que não são lidos do csv
  novoRegistro->removido[0] = '0'; // 0 é não removido
  novoRegistro->tamRegistro = tamRegistroBytes;
  novoRegistro->proxRegistro = NULL; 
  ultimoElemento->proxRegistro = novoRegistro;
  ultimoElemento = novoRegistro; 
}

//essa função cria e adiciona um nó novo a lista duplamente encadeada de registros do arquivo de índice
void criarNoRegistroIndice(indice* novoRegistroIndice, char *campoIdPessoa, int64_t byteoffset){
  novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
  novoRegistroIndice->byteOffset = byteoffset;
  //a inserção será ordenada, então começamos com um ponteiro auxiliar que aponta para a raiz da lista
  indice* ponteiroAuxiliar = raizListaIndice;

  //se a lista for nula:
  if(raizListaIndice == NULL){
    novoRegistroIndice->antIndice = NULL;
    novoRegistroIndice->proxIndice = NULL;
    novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
    novoRegistroIndice->byteOffset = byteoffset;
    raizListaIndice = novoRegistroIndice;
    return;
  }

  //enquanto o ponteiro auxiliar não for o último, esse loop estará ativo
  while(ponteiroAuxiliar != NULL){
    //declarando e criando ponteiro para o nó anterior e para o próximo nó do ponteiro auxiliar, já que a lista de indice é uma lista duplamente encadeada
    indice *ponteiroAuxiliarProx = ponteiroAuxiliar->proxIndice;
    indice *ponteiroAuxiliarAnt = ponteiroAuxiliar->antIndice;

    //nesse primeiro condicional, se encontra o caso em que o ponteiro é a raíz da lista e o que queremos inserir é menor que a raíz
    if(ponteiroAuxiliarAnt == NULL){
      if(novoRegistroIndice->idPessoa < ponteiroAuxiliar->idPessoa){
        novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
        novoRegistroIndice->byteOffset = byteoffset;
        
        novoRegistroIndice->proxIndice = ponteiroAuxiliar;
        novoRegistroIndice->antIndice = NULL;
        ponteiroAuxiliar->antIndice = novoRegistroIndice;
        raizListaIndice = novoRegistroIndice;
        //a função acaba quando ocorre a inserção
        return;
      } else { 
      }
    }
    //no segundo condicional, se não for a raíz, verificamos se o nó que queremos inserir tem id menor que o nó atual procurado, se sim insere antes disso, se não vai pro próximo; além disso, se for menor, o nó se insere no meio de dois nós, de modo que os ponteiro se ajeitem
    else if(novoRegistroIndice->idPessoa < ponteiroAuxiliar->idPessoa){
      novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
      novoRegistroIndice->byteOffset = byteoffset;
      
      novoRegistroIndice->proxIndice = ponteiroAuxiliar;
      novoRegistroIndice->antIndice = ponteiroAuxiliarAnt;
      ponteiroAuxiliarAnt->proxIndice = novoRegistroIndice;    ponteiroAuxiliar->antIndice = novoRegistroIndice;
      //a função acaba quando ocorre inserção
      return;
    }
    //esse condicional verifica se é o último nó da lista
    else if(ponteiroAuxiliarProx == NULL){
      novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
      novoRegistroIndice->byteOffset = byteoffset;

      novoRegistroIndice->antIndice = ponteiroAuxiliar;
      novoRegistroIndice->proxIndice = NULL;
      ponteiroAuxiliar->proxIndice = novoRegistroIndice;
      //retorna quando ocorre a inserção
    }
    //se não for uma dessas opções, o ponteiro auxiliar passa pro próximo
    ponteiroAuxiliar = ponteiroAuxiliarProx;
  }
}

char* meu_strsep(char** buffer, const char* delim) {
    char* inicio = *buffer;
    char* p;
    
    if (inicio == NULL) {
        return NULL;
    }
    
    p = strpbrk(inicio, delim);
    
    if (p) {
        *p = '\0';
        *buffer = p + 1;
    } else {
        *buffer = NULL;
    }
    
    return inicio;
}






//funções para as funcionalidades 3 e 4

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
int64_t buscaBinariaIndice(indice2 *vetor, int tamanho, int idBuscado){
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



// FUNÇÕES PARA FUNCIONALIDADE 6:

cabecalhoPessoa* lerCabecalho(FILE *nomeArquivo){
  cabecalhoPessoa *cabecalho = calloc(1, sizeof(cabecalhoPessoa)); //aloca memória para o cabecalho
  
  fseek(nomeArquivo, 0, SEEK_SET); //posiciona para ler status
  fread(&cabecalho->status, sizeof(char), 1, nomeArquivo); //faz a leitura para status
  cabecalho->status[1] = '\0';
  fread(&cabecalho->quantidadePessoas, sizeof(int), 1, nomeArquivo);
  fread(&cabecalho->quantidadeRemovidos, sizeof(int), 1, nomeArquivo);
  fread(&cabecalho->proxByteoffset, sizeof(int64_t), 1, nomeArquivo);
  return cabecalho; //após ler o cabecalho, retorna para a função
}

noRegistroUnico* lerEntradaInsercaoUnica(){
  noRegistroUnico *regUnico = calloc(1, sizeof(noRegistroUnico));
  regUnico->nomePessoa = malloc(100 * sizeof(char));
  regUnico->nomeUsuario = malloc(100 * sizeof(char));

  //vamos ler uma linha de entrada, usando a mesma lógica usada na main
  //faz a leitura da entrada através de stdin
  int c = 0; 
  int i = 0;
  char bufferEntrada[1024];
  
  // lê até EOF, \0 ou \n — cobre todos os casos possíveis porque a entrada no runcodes é de um jeito e pelo teclado para testes é de outro, então ao invés de usar fgets fizemos a adaptação para todos os tipos de terminação de entrada
  while ((c = getchar()) != EOF && c != '\0' && c != '\n') {
      if (i < sizeof(bufferEntrada) - 1) {
        bufferEntrada[i++] = c;
      } else {
        //limpa o resto da entrada
        while ((c = getchar()) != EOF && c != '\n');
        break; // evita estouro de buffer(seg fault)
      }
  }
  bufferEntrada[i] = '\0'; //coloca o \0 no fim da entrada para usar strtok corretamente

  //lendo idPessoa, não pode ser nulo
  char *parametro;
  parametro = strtok(bufferEntrada, ",");
  regUnico->idPessoa = atoi(parametro);
  //lendo nome da pessoa
  parametro = strtok(NULL, ",");
  char *nomePessoa = removeEspacosEmBranco(parametro);
  if(strcmp(nomePessoa, "NULO") == 0){
    regUnico->nomePessoa = NULL;
  } else{
    regUnico->nomePessoa = removerAspas(nomePessoa);
  }
  //lendo idade
  parametro = strtok(NULL, ",");
  char *idadePessoa = removeEspacosEmBranco(parametro);
  if(strcmp(idadePessoa, "NULO") == 0){
    regUnico->idadePessoa = -1;
  } else{
    regUnico->idadePessoa = atoi(idadePessoa);
  }
  //lendo nome de usuário
  parametro = strtok(NULL, ",");
  char *nomeUsuario = removeEspacosEmBranco(parametro);
  regUnico->nomeUsuario = removerAspas(nomeUsuario);

  return regUnico;
}

void insereRegistroUnicoPessoa(FILE *nomeArquivoPessoa, noRegistroUnico* regUnico, cabecalhoPessoa *cabecalho){
  fseek(nomeArquivoPessoa ,cabecalho->proxByteoffset, SEEK_SET); //posiciona ponteiro no próximo byteoffset disponível segundo o cabecalho
  //definindo variáveis que não foram lidas
  char status = '0';
  int tamNomePessoa;
  int tamNomeUsuario = strlen(regUnico->nomeUsuario);
  if(regUnico->nomePessoa == NULL){
    tamNomePessoa = 0;
  } else{
    tamNomePessoa = strlen(regUnico->nomePessoa);
  }
  int tamRegistro = tamNomePessoa + tamNomeUsuario + 16;
  fwrite(&status, sizeof(char), 1, nomeArquivoPessoa);       
  fwrite(&tamRegistro, sizeof(int), 1, nomeArquivoPessoa);   
  fwrite(&regUnico->idPessoa, sizeof(int), 1, nomeArquivoPessoa);
  fwrite(&regUnico->idadePessoa, sizeof(int), 1, nomeArquivoPessoa);
  fwrite(&tamNomePessoa, sizeof(int), 1, nomeArquivoPessoa);
  if(tamNomePessoa > 0){
    fwrite(regUnico->nomePessoa, sizeof(char), tamNomePessoa, nomeArquivoPessoa);
  }
  fwrite(&tamNomeUsuario, sizeof(int), 1, nomeArquivoPessoa);
  fwrite(regUnico->nomeUsuario, sizeof(char), tamNomeUsuario, nomeArquivoPessoa);

  //depois que escreve tudo, precisamos atualizar o cabeçalho
  int quantidadePessoas = cabecalho->quantidadePessoas;
  quantidadePessoas++;
  int64_t proxByteoffset = cabecalho->proxByteoffset + tamRegistro + 5;
  //escreve no cabeçalho:
  fseek(nomeArquivoPessoa, 1, SEEK_SET);
  fwrite(&quantidadePessoas, 1, sizeof(int), nomeArquivoPessoa);
  fseek(nomeArquivoPessoa, 9, SEEK_SET);
  fwrite(&proxByteoffset, 1, sizeof(int64_t), nomeArquivoPessoa);
}

noIndice* lerArquivoIndice(FILE *nomeArquivoIndice, int n){
  //o arquivo já está aberto, e n é o número de registros ativos
  //o array para o vetor de indices é criada com 1 espaço a mais porque vamos inserir um novo registro a esse array posteriormente, então precisa ter esse espaço para que ocorra o deslocamento e a inserção correta do novo registroz
  noIndice *indices = malloc((n) * sizeof(noIndice));
  fseek(nomeArquivoIndice, 12, SEEK_SET); //posiciona ponteiro de leitura no primeiro registro
  for(int i = 0; i < n; i++){
    fread(&indices[i].idPessoa, 1, sizeof(int), nomeArquivoIndice);
    fread(&indices[i].byteoffset, 1, sizeof(int64_t), nomeArquivoIndice);
  }
  //depois da leitura, retorna o vetor de indices
  return indices;
}

void insereRegistroUnicoVetorIndice(noIndice* indices, int tamanhoVetor, int idPessoa, int64_t byteoffset){
  //o conceito de busca binária é utilizado para retornar uma posição válida em que o registro pode ser inserido
  int pos = buscaBinariaVetorIndice(indices, tamanhoVetor, idPessoa);
  
  for (int i = tamanhoVetor; i > pos; i--){
    indices[i] = indices[i-1];
  }
  indices[pos].idPessoa = idPessoa;
  indices[pos].byteoffset = byteoffset;
  //feito isso, o vetor está atualizado e 100% pronto para voltar para memória secundária
}

// Retorna a posição onde o idPessoa deve ser inserido para manter o vetor ordenado. Se o idPessoa já existir, retorna -1
int buscaBinariaVetorIndice(noIndice* indices, int tamanho, int idPessoa) {
    int inicio = 0;
    int fim = tamanho - 1;
    
    // Caso especial: vetor vazio
    if (tamanho == 0) {
        return 0;
    }
    
    // Caso especial: inserir no início
    if (idPessoa < indices[0].idPessoa) {
        return 0;
    }
    
    // Caso especial: inserir no final
    if (idPessoa > indices[fim].idPessoa) {
        return tamanho;
    }
    
    // Busca binária
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        
        if (indices[meio].idPessoa == idPessoa) {
            // ID já existe! Retorna -1 para indicar erro
            return -1;
        }
        else if (indices[meio].idPessoa < idPessoa) {
            inicio = meio + 1;
        }
        else {
            fim = meio - 1;
        }
    }
    
    // A posição de inserção é retornada
    return inicio;
}

void insereIndice(noIndice* indices, FILE *nomeArquivoIndice, int tamanho){
  //o indice será todo reescrito em memória secundária, como aqui estamos escrevendo um registro a mais, o tamanho do arquivo anterior pouco nos importa
  //o status já está definido como inconsistente, agora só nos resta pular o cabeçalho e reescrever tudo
  fseek(nomeArquivoIndice, 12, SEEK_SET);
  for(int i = 0; i < tamanho; i++){
    fwrite(&indices[i].idPessoa, sizeof(int), 1, nomeArquivoIndice);
    fwrite(&indices[i].byteoffset, sizeof(int64_t), 1, nomeArquivoIndice);
  }
  //depois de escrever tudo:
  return;
}






char *removeEspacosEmBranco(char *campo){
  // Remove espaços do início
  while (*campo && isspace((unsigned char)*campo)) {
    campo++;
  }
  // Remove espaços do final
  int len = strlen(campo);
  while (len > 0 && isspace((unsigned char)campo[len - 1])) {
    campo[--len] = '\0';
  }
  return campo;
}

char *removerAspas(char *campo){
  if(campo == NULL || strlen(campo) < 2) return campo;
  
  int len = strlen(campo); //lê tamanho da string
  // Se começa com aspas, pula
  if (campo[0] == '"') {
    campo++;
    len--;
  }
  
  // Se termina com aspas, remove
  if (len > 0 && campo[len-1] == '"') {
    campo[len-1] = '\0';
  }
  
  return campo;
}