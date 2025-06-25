#pragma once
#include <string>
#include <vector>
#include <memory>
#include "database.h"



class ChatPrivate;
class ChatHared;

/*
Класс User взаимодействует с классами:
        Database                - создает данный клас и осуществляет логирование
        Chat    -> ChatPrivate     - объект User хранит указатели на чаты, что обеспечивает доступ к уже имеющимся приватным чатам Хранит. историю сообщений.
*/
class User
{
private:
    std::string _login; 
    std::string _pasword; //храниться хэш
    std::string _name; 
    std::vector<std::shared_ptr<ChatPrivate>> _connectChatPrivate;
public:
    // Передать логин, пароль, имя
    // Проверка на создание в БД, БД владееет пользователями, создание Юзера через нее
    User(const std::string& login, const std::string& pasword, const std::string& name)
    : _login(login), _pasword(pasword), _name(name) {}
    
    ~User() {}
    
    // Получить логин
    std::string getLogin() const;

    // Получить имя
    std::string getName() const;

    // Получить проль не безопасный
    std::string getPass() const;

    // Получить список чатов пользователя
    std::vector<std::shared_ptr<ChatPrivate>> getConnectionChatPrivate() const;

    // Добавить пользователя в приватный чат
    // вернет true - при успехе, false - при ошибке
    bool setChat(std::shared_ptr<ChatPrivate> chat);

        // проверяет есть чаты с данным узером std::weak_ptr<User> user
    bool userInChatsP(std::weak_ptr<User> user) const;

};


