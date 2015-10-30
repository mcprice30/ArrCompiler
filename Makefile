all: src/ExpressionParser.cpp src/Parser.h src/Arr.cpp src/ParseUtil.cpp src/ParseUtil.h
	g++ src/Arr.cpp src/ExpressionParser.cpp src/ParseUtil.cpp -o arr

tests: src/ExpressionParser.cpp src/Parser.h src/Tester.h src/Tester.cpp src/Test.cpp src/ParseUtil.cpp src/ParseUtil.h
	g++ src/Test.cpp src/ExpressionParser.cpp src/Tester.cpp src/ParseUtil.cpp -o tst

clean:
	rm *.exe
