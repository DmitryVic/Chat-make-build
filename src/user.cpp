#include "user.h"
#include "chat.h"
#include "database.h"
#include "exept.h"
#include <exception>
#include "chatPrivate.h"
#include "chathared.h"






// Получить логин
std::string User::getLogin() const{
    return this->_login;
}

// Получить имя
std::string User::getName() const{
    return this->_name;
}

// Получить проль не безопасный
std::string User::getPass() const{
    return this->_pasword;
}


// Получение чатов
std::vector<std::shared_ptr<ChatPrivate>> User::getConnectionChatPrivate() const {
    return _connectChatPrivate;
}


//Добавление в приватный чат
bool User::setChat(std::shared_ptr<ChatPrivate> chat) {
    _connectChatPrivate.push_back(chat);
    return true;
}


// проверяет есть чаты с данным узером std::weak_ptr<User> user
bool User::userInChatsP(std::weak_ptr<User> user) const{
    size_t sizeConnectChatPrivate = this->_connectChatPrivate.size();
    if (sizeConnectChatPrivate != 0)                                    // если не пустой вектор
    {
        for (size_t i = 0; i < sizeConnectChatPrivate; i++)
        {
            if (this->_connectChatPrivate[i]->userInChat(user))          // обращаемся к чату проверить есть ли в данном чате пользователь std::weak_ptr<User> user
            {
                return true;
            }
        }
    }
    return false;
}