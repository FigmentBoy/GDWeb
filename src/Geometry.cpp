#include "Geometry.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Point Point::operator+ (Point other) const {
    return Point(x + other.x, y + other.y);
}

Point Point::operator- (Point other) const {
    return Point(x - other.x, y - other.y);
}

Point Point::operator* (Point other) const {
    return Point(x * other.x, y * other.y);
}

Point Point::operator/ (Point other) const {
    return Point(x / other.x, y / other.y);
}

Point Point::operator+ (Size other) const {
    return Point(x + other.width, y + other.height);
}

Point Point::operator- (Size other) const {
    return Point(x - other.width, y - other.height);
}

Point Point::operator* (Size other) const {
    return Point(x * other.width, y * other.height);
}

Point Point::operator/ (Size other) const {
    return Point(x / other.width, y / other.height);
}

Point Point::operator+ (float scalar) const {
    return Point(x + scalar, y + scalar);
}

Point Point::operator- (float scalar) const {
    return Point(x - scalar, y - scalar);
}

Point Point::operator* (float scalar) const {
    return Point(x * scalar, y * scalar);
}

Point Point::operator/ (float scalar) const {
    return Point(x / scalar, y / scalar);
}

template<class Type> void Point::operator+= (Type other) {
    *this = operator+(other);
}

template<class Type> void Point::operator-= (Type other) {
    *this = operator-(other);
}

template<class Type> void Point::operator*= (Type other) {
    *this = operator*(other);
}

template<class Type> void Point::operator/= (Type other) {
    *this = operator/(other);
}

template void Point::operator+= (Point other);
template void Point::operator-= (Point other);
template void Point::operator*= (Point other);
template void Point::operator/= (Point other);

template void Point::operator+= (Size other);
template void Point::operator-= (Size other);
template void Point::operator*= (Size other);
template void Point::operator/= (Size other);

template void Point::operator+= (float scalar);
template void Point::operator-= (float scalar);
template void Point::operator*= (float scalar);
template void Point::operator/= (float scalar);

Size Size::operator+ (Size other) const {
    return Size(width + other.width, height + other.height);
}

Size Size::operator- (Size other) const {
    return Size(width - other.width, height - other.height);
}

Size Size::operator* (Size other) const {
    return Size(width * other.width, height * other.height);
}

Size Size::operator/ (Size other) const {
    return Size(width / other.width, height / other.height);
}

Size Size::operator+ (Point other) const {
    return Size(width + other.x, height + other.y);
}

Size Size::operator- (Point other) const {
    return Size(width - other.x, height - other.y);
}

Size Size::operator* (Point other) const {
    return Size(width * other.x, height * other.y);
}

Size Size::operator/ (Point other) const {
    return Size(width / other.x, height / other.y);
}

Size Size::operator+ (float scalar) const {
    return Size(width + scalar, height + scalar);
}

Size Size::operator- (float scalar) const {
    return Size(width - scalar, height - scalar);
}

Size Size::operator* (float scalar) const {
    return Size(width * scalar, height * scalar);
}

Size Size::operator/ (float scalar) const {
    return Size(width / scalar, height / scalar);
}

template<class Type> void Size::operator+= (Type other) {
    *this = operator+(other);
}

template<class Type> void Size::operator-= (Type other) {
    *this = operator-(other);
}

template<class Type> void Size::operator*= (Type other) {
    *this = operator*(other);
}

template<class Type> void Size::operator/= (Type other) {
    *this = operator/(other);
}

template void Size::operator+= (Point other);
template void Size::operator-= (Point other);
template void Size::operator*= (Point other);
template void Size::operator/= (Point other);

template void Size::operator+= (Size other);
template void Size::operator-= (Size other);
template void Size::operator*= (Size other);
template void Size::operator/= (Size other);

template void Size::operator+= (float scalar);
template void Size::operator-= (float scalar);
template void Size::operator*= (float scalar);
template void Size::operator/= (float scalar);

Rect Rect::operator+ (Rect other) const {
    return Rect(position + other.position, size + other.size);
}

Rect Rect::operator- (Rect other) const {
    return Rect(position - other.position, size - other.size);
}

Rect Rect::operator* (Rect other) const {
    return Rect(position * other.position, size * other.size);
}

Rect Rect::operator/ (Rect other) const {
    return Rect(position / other.position, size / other.size);
}

Rect Rect::operator+ (Point other) const {
    return Rect(position + other, size);
}

Rect Rect::operator- (Point other) const {
    return Rect(position - other, size);
}

Rect Rect::operator* (Point other) const {
    return Rect(position * other, size * other);
}

Rect Rect::operator/ (Point other) const {
    return Rect(position / other, size / other);
}

Rect Rect::operator+ (Size other) const {
    return Rect(position, size + other);
}

Rect Rect::operator- (Size other) const {
    return Rect(position, size - other);
}

Rect Rect::operator* (Size other) const {
    return Rect(position * other, size * other);
}

Rect Rect::operator/ (Size other) const {
    return Rect(position / other, size / other);
}

template<class Type> void Rect::operator+= (Type other) {
    *this = operator+(other);
}

template<class Type> void Rect::operator-= (Type other) {
    *this = operator-(other);
}

template<class Type> void Rect::operator*= (Type other) {
    *this = operator*(other);
}

template<class Type> void Rect::operator/= (Type other) {
    *this = operator/(other);
}

template void Rect::operator+= (Rect other);
template void Rect::operator-= (Rect other);
template void Rect::operator*= (Rect other);
template void Rect::operator/= (Rect other);

template void Rect::operator+= (Point other);
template void Rect::operator-= (Point other);
template void Rect::operator*= (Point other);
template void Rect::operator/= (Point other);

template void Rect::operator+= (Size other);
template void Rect::operator-= (Size other);
template void Rect::operator*= (Size other);
template void Rect::operator/= (Size other);

Rect Rect::expand(float pad) const {
    return Rect(position - Point(pad, pad), size + Size(pad * 2, pad * 2));
}

bool Rect::intersects(Rect other) const {
    return (position.x < other.position.x + other.size.width &&
            position.x + size.width > other.position.x &&
            position.y < other.position.y + other.size.height &&
            position.y + size.height > other.position.y);
}