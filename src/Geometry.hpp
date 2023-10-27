#pragma once

struct Point;
struct Size;
struct Rect;

struct Point {
    float x;
    float y;

    Point() : x(0), y(0) {}
    Point(float _x, float _y) : x(_x), y(_y) {}
    Point(int _x, int _y) : x(_x), y(_y) {}

    bool operator== (Point other) const {
        return x == other.x && y == other.y;   
    };

    Point operator+ (Point other) const;
    Point operator- (Point other) const;
    Point operator* (Point other) const;
    Point operator/ (Point other) const;

    Point operator+ (Size other) const;
    Point operator- (Size other) const;
    Point operator* (Size other) const;
    Point operator/ (Size other) const;

    Point operator+ (float scalar) const;
    Point operator- (float scalar) const;
    Point operator* (float scalar) const;
    Point operator/ (float scalar) const;

    template<class Type> void operator+= (Type other);
    template<class Type> void operator-= (Type other);
    template<class Type> void operator*= (Type other);
    template<class Type> void operator/= (Type other);
};

struct Size {
    float width;
    float height;

    Size() : width(0), height(0) {}
    Size(float _w, float _h) : width(_w), height(_h) {}
    Size(int _w, int _h) : width(_w), height(_h) {}

    bool operator== (Size other) const {
        return width == other.width && height == other.height;   
    };

    Size operator+ (Size other) const;
    Size operator- (Size other) const;
    Size operator* (Size other) const;
    Size operator/ (Size other) const;

    Size operator+ (Point other) const;
    Size operator- (Point other) const;
    Size operator* (Point other) const;
    Size operator/ (Point other) const;

    Size operator+ (float scalar) const;
    Size operator- (float scalar) const;
    Size operator* (float scalar) const;
    Size operator/ (float scalar) const;

    template<class Type> void operator+= (Type other);
    template<class Type> void operator-= (Type other);
    template<class Type> void operator*= (Type other);
    template<class Type> void operator/= (Type other);
};

struct Rect {
    Point position;
    Size size;

    Rect rotateAround(Point point, float angle);

    void fix() {
        if (size.width < 0) {
            this->position.x += size.width;
            this->size.width = -size.width;
        };

        if (size.height < 0) {
            this->position.y += size.height;
            this->size.height = -size.height;
        };
    }

    Rect() : position(0, 0), size(0, 0) {fix();}
    Rect(Point position, Size size) : position(position), size(size) {fix();}
    Rect(float x, float y, float width, float height) : position(x, y), size(width, height) {fix();}
    Rect(int x, int y, int width, int height) : position(x, y), size(width, height) {fix();}

    float left() const { return position.x; }
    float right() const { return position.x + size.width; }
    float top() const { return position.y + size.height; }
    float bottom() const { return position.y; }

    Point topLeft() const { return Point(left(), top()); }
    Point topRight() const { return Point(right(), top()); }
    Point bottomLeft() const { return Point(left(), bottom()); }
    Point bottomRight() const { return Point(right(), bottom()); }

    bool operator== (Rect other) const {
        return position == other.position && size == other.size;   
    };

    Rect operator+ (Point other) const;
    Rect operator- (Point other) const;
    Rect operator* (Point other) const;
    Rect operator/ (Point other) const;

    Rect operator+ (Size other) const;
    Rect operator- (Size other) const;
    Rect operator* (Size other) const;
    Rect operator/ (Size other) const;

    Rect operator+ (Rect other) const;
    Rect operator- (Rect other) const;
    Rect operator* (Rect other) const;
    Rect operator/ (Rect other) const;

    template<class Type> void operator+= (Type other);
    template<class Type> void operator-= (Type other);
    template<class Type> void operator*= (Type other);
    template<class Type> void operator/= (Type other);

    Rect expand(float pad) const;
    bool intersects(Rect other) const;
};