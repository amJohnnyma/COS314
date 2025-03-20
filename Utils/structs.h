#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <vector>
#include <unordered_set>

struct coord
{
    std::string name;
    double x;
    double y;

    bool operator==(const coord& other) const {
        return x == other.x && y == other.y;
    }
};

// Hash function for a single coord
struct CoordHash {
    size_t operator()(const coord& c) const {
        return std::hash<double>()(c.x) ^ (std::hash<double>()(c.y) << 1);
    }
};

// Hash function for a vector of coords (needed for tabu list)
struct VectorHash {
    size_t operator()(const std::vector<coord>& v) const {
        size_t hashVal = v.size();  // Base hash on size to ensure valid start
        for (const coord& c : v) {
            hashVal ^= CoordHash()(c) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
        }
        return hashVal;
    }
};
#endif