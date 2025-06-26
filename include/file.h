#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>  // для работы с директориями
#include "user.h" 


namespace fs = std::filesystem;

/*=====================================
        ОБЩИЕ ФУНКЦИИ
=====================================*/

//Файл существует или нет
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Создает директорию, если её нет
void createDirectoryIfNeeded(const std::string& path) {
    auto dir_path = fs::path(path).parent_path();
    if (!dir_path.empty() && !fs::exists(dir_path)) {
        fs::create_directories(dir_path);
        std::cout << "Создана директория: " << dir_path << std::endl;
    }
}

// Создать файл
void createFile(const std::string& filename) {
    createDirectoryIfNeeded(filename);  // сначала создаем папку
    std::ofstream file(filename);
    if (file.is_open()) {
        std::cout << "Файл " << filename << " создан успешно." << std::endl;
        file.close();
    } else {
        std::cout << "Не удалось создать файл " << filename << std::endl;
    }
}


/*=====================================
        ФУНКЦИИ БД
=====================================*/

void write_BD(const std::string& filename, std::shared_ptr<User> user) {
    std::ofstream fs(filename, std::ios::app);  // app = append (добавление в конец)
    if (fs.is_open()) {
        fs << user->getLogin() << '\n' 
           << user->getPass() << '\n' 
           << user->getName() << "\n\n";        // Два \n для разделения записей
        fs.close();
    } else {
        std::cerr << "Ошибка записи в файл: " << filename << std::endl;
    }
}



/*=====================================
        ФУНКЦИИ ЧАТОВ
=====================================*/