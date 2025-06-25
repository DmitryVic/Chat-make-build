# Компилятор
CXX := g++

# Флаги компиляции
# - std=c++17: используем стандарт C++17
# - Wall: включаем все основные предупреждения
# - Wextra: дополнительные предупреждения
# - Iinclude: добавляем папку include в пути поиска заголовочных файлов
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

# Определение ОС и настройки
# Проверяем, работаем ли мы в Windows
ifeq ($(OS),Windows_NT)
    # - Добавляем флаги для корректной работы с UTF-8
    CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8
    # Имя исполняемого файла с расширением .exe
    TARGET := chat.exe
    # Команда создания папки
    MKDIR := mkdir
    # Команда удаления файлов в Windows
    RM := del /Q /F
    # Команда удаления папок в Windows
    RMDIR := rmdir /S /Q
    # Windows-специфичная проверка существования директории
    CHECK_DIR_CMD = if not exist "$(1)" $(MKDIR) "$(1)"
    CHECK_FILE_CMD = if exist "$(1)"
else
    # Linux
    # Linux локализация
    CXXFLAGS += -DSET_GLOBAL_LOCALE_LINUX
    # Имя исполняемого файла
    TARGET := chat
    # Команда создания папки с родительскими директориями
    MKDIR := mkdir -p
    # Удаление файлов
    RM := rm -f
    # Рекурсивное удаление
    RMDIR := rm -rf
    # Linux-специфичная проверка существования директории
    CHECK_DIR_CMD = test -d "$(1)" || $(MKDIR) "$(1)"
    CHECK_FILE_CMD = test -f "$(1)"
endif

# Все исходники
# Находим все .cpp файлы в папке src
SRCS := $(wildcard src/*.cpp)
# Преобразуем имена исходников в имена объектных файлов (заменяем src/ на obj/)
OBJS := $(patsubst src/%.cpp, obj/%.o, $(SRCS))

# Основное правило
all: $(TARGET)

# Сборка исполняемого файла из объектных файлов
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Компиляция .cpp в .o
obj/%.o: src/%.cpp
	@$(call CHECK_DIR_CMD,$(@D))
	# Компилируем исходник в объектный файл
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка (кроссплатформенная)
clean:
	# Удаляем исполняемый файл, если он существует
	@$(call CHECK_FILE_CMD,$(TARGET)) && $(RM) "$(TARGET)" || true
	# Удаляем папку с объектными файлами, если она существует
	@$(call CHECK_DIR_CMD,obj) && $(RMDIR) "obj" || true

.PHONY: all clean