PROJ_NAME=ted
ALUNO=LarissaMSFonseca
LIBS=-lm
OBJETOS=main.o lista.o radialtree.o retangulo.o circulo.o arq_svg.o linha.o texto.o fila.o forma.o quick_sort.o
# compilador
CC=gcc
# Flags

CFLAGS= -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration

LDFLAGS=-O0

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME)  $(LDFLAGS) $(OBJETOS) $(LIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

#
# COLOCAR DEPENDENCIAS DE CADA MODULO
#
# Exemplo: suponha que o arquivo a.c possua os seguintes includes:
#
# #include "a.h"
# #include "b.h"
# #include "c.h"
#
# a.o: a.h b.h c.h a.c
#

main.o: lista.h radialtree.h retangulo.h circulo.h arq_svg.h linha.h texto.h fila.h forma.h main.c
lista.o: lista.h lista.c
radialtree.o: radialtree.h quick_sort.h radialtree.c
retangulo.o: retangulo.h retangulo.c
circulo.o: circulo.h circulo.c
arq_svg.o: arq_svg.h lista.h retangulo.h circulo.h linha.h arq_svg.c 
linha.o: linha.h linha.c
texto.o: texto.h texto.c
fila.o: fila.h fila.c
forma.o: forma.h arq_svg.h forma.c
quick_sort.o: quick_sort.h quick_sort.c 

# EMPACOTAR PARA ENTREGA
#
# - se necessario: sudo apt install zip



pack: $(PROJ_NAME)
	rm -f ../$(ALUNO).zip
	echo $(ALUNO)
	date >> .entrega

	cd ..; zip $(ALUNO).zip -r src/*.c src/*.h src/Makefile LEIA-
	ME.txt .entrega
