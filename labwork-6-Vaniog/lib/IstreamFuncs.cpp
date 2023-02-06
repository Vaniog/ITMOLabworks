#include "IstreamFuncs.h"

namespace isf{

std::string GetWhile(std::istream& is, const std::function<bool(uint8_t)>& IsValid) {
    std::string str;
    while (is && IsValid(is.peek())) {
        str += static_cast<char>(is.get());
    }
    return str;
}

void IgnoreWhile(std::istream& is, const std::function<bool(uint8_t)>& IsIgnorable) {
    while (is && IsIgnorable(is.peek())) {
        is.ignore();
    }
}
}