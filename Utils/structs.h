#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>

struct coord
{
    std::string name;
    double x;
    double y;

    bool operator==(const coord& other) const {
        return x == other.x && y == other.y;
    }
};

#endif