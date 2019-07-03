CC = gcc
FILES  = xpm_main.c compress.c decompress.c xpm.h
OUT_EXE = xpm_compresser

xpm_compresser: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES)
	
clean: 
	rm -f *.o core	

rebuild: clean xpm_compresser
