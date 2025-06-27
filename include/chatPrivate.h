#pragma once
#include "chat.h"
#include <string>
#include <utility>
#include <memory>
#include <utility>

// приватный чат хранит историю чата, пользователей
class ChatPrivate : public Chat
{
protected:
// Список участников
std::vector<std::weak_ptr<User>> usersInChat;

public:
    // при создни нужно указать пользователей для usersInChat, при этом historyChat будет пустым без истории
    // getAllMessage выводит сообщений нет
    ChatPrivate(std::weak_ptr<User> thisUser, std::weak_ptr<User> otherUser); 
    ~ChatPrivate() override;
    
    // Получить имена всех участников
    std::string showUsers() const;

    // Получить weak_ptr<User> всех участников
    std::vector<std::weak_ptr<User>> UsersInChatPtr() const; 

    // Должен предать все сообщения из historyChat, форматируя текст относительно пользователя User, тоесть того, кто открыл чат
    std::string getAllMessage(std::weak_ptr<User> user) const override;


    // Добавить сообщение
    // передаем указатель отправителя и само сообщение, записываем в historyChat
    // вернет true - при успехе, false - при ошибке
    bool addMessage(std::weak_ptr<User> sender, const std::string& msg) override;

    //есть ли в данном чате пользователь
    bool userInChat(std::weak_ptr<User> user) const;
}; 


