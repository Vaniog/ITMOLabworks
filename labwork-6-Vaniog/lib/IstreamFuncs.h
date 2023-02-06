#pragma once
#include <iostream>
#include <string>
#include <functional>

namespace isf {

std::string GetWhile(std::istream& is, const std::function<bool(uint8_t)>& IsValid);
void IgnoreWhile(std::istream& is, const std::function<bool(uint8_t)>& IsIgnorable);
}