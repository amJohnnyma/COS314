#ifndef TS_H
#define TS_H

#include "../../Utils/structs.h"
#include "../../Utils/Logger.h"
#include "ProblemInstance.h"
#include "iostream"
#include <map>

#include <vector>
#include <cmath>
#include <random>
#include <ctime>
#include <algorithm>
#include <unordered_set>

class ProblemInstance;

class TS
{
    private:
        std::vector<coord> currentSolution;
        int maxTabuListLength;
        std::unordered_set<std::vector<coord>, VectorHash> tabuList;
      //  std::vector<std::vector<coord>> tabuList;
        unsigned int seed;
    public:
        TS(ProblemInstance pI, unsigned int seed);
        std::vector<coord> solve(int maxIt, double maxTL);
        void swapCities(std::vector<coord>& route, size_t i, size_t j);
        double distanceR(const coord &a, const coord &b);
        double totalDistance(std::vector<coord> &route);
        std::string getPath(std::vector<coord> &route, ProblemInstance pi);
};

#endif