#include "database.h"
#include "colorConsole.h"
#include "user.h" 
#include "exept.h"
#include <iostream>
#include <set>



Database::Database()
{
    std::cout << _BLUE <<  "База данных запущена!" <<  _CLEAR << std::endl;
}

Database::~Database()
{
    
}

// Добавить пользовател - принимает и временные, и существующие объекты
void Database::setUser(std::shared_ptr<User> user){
    this->usersInChat.insert(std::move(user));       // Перемещаем, чтобы избежать копирования
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
                this->usersInChat.insert(user);
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