#pragma once
#include <exception>


// Ошибка передаваемых данных
class ErrorCreateUserData : public std::exception 
{
public:
	//переопределяем виртуальный метод обещая
	//не использовать исключения внутри
    virtual const char* what() const noexcept override   
    {
        return "Пользователь не создан, не коректные данные";
    }
};


// Ошибка логин уже занят
class ErrorCreateUserExists : public std::exception
{
public:
	//переопределяем виртуальный метод обещая
	//не использовать исключения внутри
    virtual const char* what() const noexcept override   
    {
        return "Пользователь не создан, такой пользователь уже существует";
    }
};


// Ошибка Чата
class ErrorChat : public std::exception 
{
public:
	//переопределяем виртуальный метод обещая
	//не использовать исключения внутри
    virtual const char* what() const noexcept override   
    {
        return "Ошибка приложения";
    }
};

// Ошибка доступа  
class ErrorChatAccess : public std::exception 
{
public:
	//переопределяем виртуальный метод обещая
	//не использовать исключения внутри
    virtual const char* what() const noexcept override   
    {
        return "Ошибка доступа";
    }
};


// Ошибка Чата
class ErrorChatMess : public std::exception 
{
public:
	//переопределяем виртуальный метод обещая
	//не использовать исключения внутри
    virtual const char* what() const noexcept override   
    {
        return "Вы не ввели сообщение. Введите текст, прежде чем отправить сообщение.";
    }
};