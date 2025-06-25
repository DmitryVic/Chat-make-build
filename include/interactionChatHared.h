#pragma once
#include "database.h"
#include "user.h"
#include "chathared.h"
#include "chat.h"
#include "template.h"
#include <memory>
using namespace std;

//открывает чат, записывает принятые сообщения от пользователей и добавляет еще пользователей
void openChatHared(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization, shared_ptr<ChatHared>& chatP);
 