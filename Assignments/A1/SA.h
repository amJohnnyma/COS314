#ifndef SA_H
#define SA_H

#include "structs.h"
#include "../../Utils/Logger.h"
#include "ProblemInstance.h"
#include "iostream"
#include <map>

#include <vector>
#include <cmath>
#include <random>
#include <ctime>
#include <algorithm>

class ProblemInstance;

class SA
{
    private:
        std::vector<coord> currentRoute;
        std::vector<coord> bestRoute;
        double curDistance;
        double bestDistance;
        unsigned int seed;
    public:
        SA(ProblemInstance pI, unsigned int seed);
        std::vector<coord> solve(double initTemp,double finTemp, double coolRate, int maxIt);
        void swapCities(std::vector<coord>& route, size_t i, size_t j);
        double distanceR(const coord &a, const coord &b);
        double totalDistance(std::vector<coord> &route);
        std::string getPath(std::vector<coord> &route, ProblemInstance pi);
};
#endif