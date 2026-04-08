binaryconversion: binaryConverterMain.o binaryConverter.o binaryConverter.h #Linking 
	gcc binaryConverterMain.o binaryConverter.o -o binaryconversion 
%.o:%.c binaryConverter.h #Compiling to object file
	gcc -Wall -c $< -o $@ 