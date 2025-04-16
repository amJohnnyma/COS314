#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>

struct coord
{
    int score;
    double x;
    double y;

    coord() : score(0), x(0.0), y(0.0) {}

    coord(int score, double x, double y)
        : score(score), x(x), y(y) {}

    bool operator==(const coord& other) const {
        return x == other.x && y == other.y && score == other.score;
    }

    std::string to_string()
    {
        return "Score: " + std::to_string(score) + '\t' + std::to_string(x) + ", " + std::to_string(y);
    }

    std::string coord_only_to_string()
    {
        return "("+std::to_string(x) + ", " + std::to_string(y) + ")";
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


struct vehicle
{
    double tmax;
    double score;
    //path -> node ID, node coord
    std::vector<std::pair<std::string, coord>> path;
    double travelDistance;
    std::vector<std::pair<coord,double>> scores;

    std::string to_string()
    {
        std::string p = "\nPath:\n";
        for(const auto &i : path)
        {
            coord c = i.second;
            p += "(" + i.first + ")" + c.to_string() + "\n";
        }
        return "Tmax:" + std::to_string(tmax) + " Score:" + std::to_string(score) + " travelDistance:" + std::to_string(travelDistance) + p;
    }
};

struct param
{
    double pheromones;
    double pheromoneImportance;
    double heuristicImportance;
    double evaporationRate;
    double Q;
    unsigned int seed;
    int maxIt;
    int maxItNoImpro;
    double newPathCoeff;

    std::string to_string()
    {
        return "alpha: " + std::to_string(pheromoneImportance) + "\tbeta: " + std::to_string(heuristicImportance) + "\tevap: " + std::to_string(evaporationRate) + "\tQ: "+ std::to_string(Q);
    }
};

struct seedResult
{
    unsigned int seed; //
    std::vector<coord> solution; //depot -> nodes -> depot
    double cost; //Distance travelled by seeds
    double duration;
    std::chrono::duration<double> runtime; //Total time to run
    int iterations;

    std::string to_string()
    {
        std::string sol = "";
        for(auto &s : solution)
        {
            sol += s.coord_only_to_string() + "->";
        }
        return sol;
    }
};
struct allResult
{
    std::string name; //instance name
    std::vector<seedResult> results; //will need to string

     
    //Total cost
    //total duration
    //total runtime

};

struct TuneResult {
    param p;
    double score; 
    std::chrono::duration<double> runtime;
};




#endif