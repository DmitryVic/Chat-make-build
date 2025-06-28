#include "chathared.h"
#include "chat.h"
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "exept.h"
#include "user.h" 
#include "colorConsole.h"
#include <iostream>
#include "file.h"
#include <fstream>


//путь к файлу общего чата, путь к БД находится в БД cpp
static std::string CHAT_HARED_FILE = "file/ChH.txt";



// Конструктор Деструктор
ChatHared::ChatHared(const std::string &nameChat) 
    : _nameChat(nameChat)  // Инициализация через const &
{
    if (_nameChat.empty()) {  // Проверка уже после инициализации
        throw ErrorChat();    // Пустое название чата
    }
}

ChatHared::~ChatHared() = default;


// Получить все сообщения предать юзера относительно которого будет осуществлятся форматирование
std::string ChatHared::getAllMessage(std::weak_ptr<User> user) const{
    try
    {
        if (user.expired())                                                 // если указывает на не существующий объект
            throw ErrorChat();
        
        std::string output;                                                 //выходное значение
        if (this->historyChat.size() == 0)                                  //если вектор чата пуст
        {
            output += _CYAN;
            output += "\nИстоия чата пуста. Напишите сообщение первым!\n\n";
            output += _CLEAR;
            return output;
        }
        else
        {
            size_t longHistoryChat = this->historyChat.size();              //размер вектора чата
            for (size_t i = 0; i < longHistoryChat; i++)
            {
                // указатель на пользователя в historyChat и user привожу к shared_ptr и сравниваю
                // если это переданный пользователь, то меняю форматирование текста
                if (this->historyChat[i].first.lock() == user.lock())
                {
                    output += "\n";
                    output += _CYAN;
                    output += this->historyChat[i].first.lock()->getName(); // Добавляю имя пользователя в вывод
                    output += _CLEAR;
                    output += "\t";
                    output += this->historyChat[i].second;                  //сообщение
                }
                else{
                    output += "\n";
                    output += _YELLOW;
                    output += this->historyChat[i].first.lock()->getName(); // Добавляю имя пользователя в вывод
                    output += _CLEAR;
                    output += "\t";
                    output += this->historyChat[i].second;                  //сообщение
                }
            }
            return output;
        }
    }
    catch (const ErrorChat& e) {
        std::cerr << _RED << "(Код ошибки 6) Пользователь удален: " << e.what() << _CLEAR << std::endl;
        return _RED + std::string("Ошибка") + _CLEAR;                       // Возвращаем сообщение об ошибке
    }
    return ""; // Запасной вариант (технически недостижим)
}

// Добавить сообщение, передать отправителя и стекст сообщения, возврат bool об успехе операции
bool ChatHared::addMessage(std::weak_ptr<User> sender, const std::string& msg){
    try
    {
        if (sender.expired())                                                // если sender указывает на не существующий объект
            throw ErrorChat();

        if (msg.size() == 0)                                                // если сообщение пустое
            throw ErrorChatMess();
        
        this->historyChat.emplace_back(sender, msg);                         //emplace_back - избегает лишнего копирования, сразу передает значения
        
        //Запись в файл
        if(!fileExists(CHAT_HARED_FILE)) // выход еслифайла нет
            return false;

        std::ofstream fs(CHAT_HARED_FILE, std::ios::app);  // app = append (добавление в конец)
        //ессли ссылка не действительна не записываем
        if (std::shared_ptr<User> user = sender.lock())
        {    
            if (fs.is_open()) {
                fs << user->getLogin() << '\n' 
                << msg << "\n\n";        // Два \n для разделения записей
                fs.close();
            } 
        }else{return false;}
        return true;
    }
    catch (const ErrorChatMess& e) {                                          //обрабатываем исключения, дополня их
        std::cerr << _RED << "(Код ошибки 7) Не верные данные: " << e.what() << _CLEAR << std::endl;
        return false;
    }
    catch (const ErrorChat& e) {                                            //обрабатываем исключения, дополня их
        std::cerr << _RED << "(Код ошибки 8) Отправитель не найден: " << e.what() << _CLEAR << std::endl;
        return false;
    } 
    return false;
}


// Получить название чата
std::string ChatHared::getNameChat() const{
    return this->_nameChat;
} 


//Востановить историю чата + создать файл, если отсутствует
void ChatHared::loadChatHered(std::shared_ptr<Database> db){
    //Проверка существования файла приватного чата
    if (!fileExists(CHAT_HARED_FILE)) {
        std::cout << _CYAN << "История общего чата пуста, создаю конфу..." << CHAT_HARED_FILE <<  _CLEAR << std::endl;
        createFile(CHAT_HARED_FILE);
    } else {
        std::cout << _CYAN << "Востанавливаю историю общего чата..." << CHAT_HARED_FILE <<  _CLEAR << std::endl;
        std::ifstream file(CHAT_HARED_FILE);
        if (!file.is_open()) {
            std::cerr << _RED << "Ошибка открытия файла общего чата для чтения!" << _CLEAR << std::endl;
            return;
        }

        std::string login, mess;
        //читаем файл
        while (std::getline(file, login) && std::getline(file, mess))
        {
            // Если любое из полей пустое - пропускаем эту итерацию цикла Защита от некорр данных
            if (login.empty() || mess.empty()) {
                continue;
            }

            std::weak_ptr<User> usr = db->getOneUserByLogin(login);
            //auto user_ptr = usr.lock() гарантирует безопасность хранения и использования объекта, usr.lock() не достаточно!
            if (auto user_ptr = usr.lock()) {
                historyChat.emplace_back(std::weak_ptr<User>(user_ptr), mess);
            }


            // Пропускаем пустую строку-разделитель
            std::string empty_line;
            std::getline(file, empty_line);
        }
        file.close();
        std::cout << _BLUE <<  "История общего чата востановлена" <<  _CLEAR << std::endl;
    }

}