PROJECT = test
CC = g++
DEFS =
INCDIR =
INCS =
LIBDIR =
LIBS = -lSmrFramework
DEFS = -pthread -std=c++11 -Wall
HEADERS = header.h
OBJS = \
	main.o \
	stats.o \
	testArray.o \
	testBase64.o \
	testCallback.o \
	testConvert.o \
	testDateTime.o \
	testDictionary.o \
	testDirectory.o \
	testFile.o \
	testFileInfo.o \
	testForth.o \
	testForthContext.o \
	testFraction.o \
	testHash.o \
	testHexGrid.o \
	testLexer.o \
	testLine.o \
	testList.o \
	testMath.o \
	testObject.o \
	testParser.o \
	testQueue.o \
	testRandom.o \
	testRegEx.o \
	testSet.o \
	testStopWatch.o \
	testStack.o \
	testStreamReader.o \
	testString.o \
	testSystem.o \
	testTests.o \
	testTimeSpan.o \
	testUtils.o \
	testVector.o

$(PROJECT): $(OBJS)
	$(CC) $(DEFS) $(LIBDIR) $(OBJS) $(LIBS) -o $(PROJECT)

.c.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

.cpp.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

clean:
	-rm $(PROJECT)
	-rm *.o

main.o:             $(HEADERS) main.cpp
stats.o:            $(HEADERS) stats.cpp
testArray.o:        $(HEADERS) testArray.cpp
testCallback.o:     $(HEADERS) testCallback.cpp
testConvert.o:      $(HEADERS) testConvert.cpp
testDictionary.o:   $(HEADERS) testDictionary.cpp
testDateTime.o:     $(HEADERS) testDateTime.cpp
testDicrectory.o:   $(HEADERS) testDicrectory.cpp
testFile.o:         $(HEADERS) testFile.cpp
testFileInfo.o:     $(HEADERS) testFileInfo.cpp
testForth.o:        $(HEADERS) testForth.cpp
testForthContext.o: $(HEADERS) testForthContext.cpp
testFraction.o:     $(HEADERS) testFraction.cpp
testHash.o:         $(HEADERS) testHash.cpp
testHexGrid.o:      $(HEADERS) testHexGrid.cpp
testLexer.o:        $(HEADERS) testLexer.cpp
testLine.o:         $(HEADERS) testLine.cpp
testList.o:         $(HEADERS) testList.cpp
testMath.o:         $(HEADERS) testMath.cpp
testParser.o:       $(HEADERS) testParser.cpp
testObject.o:       $(HEADERS) testObject.cpp
testQueue.o:        $(HEADERS) testQueue.cpp
testRandom.o:       $(HEADERS) testRandom.cpp
testRegEx.o:        $(HEADERS) testRegEx.cpp
testSet.o:          $(HEADERS) testSet.cpp
testStopWatch.o:    $(HEADERS) testStopWatch.cpp
testStack.o:        $(HEADERS) testStack.cpp
testStreamReader.o: $(HEADERS) testStreamReader.cpp
testString.o:       $(HEADERS) testString.cpp
testSystem.o:       $(HEADERS) testSystem.cpp
testTests.o:        $(HEADERS) testTests.cpp
testTimeSpan.o:     $(HEADERS) testTimeSpan.cpp
testUtils.o:        $(HEADERS) testUtils.cpp
testVector.o:       $(HEADERS) testVector.cpp
