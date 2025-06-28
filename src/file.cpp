#include "file.h"
#include "user.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#ifdef _WIN32
  #include <windows.h>
#endif

namespace fs = std::filesystem;

#ifdef _WIN32
// Конвертация UTF-8 std::string -> UTF-16 std::wstring
static std::wstring utf8_to_wstring(const std::string& str) {
    if (str.empty()) return {};
    // узнаём, сколько UTF-16 символов нам нужно (включая завершающий '\0')
    //::MultiByteToWideChar(...) из глобальной области видимости WinAPI
    int size_needed = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    // создаём строку нужного размера, заполненную нулём
    std::wstring wstr(size_needed, L'\0');
    // конвертируем
    ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
    // убрать последний (надёжно) завершающий нулевой символ
    if (!wstr.empty() && wstr.back() == L'\0')
        wstr.pop_back();
    return wstr;
}

// Конвертация UTF-16 std::wstring -> UTF-8 std::string
static std::string wstring_to_utf8(const std::wstring& wstr) {
    if (wstr.empty()) return {};
    int size_needed = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, '\0');
    ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
    if (!str.empty() && str.back() == '\0') str.pop_back();
    return str;
}
#endif

// Файл существует или нет
bool fileExists(const std::string& filename) {
#ifdef _WIN32
    fs::path p{ utf8_to_wstring(filename) };
    return fs::exists(p) && fs::is_regular_file(p);
#else
    std::ifstream file(filename);
    return file.good();
#endif
}

// Создает директорию, если её нет
void createDirectoryIfNeeded(const std::string& path) {
#ifdef _WIN32
    fs::path p{ utf8_to_wstring(path) };
    auto dir = p.parent_path();
    if (!dir.empty() && !fs::exists(dir)) {
        fs::create_directories(dir);
        std::wcout << L"Создана директория: " << dir.wstring() << std::endl;
    }
#else
    auto dir_path = fs::path(path).parent_path();
    if (!dir_path.empty() && !fs::exists(dir_path)) {
        fs::create_directories(dir_path);
        std::cout << "Создана директория: " << dir_path << std::endl;
    }
#endif
}

// Создать файл
void createFile(const std::string& filename) {
    createDirectoryIfNeeded(filename);
#ifdef _WIN32
    fs::path p{ utf8_to_wstring(filename) };
    std::ofstream file(p, std::ios::out);
#else
    std::ofstream file(filename, std::ios::out);
#endif
    if (file.is_open()) {
        std::cout << "Файл " << filename << " создан успешно." << std::endl;
        file.close();
    } else {
        std::cout << "Не удалось создать файл " << filename << std::endl;
    }
}

// Список всех .txt-файлов в директории dir
std::vector<std::string> listChatFiles(const std::string& dir) {
    std::vector<std::string> out;
#ifdef _WIN32
    fs::path pdir{ utf8_to_wstring(dir) };
    if (!fs::exists(pdir) || !fs::is_directory(pdir)) return out;
    for (auto& entry : fs::directory_iterator(pdir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            out.push_back(wstring_to_utf8(entry.path().wstring()));
        }
    }
#else
    if (!fs::exists(dir) || !fs::is_directory(dir)) return out;
    for (auto& p : fs::directory_iterator(dir)) {
        if (p.is_regular_file() && p.path().extension() == ".txt") {
            out.push_back(p.path().string());
        }
    }
#endif
    return out;
}

//=================================================================
// БД: запись и чтение структур
//=================================================================

void write_BD(const std::string& filename, std::shared_ptr<User> user) {
#ifdef _WIN32
    fs::path p{ utf8_to_wstring(filename) };
    std::ofstream fs(p, std::ios::app);
#else
    std::ofstream fs(filename, std::ios::app);
#endif
    if (fs.is_open()) {
        fs << user->getLogin() << '\n'
           << user->getPass() << '\n'
           << user->getName() << "\n\n";
        fs.close();
    } else {
        std::cerr << "Ошибка записи в файл БД: " << filename << std::endl;
    }
}

void write_Chat_P(const std::string& filename, std::shared_ptr<User> user, const std::string& mess) {
    if (!fileExists(filename)) return;
#ifdef _WIN32
    fs::path p{ utf8_to_wstring(filename) };
    std::ofstream fs(p, std::ios::app);
#else
    std::ofstream fs(filename, std::ios::app);
#endif
    if (fs.is_open()) {
        fs << user->getLogin() << '\n'
           << mess << "\n\n";
        fs.close();
    } else {
        std::cerr << "Ошибка записи в файл чата: " << filename << std::endl;
    }
}

bool load_Chat_P(const std::string& filename,
                  std::vector<std::pair<std::string, std::string>>& out) {
    if (!fileExists(filename)) return false;
#ifdef _WIN32
    fs::path p{ utf8_to_wstring(filename) };
    std::ifstream fs(p);
#else
    std::ifstream fs(filename);
#endif
    if (!fs.is_open()) return false;

    std::string login, mess;
    while (std::getline(fs, login) && std::getline(fs, mess)) {
        out.emplace_back(login, mess);
        std::string sep;
        std::getline(fs, sep);
    }
    return true;
}
