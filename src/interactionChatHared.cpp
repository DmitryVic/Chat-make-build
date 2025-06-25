#include "interactionChatHared.h"
#include "database.h"
#include "user.h"
#include "chathared.h"
#include "chat.h"
#include "template.h"
#include "colorConsole.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits> //Для  cin.ignore(numeric_limits<streamsize>::max(), '\n');
using namespace std;


//открывает общий чат, получает от пользователя сообщеня при "0" - выход, передать базу данных, залогированнного пользователя, общий чат
void openChatHared(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization, shared_ptr<ChatHared>& chatP){
    cout << _GREEN << "Зашли в общий чат"  << _CLEAR << endl;
    string userInput;                                                       // Вводимое пользователем знначение
    while (userInput != "0")
    {
        cout << _GREY_BG << "\n\n\t\t" << chatP->getNameChat() << " - всего пользователей " << db->getAllUsersInChat().size() << _GREY_BG << "\n" << _CLEAR  << endl;
        cout << chatP->getAllMessage(userAuthorization) << endl << endl;
        cout    << _CYAN << "Напишите сообщение:" << _CLEAR << endl
                << "0 - отправте \"0\" чтобы выйти" << endl;
        std::getline(std::cin >> std::ws, userInput);                       // Читаем всю строку
        if (userInput != "0")                                               // при выходе пропускаем запись
        {
            chatP->addMessage(userAuthorization, userInput);
        }
    }

}