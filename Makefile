# Компилятор
CXX := g++

# Флаги компиляции
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

# Определение ОС и настройки
ifeq ($(OS),Windows_NT)
    # Windows settings
    CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8
    TARGET := chat.exe
    MKDIR := mkdir
    RM := del /Q /F
    RMDIR := rmdir /S /Q
    CHECK_DIR = if not exist "$(1)" $(MKDIR) "$(1)"
    CHECK_FILE = if exist "$(1)"
else
    # Linux settings
    CXXFLAGS += -DSET_GLOBAL_LOCALE_LINUX
    TARGET := chat
    MKDIR := mkdir -p
    RM := rm -f
    RMDIR := rm -rf
    CHECK_DIR = test -d "$(1)" || $(MKDIR) "$(1)"
    CHECK_FILE = test -f "$(1)"
endif

# Все исходники
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp, obj/%.o, $(SRCS))

# Основное правило
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Компиляция .cpp в .o
obj/%.o: src/%.cpp
	@$(call CHECK_DIR,$(@D))
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
ifeq ($(OS),Windows_NT)
	- del /Q /F "$(TARGET)"
	- rmdir /S /Q obj
else
	- rm -f "$(TARGET)"
	- rm -rf obj
endif

.PHONY: all clean