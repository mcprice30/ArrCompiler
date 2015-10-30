all: src/ExpressionParser.cpp src/Parser.h src/Arr.cpp
	g++ src/Arr.cpp src/ExpressionParser.cpp -o arr

tests: src/ExpressionParser.cpp src/Parser.h src/Tester.h src/Tester.cpp src/Test.cpp
	g++ src/Test.cpp src/ExpressionParser.cpp src/Tester.cpp -o tst

clean:
	rm *.exe
