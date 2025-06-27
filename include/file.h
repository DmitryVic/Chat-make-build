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
bool fileExists(const std::string& filename);

// Создает директорию, если её нет
void createDirectoryIfNeeded(const std::string& path);

// Создать файл
void createFile(const std::string& filename);


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