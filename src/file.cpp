#include "file.h" 
#include "user.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>  // для работы с директориями


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

// Список всех .txt-файлов в директории dir
std::vector<std::string> listChatFiles(const std::string& dir) {
    std::vector<std::string> out;
    if (!fs::exists(dir) || !fs::is_directory(dir)) return out;
    for (auto& p : fs::directory_iterator(dir)) {
        if (p.is_regular_file() && p.path().extension() == ".txt") {
            out.push_back(p.path().string());
        }
    }
    return out;
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
        std::cerr << "Ошибка записи в файл БД: " << filename << std::endl;
    }
}



/*=====================================
        ФУНКЦИИ ЧАТОВ
=====================================*/


void write_Chat_P(const std::string& filename, std::shared_ptr<User> user, std::string mess){
    
    if(!fileExists(filename)) // выход еслифайла нет
        return;

    std::ofstream fs(filename, std::ios::app);  // app = append (добавление в конец)
    if (fs.is_open()) {
        fs << user->getLogin() << '\n' 
           << mess << "\n\n";        // Два \n для разделения записей
        fs.close();
    } else {
        std::cerr << "Ошибка записи в файл чата: " << filename << std::endl;
    }
}


// Загрузка истории приватного чата
 bool load_Chat_P(const std::string& filename,
                  std::vector<std::pair<std::string, std::string>>& out)
 {
     if (!fileExists(filename)) return false;
     std::ifstream fs(filename);
     if (!fs.is_open()) return false;

     std::string login, mess;
     while (std::getline(fs, login) && std::getline(fs, mess)) {
         out.emplace_back(login, mess);
         std::string sep;
         std::getline(fs, sep);  // пропускаем пустую строку
     }
     return true;
 }