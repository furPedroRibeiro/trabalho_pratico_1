CC = gcc

CFLAGS = -lm

TARGET = trabPratico
MAIN = main
CRIAR_ARQUIVOS = criarArquivos
LEITURA_BUSCA_ARQUIVOS = leituraBuscaArquivos
UTILIDADES = utilidades

AUXILIAR_CRIAR_ARQUIVOS = auxiliarCriarArquivos
AUXILIAR_LEITURA_BUSCA = auxiliarLeituraBusca

all: $(TARGET)

$(TARGET): ./objects/$(MAIN).o ./objects/$(CRIAR_ARQUIVOS).o ./objects/$(AUXILIAR_CRIAR_ARQUIVOS).o ./objects/$(UTILIDADES).o ./objects/$(LEITURA_BUSCA_ARQUIVOS).o ./objects/$(AUXILIAR_LEITURA_BUSCA).o
	$(CC) ./objects/$(MAIN).o ./objects/$(CRIAR_ARQUIVOS).o ./objects/$(AUXILIAR_CRIAR_ARQUIVOS).o ./objects/$(UTILIDADES).o ./objects/$(LEITURA_BUSCA_ARQUIVOS).o ./objects/$(AUXILIAR_LEITURA_BUSCA).o -o $(TARGET) $(CFLAGS)

./objects/$(MAIN).o: ./src/$(MAIN).c ./headers/$(CRIAR_ARQUIVOS).h ./headers/$(LEITURA_BUSCA_ARQUIVOS).h
	$(CC) -c ./src/$(MAIN).c -o ./objects/$(MAIN).o

./objects/$(CRIAR_ARQUIVOS).o: ./src/$(CRIAR_ARQUIVOS).c ./headers/$(CRIAR_ARQUIVOS).h ./auxiliares/headers/$(AUXILIAR_CRIAR_ARQUIVOS).h ./headers/$(UTILIDADES).h
	$(CC) -c ./src/$(CRIAR_ARQUIVOS).c -o ./objects/$(CRIAR_ARQUIVOS).o

./objects/$(AUXILIAR_CRIAR_ARQUIVOS).o: ./auxiliares/src/$(AUXILIAR_CRIAR_ARQUIVOS).c ./auxiliares/headers/$(AUXILIAR_CRIAR_ARQUIVOS).h 
	$(CC) -c ./auxiliares/src/$(AUXILIAR_CRIAR_ARQUIVOS).c -o ./objects/$(AUXILIAR_CRIAR_ARQUIVOS).o

./objects/$(UTILIDADES).o: ./src/$(UTILIDADES).c ./headers/$(UTILIDADES).h
	$(CC) -c ./src/$(UTILIDADES).c -o ./objects/$(UTILIDADES).o

./objects/$(LEITURA_BUSCA_ARQUIVOS).o: ./src/$(LEITURA_BUSCA_ARQUIVOS).c ./headers/$(LEITURA_BUSCA_ARQUIVOS).h ./auxiliares/headers/$(AUXILIAR_LEITURA_BUSCA).h
	$(CC) -c ./src/$(LEITURA_BUSCA_ARQUIVOS).c -o ./objects/$(LEITURA_BUSCA_ARQUIVOS).o

./objects/$(AUXILIAR_LEITURA_BUSCA).o: ./auxiliares/src/$(AUXILIAR_LEITURA_BUSCA).c ./auxiliares/headers/$(AUXILIAR_LEITURA_BUSCA).h
	$(CC) -c ./auxiliares/src/$(AUXILIAR_LEITURA_BUSCA).c -o ./objects/$(AUXILIAR_LEITURA_BUSCA).o

clean:
	rm -f ./objects/*.o $(TARGET)

delete:
	rm -f ./*.bin

delete_zip:
	rm -f ./*.zip

run: $(TARGET)
	./$(TARGET)