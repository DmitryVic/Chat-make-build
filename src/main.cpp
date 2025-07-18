#include "database.h"
#include "colorConsole.h"
#include "user.h"
#include "chatPrivate.h"
#include "chathared.h"
#include "chat.h"
#include "template.h"
#include "interactionChatPrvate.h"
#include "interactionChatHared.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <limits> //Для  cin.ignore(numeric_limits<streamsize>::max(), '\n');
#include <clocale> //для правильной лолокализации
#include <locale>
#include "hashPass.h"
using namespace std;

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN  // Уменьшает количество включаемых заголовков Windows
    #include <windows.h>        // Используется для настройки консоли
    #define OS "Win"
#elif defined(__linux__)
    #include <sys/utsname.h>
    #define OS "Linux"
#endif




void chatStart(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization, shared_ptr<ChatHared>& haredChat){
    char menu = '9';

    while (menu != '0')
    {
        cout << _GREY_BG << "\n\n\t\tЗдраствуйте - " << userAuthorization->getName() << "\n" << _CLEAR << endl << endl;
        cout << "Для выбора введите значение."<< endl 
            << _CYAN << " Меню:" << _CLEAR << endl
            << "0 - Выход в главное меню;" << endl
            << "1 - Открыть список приватных чатов - написать пользователю;" << endl
            << "2 - Открыть общий чат" << endl
            << "Ведите значение" << endl;
        // Обработка ввода
        if (!(cin >> menu)) {
            cin.clear(); // Сброс флагов ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
            cout << _YELLOW << "Ошибка: введите число." << _CLEAR << endl;
            menu = '9'; // Предотвращаем выход из цикла
            continue;
        }

        switch (menu)
        {
        case '0':
            cout << _GREEN << "Возврат в главное меню!" << _CLEAR;
            return;
        case '1':
            menuChatPrivate(db, userAuthorization);
            break;
        case '2':
            openChatHared(db, userAuthorization, haredChat);
            break;
        default:
            cout << _YELLOW << "Не верно введено значение, попробуй еще раз" << _CLEAR << endl;
            break;
        }
    }
    
}

void authorization(shared_ptr<Database>& db, shared_ptr<ChatHared>& haredChat){
    cout << _GREY_BG << "\n\n\t\tАвторизация\n" << _CLEAR << endl << endl;
    string login;
    string pass;

    cin.clear(); // Сбрасывает флаги ошибок
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищает буфер до новой строки

    cout << "Введите логин" << endl;
    getline(cin, login);

    cout << "Введите пароль" << endl;
    getline(cin, pass);
    
    auto userAuthorization = db->autorizUser(login, hashPassword(pass));
    if (!userAuthorization)
    {
        cout << _MAGENTA << "Не удалось авторизоваться, возврат в главное меню" << _CLEAR << endl;
        return;
    }
    else
    {
        cout << _GREEN << "Авторизация выполнена!" << _CLEAR;
        chatStart(db, userAuthorization, haredChat);
    }
    
}

void reg(shared_ptr<Database>& db, shared_ptr<ChatHared>& haredChat){
    cout << _GREY_BG << "\n\n\t\tРегистрация\n" << _CLEAR << endl << endl;
    string login;
    string pass;
    string name;
    cin.clear(); // Сбрасывает флаги ошибок
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищает буфер до новой строки

    cout << "Введите логин" << endl;
    getline(cin, login);

    cout << "Введите пароль" << endl;
    getline(cin, pass);

    cout << "Введите свое имя" << endl;
    getline(cin, name);
    auto userAuthorization = db->regUser(login, hashPassword(pass), name);
    if (!userAuthorization)
    {
        cout << _MAGENTA << "Не удалось зарегистрироваться, возврат в главное меню" << _CLEAR ;
        return;
    }
    else
    {
        cout << name << _GREEN << " - Вы успешно зарегистрировались" << _CLEAR;
        chatStart(db, userAuthorization, haredChat);
    }    
}


int main()
{

    // Универсальная настройка локали
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    
    // Настройки для Windows
    // исправляет не коректную запись в string русских символов в консоле
    // распространяет локализацию на весь проект 
    #if defined(_WIN32)
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        cout << _GREY_BG << "Сработала ветка OS: " << OS << _CLEAR << endl << endl;

        std::cout << "OS name: Windows" << std::endl;
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
         std::cout << "Процессорная архитектура: ";
        switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            std::cout << "x64" << std::endl;
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            std::cout << "x86" << std::endl;
            break;
        default:
            std::cout << "Другая" << std::endl;
            break;
        }
        std::cout << "Количество ядер: " << sysInfo.dwNumberOfProcessors << std::endl;
        // Получаем идентификатор текущего процесса (PID)
        DWORD processId = GetCurrentProcessId();
        std::cout << _GREY_BG  << "ID процесса: " << processId << _CLEAR << std::endl;
        std::cout << "RUN...." << std::endl;

    // Для Linux/Mac
    // был ли определен SET_GLOBAL_LOCALE_LINUX ? да (в cmake) или нет (в cmake)
    #elif defined(__linux__)
        std::locale::global(std::locale("ru_RU.UTF-8"));
        cout << _GREY_BG << "Сработала ветка OS: " << OS << _CLEAR << endl << endl;
        struct utsname utsname;
        uname(&utsname);
        cout << "OS name: " << utsname.sysname << endl; 
        cout << "Host name: " << utsname.nodename << endl; 
        cout << "OS release: " << utsname.release << endl; 
        cout << "OS version: " << utsname.version << endl; 
        cout << "Architecture: " << utsname.machine << endl;
        cout << "RUN...." << endl;
    #endif


    char menu = '9';
    shared_ptr<Database> database(new Database());
    //Создание и загрузка истории общего чата
    shared_ptr<ChatHared> haredChat(new ChatHared("Общий чат"));
    haredChat->loadChatHered(database);

    cout << _GREEN <<  "Готов к работе" <<  _CLEAR << std::endl;

    while (menu != '0')
    {
        cout << _GREY_BG << "\n\n\t\tДобо пожаловать в чат!\n" << _CLEAR << endl << endl;
        cout << "Для выбора введите значение."<< endl 
            << _CYAN << " Меню:" << _CLEAR << endl
            << "0 - Закрыть приложение;" << endl
            << "1 - Авторизация;" << endl
            << "2 - Регистрация" << endl
            << "Ведите значение" << endl;

        // Обработка ввода
        if (!(cin >> menu)) {
            cin.clear();                                                // Сброс флагов ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n');        // Очистка буфера
            cout << _YELLOW << "Ошибка: введите число." << _CLEAR << endl;
            menu = '9';                                                 // Предотвращаем выход из цикла
            continue;
        }

        switch (menu)
        {
        case '0':
            cout << _GREEN << "Досвидания!" << _CLEAR;
            return 0;
        case '1':
            authorization(database, haredChat);
            break;
        case '2':
            reg(database, haredChat);
            break;
        default:
            cout << _YELLOW << "Не верно введено значение, попробуй еще раз" << _CLEAR << endl;
            break;
        }
    }
    return 0;
}