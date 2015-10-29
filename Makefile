all: src/ExpressionParser.cpp src/Parser.h
	g++ src/ExpressionParser.cpp -o Parser

clean:
	rm *.exe
