CPP=g++
ARCHIVER=ar rvs 
CPP_FLAGS=-Wall -std=c++20 -I~/.local/include/ -g 

TARGET=target/scluk.a
OBJ_FILES=target/math.o target/range.o target/logging.o target/exception.o

$(TARGET): $(OBJ_FILES)
	$(ARCHIVER) $(TARGET) $(OBJ_FILES)

target/%.o: definition/%.cpp
	$(CPP) $(CPP_FLAGS) -c $< -o $@

validate: $(TARGET)
	@echo generating tmp.cpp for validating headers and obj archive
	@echo "int main() {}" > tmp.cpp
	@bash -c 'for f in $(wildcard *.hpp); do echo "#include \"$$f\"" >> tmp.cpp; done'
	$(CPP) $(CPP_FLAGS) tmp.cpp $(TARGET) -o tmp.out
	rm tmp.cpp tmp.out

clean:
	rm target/* 
