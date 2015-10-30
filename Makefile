all: src/ExpressionParser.cpp src/ExpParser.h src/Arr.cpp src/ParseUtil.cpp src/ParseUtil.h src/StmtParser.cpp src/StmtParser.h
	g++ src/Arr.cpp src/ExpressionParser.cpp src/ParseUtil.cpp src/StmtParser.cpp -o arr

tests: src/ExpressionParser.cpp src/ExpParser.h src/Tester.h src/Tester.cpp src/Test.cpp src/ParseUtil.cpp src/ParseUtil.h src/StmtParser.cpp src/StmtParser.h
	g++ src/Test.cpp src/ExpressionParser.cpp src/Tester.cpp src/ParseUtil.cpp src/StmtParser.cpp -o tst

clean:
	rm *.exe
