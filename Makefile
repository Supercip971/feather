DIR_GUARD=mkdir -p $(@D)
BUILD_PATH := ./build
PROJECT_DIR := ./src ./flang

CFILES := $(shell find $(PROJECT_DIR) -type f -name *.cpp)
HFILES := $(shell find $(PROJECT_DIR) -type f -name *.h)

OBJFILES := $(patsubst %.cpp, $(BUILD_PATH)/%.o, $(CFILES))
OUTPUT := $(BUILD_PATH)/flang-lang

SANITIZER := \
	-fsanitize=address \
	-fsanitize=undefined 

GCC_FLAGS := \
	-g \
	-O2 \
  	-pthread \
	-Isrc \
	-Iflang \
	-msse \
	-std=c++20 \
	-m64 \
	-Werror

$(BUILD_PATH)/%.o: %.cpp $(CFILES) $(HFILES)
	@$(DIR_GUARD)
	@echo " [ CXX ] $<"
	@clang++ -c $(GCC_FLAGS) $(SANITIZER) -o $@ $<

$(OUTPUT): $(OBJFILES)
	@$(DIR_GUARD)
	@echo " [ flang ] $(OUTPUT) "
	@clang++ $(OBJFILES) $(SANITIZER) -pthread -o $(OUTPUT)


clean: 
	rm $(OBJFILES)
	rm $(OUTPUT)


all: $(OUTPUT)

run: $(OUTPUT)
	$(OUTPUT)

format: 
	clang-format -i --style=file $(CFILES) $(HFILES)
