
#pragma once
#include "database.h"
#include "user.h"
#include "chatPrivate.h"
#include "chat.h"
#include "template.h"
#include <memory>
using namespace std;

// открывает чат, записывает принятые сообщения от пользователей
void openChatPrivate(shared_ptr<User>& userAuthorization, shared_ptr<ChatPrivate>& chatP);

// предоставляет доступ выбора чата из имеющихся
void  UserChoiceChatPrivate(shared_ptr<User>& userAuthorization);
 
// предоставляет варианты кому написать из пользователей, создает чат если нет, то сообщает об этом
void createChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization);

//предоставляет выбор написать пользователю или зайти в имеющийся чат
void menuChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization);