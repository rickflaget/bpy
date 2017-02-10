dpl: recognizer.o grammartypes.o evaluate.o ENV.o lexer.o
	g++ -Wall -g -std=c++11 recognizer.o evaluate.o lexer.o ENV.o grammartypes.o -o dpl.out
recognizer.o: recognizer.cpp recognizer.h grammartypes.h ENV.h evaluate.h lexer.h
	g++ -Wall -g -c -std=c++11 recognizer.cpp
lexer.o: lexer.cpp lexer.h grammartypes.h 
	g++ -Wall -g -c -std=c++11 lexer.cpp
grammartypes.o: grammartypes.cpp grammartypes.h
	g++ -Wall -g -c -std=c++11 grammartypes.cpp
ENV.o: ENV.cpp ENV.h grammartypes.h 
	g++ -Wall -g -c -std=c++11 ENV.cpp
evaluate.o: evaluate.cpp evaluate.h grammartypes.h  lexer.h ENV.h
	g++ -Wall -g -c -std=c++11 evaluate.cpp 
clean:
	rm -f *.o *.exe *.exe.stackdump *.out 
error1:
	cat ./testfiles/error1.bpy
error1x:
	./dpl ./testfiles/error1.bpy
error2:
	cat ./testfiles/error2.bpy
error2x:
	./dpl ./testfiles/error2.bpy
error3:
	cat ./testfiles/error3.bpy
error3x:
	./dpl ./testfiles/error3.bpy
arrays:
	cat ./testfiles/arrays.bpy	
arraysx:
	./dpl ./testfiles/arrays.bpy
conditionals:
	cat ./testfiles/conditionals.bpy
conditionalsx:
	./dpl ./testfiles/conditionals.bpy
recursion:
	cat ./testfiles/recursion.bpy
recursionx:
	./dpl ./testfiles/recursion.bpy
iteration:
	cat ./testfiles/iteration.bpy	
iterationx:
	./dpl ./testfiles/iteration.bpy
functions:
	cat ./testfiles/functions.bpy
functionsx:
	./dpl ./testfiles/functions.bpy
dictionary:
	echo dictionary implementation found in ./dict.bpy
	cat ./testfiles/dictionary.bpy
dictionaryx:
	./dpl ./testfiles/dictionary.bpy ./dict.bpy
problem:
	cat ./testfiles/rpn.bpy
	cat ./testfiles/testInputFile	
problemx:
	cat ./testfiles/testInputFile | ./dpl ./testfiles/rpn.bpy
