#include "Color.h"

Color::Color() : colorInt(0) {
}

Color::Color(unsigned int colorInt) : colorInt(colorInt) {
}

Color::Color(BYTE r, BYTE g, BYTE b) : Color(r, g, b, 255) {
}

Color::Color(BYTE r, BYTE g, BYTE b, BYTE a) {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}

Color::Color(const Color &src) : colorInt(src.colorInt) {
}

Color &Color::operator=(const Color &src) {
    this->colorInt = src.colorInt;
}

bool Color::operator==(const Color &rhs) const {
    return (this->colorInt == rhs.colorInt);
}

bool Color::operator!=(const Color &rhs) const {
    return (this->colorInt != rhs.colorInt);
}

constexpr BYTE Color::GetR() const {
    return rgba[0];
}

void Color::SetR(BYTE r) {
    rgba[0] = r;
}

constexpr BYTE Color::GetG() const {
    return rgba[1];
}

void Color::SetG(BYTE g) {
    rgba[1] = g;
}

constexpr BYTE Color::GetB() const {
    return rgba[2];
}

void Color::SetB(BYTE b) {
    rgba[2] = b;
}

constexpr BYTE Color::GetA() const {
    return rgba[3];
}

void Color::SetA(BYTE a) {
    rgba[3] = a;
}
