CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
TARGET = bin/credit_risk
OBJECTS = build/main.o build/Customer.o build/Scorer.o build/CSVReader.o
.PHONY: all clean
all: $(TARGET)
$(TARGET): $(OBJECTS)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)
build/main.o: src/main.cpp src/Customer.h src/Scorer.h src/CSVReader.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o
build/Customer.o: src/Customer.cpp src/Customer.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/Customer.cpp -o build/Customer.o
build/Scorer.o: src/Scorer.cpp src/Scorer.h src/Customer.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/Scorer.cpp -o build/Scorer.o
build/CSVReader.o: src/CSVReader.cpp src/CSVReader.h src/Customer.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/CSVReader.cpp -o build/CSVReader.o
clean:
	rm -rf build bin reports
