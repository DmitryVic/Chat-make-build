#include "database.h"
#include "colorConsole.h"
#include "user.h" 
#include "exept.h"
#include "file.h" // функции для работы с файлами
#include <iostream>
#include <set>
#include <fstream>
#include <string>
#include "chatPrivate.h"


static std::string BD_FILE = "file/DB.txt";


//Проверяет, что файл БД существуюет, если нет, то содает
Database::Database()
{
    //Проверка существования файла БД

    if (!fileExists(BD_FILE)) {
        std::cout << _CYAN << "Создание БД: " << BD_FILE <<  _CLEAR << std::endl;
        createFile(BD_FILE);
    } else {
        std::cout << _CYAN << "Чтение БД: " << BD_FILE <<  _CLEAR << std::endl;
        loadUsersFromFile();  // Загружаем пользователей из файла
        // Восстанавливаем все приватные чаты из файлов
        loadChats();
    }

    std::cout << _BLUE <<  "База данных запущена!" <<  _CLEAR << std::endl;
}

Database::~Database()
{
    
}

// Загрузка пользователей из файла
void Database::loadUsersFromFile() {
    std::ifstream file(BD_FILE);
    if (!file.is_open()) {
        std::cerr << _RED << "Ошибка открытия файла БД для чтения!" << _CLEAR << std::endl;
        return;
    }

    std::string login, password, name;
    while (std::getline(file, login) && 
           std::getline(file, password) && 
           std::getline(file, name)) {
        // Пропускаем пустые строки
        // Если любое из полей пустое - пропускаем эту итерацию цикла Защита от некорр данных
        if (login.empty() || password.empty() || name.empty()) {
            continue;
        }
        
        // Создаем пользователя и добавляем в множество
        auto user = std::make_shared<User>(login, password, name);
        usersInChat.insert(user);
        
        // Пропускаем пустую строку-разделитель
        std::string empty_line;
        std::getline(file, empty_line);
    }
    file.close();
}

// Добавить пользовател - принимает и временные, и существующие объекты
void Database::setUser(std::shared_ptr<User> user){
    this->usersInChat.insert(user);
    write_BD(BD_FILE, user);

}

// Получить список всех пользователей
std::set<std::shared_ptr<User>> Database::getAllUsersInChat() const{
    return this->usersInChat;
}

// Получить указатель на пользователя по логину (уникален для каждого)
std::shared_ptr<User> Database::getOneUserByLogin(const std::string &login) const{
    //если список пользовватель пуст - nullptr
    if (this->usersInChat.empty())
    {
        return nullptr;
    }

    // Проверка, есть ли пользователь с таким логином, если да, то возвращаем указатель на него
    for (const auto& user : usersInChat) {
        if (user->getLogin() == login) {
            return user;
        }
    }

    // не нашли пользователя возвращаем nullptr
    return nullptr;
}

// регистрация пользователя
std::shared_ptr<User> Database::regUser(
    const std::string& login,
    const std::string& password,
    const std::string& name
) {
    try {
        if (login.empty() || password.empty() || name.empty())
        {
            throw ErrorCreateUserData(); // исключение, пользователь не создан
        }
        else
        {
            if (this->getOneUserByLogin(login)) {  // если пользователя нет, вернет nullptr
                // Пользователь с таким логином уже существует
                throw ErrorCreateUserExists();
            }
            else
            {
                // создаем пользователя, все нормально
                // Пытаемся создать пользователя
                auto user = std::make_shared<User>(login, password, name);
                // Добавляем копию указателя в базу данных
                setUser(user);
                // Возвращаем указатель на созданного юзера
                return user;
            }
        
        }
        
    } catch (const ErrorCreateUserExists& e) {  //обрабатываем исключения от класса User, дополня их
        std::cerr << _RED << "(Код ошибки 1) Ошибка регистрации: " << e.what() << _CLEAR << std::endl;
    } catch (const ErrorCreateUserData& e) {    // cerr стандартный поток для вывода ошибок и диагностических сообщений
        std::cerr << _RED << "(Код ошибки 2) Ошибка данных: " << e.what() << _CLEAR << std::endl;
    }
    return nullptr;
}


// Возвращает shared_ptr<User> при успехе или nullptr при ошибке
std::shared_ptr<User> Database::autorizUser(
    const std::string& login,
    const std::string& password
){
    auto user = this->getOneUserByLogin(login); // нашли логин в базе данных
    if (user.get())                             // если не nullptr
    {
        if (user->getPass() == password )        //если пароль совпадает логин уже и так совпал, нашелся же пользователь
        {
            return user;                         
        }
        else
        {
            return nullptr;
        }  
    }
    else
    {
        return nullptr;
    }
}


void Database::loadChats() {
    // файлы лежат в "file/"
    for (auto& path : listChatFiles("file")) {
        // файл: ".../file/login1_login2.txt"
        auto fname = std::filesystem::path(path).stem().string(); 
        auto pos = fname.find('_');
        if (pos == std::string::npos) continue;

        auto login1 = fname.substr(0, pos);
        auto login2 = fname.substr(pos + 1);

        auto u1 = getOneUserByLogin(login1);
        auto u2 = getOneUserByLogin(login2);
        if (!u1 || !u2) continue;

        auto chatP = std::make_shared<ChatPrivate>(u1, u2);
        u1->setChat(chatP);
        u2->setChat(chatP);

        chatP->loadHistory(path, *this);
    }
}