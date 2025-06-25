#pragma once
#include "chat.h"
#include <string>
#include <vector>
#include <memory>
#include <utility>


// Общий чат для всех, у всех есть доступ, создается изначально после БД, хранит только сообщения (юзер указатель, сообщение string)
class ChatHared : public Chat
{
private:
std::string _nameChat;
//Список участников Берется из базы данных 

public:

    // передать название чата
    explicit ChatHared(const std::string &nameChat);
    ~ChatHared() override;

    // Должен предать все сообщения из historyChat, форматируя текст относительно пользователя User, тоесть того, кто открыл чат
    std::string getAllMessage(std::weak_ptr<User> user) const override;

    // Добавить сообщение
    // передаем указатель отправителя и само сообщение, записываем в historyChat
    // вернет true - при успехе, false - при ошибке
    bool addMessage(std::weak_ptr<User> sender, const std::string& msg) override;


    // Получить название чата
    std::string getNameChat() const;

};