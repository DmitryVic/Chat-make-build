#pragma once
#include <vector>
using namespace std;

//проверяет есть ли элемент в векторе, если да, то возвращает true, false - нет
template <typename T>
bool contains(const std::vector<T>& vec, const T& value){
    for (const auto& el: vec)
    {
        if (el == value)
        {
            return true;
        }
    }
    return false;
} 