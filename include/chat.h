#pragma once
#include <string>
#include <vector>
#include <memory>
#include <utility>

// Исправлена ошибка объявления user.h
class User;

/*
Chat - абстрактный класс, наследники ChatPrivate, ChatHared
*/
class Chat
{
protected:  
    /*
        historyChat - состоит из списка пар 
            pair - должен содержать указатель на пользователя и само сообщение
            pair  в последующем можно заменить на tuple, если будет нужно больше полей, например, дата/время
        
        .first - weak_ptr<User>
        .second - string Message
    */
    std::vector<std::pair<std::weak_ptr<User>, std::string>> historyChat;
    

public:
    virtual ~Chat() = default;

    // Должен предать все сообщения из historyChat, форматируя текст относительно пользователя User, тоесть того, кто открыл чат
    virtual std::string getAllMessage(std::weak_ptr<User> user) const = 0;


    // Добавить сообщение
    // передаем указатель отправителя и само сообщение, записываем в historyChat
    // вернет true - при успехе, false - при ошибке
    virtual bool addMessage(std::weak_ptr<User> sender, const std::string& msg) = 0;
};


