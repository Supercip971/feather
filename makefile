
CFILES := $(shell find src -type f -name \*.cpp)
HFILES := $(shell find src -type f -name \*.h)
OBJFILES := $(patsubst %.cpp, %.o, $(CFILES))
OUTPUT := "./build/feather"
%.o: %.cpp $(CFILES) $(HFILES)
	@echo "[ C++] building $< "
	@clang++ -c -pthread -fpermissive -fsanitize=address -fsanitize=undefined -std=c++20 -m64 -Wall -g -march=x86-64 -I src/ -O3 -msse -o $@ $<

$(OUTPUT): $(OBJFILES)
	@mkdir -p ./build/
	@echo "all"
	@clang++ $(OBJFILES) -fpermissive -fsanitize=address -fsanitize=undefined -pthread -o build/feather



run: 
	./build/feather
run_test:
	./build/feather $(TST_FILES)
	cat output

format: 
	clang-format -i --style=file $(CFILES) $(HFILES)


clean:
	rm $(OBJFILES)
	rm ./build/feather


all: $(OUTPUT) 
	@make run




