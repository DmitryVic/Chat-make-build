#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>  // для работы с директориями
#include "user.h" 
#include <vector>
#include <utility>


namespace fs = std::filesystem;

/*=====================================
        ОБЩИЕ ФУНКЦИИ
=====================================*/

//Файл существует или нет
bool fileExists(const std::string& filename);

// Создает директорию, если её нет
void createDirectoryIfNeeded(const std::string& path);

// Создать файл
void createFile(const std::string& filename);

// Получить список всех файлов чатов в директории
std::vector<std::string> listChatFiles(const std::string& dir);

/*=====================================
        ФУНКЦИИ БД
=====================================*/

//запись в БД
void write_BD(const std::string& filename, std::shared_ptr<User> user);


/*=====================================
        ФУНКЦИИ ЧАТОВ
=====================================*/

//запись в приватный чат, проверить перед записью существоваие файла!
void write_Chat_P(const std::string& filename, std::shared_ptr<User> user, std::string mess);


// Загрузить историю приватного чата: пары <login, сообщение>
bool load_Chat_P(const std::string& filename,
std::vector<std::pair<std::string, std::string>>& out);
