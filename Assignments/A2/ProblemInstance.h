#ifndef PI_H
#define PI_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <random>
#include <algorithm>
#include <map>
#include "structs.h"
#include "../../Utils/Logger.h"
#include <regex>
#include <set>


class ProblemInstance
{
    private:
        int numNodes; 
        int numVehicles;
        std::string tMaxString;
        double tmax;
        int score;
        coord depot;
        std::map<std::string, coord> node_coord_section;
        std::map<std::string, std::vector<std::pair<std::string, double>>> adjList;
        std::string debugfile = "TOP";
        int id = 0;

    public:
        ProblemInstance(std::string filename);
        ~ProblemInstance();

        void addCoord(double x, double y, int score);
        double distance(const coord& a, const coord& b);
        void addEdge(std::string u, std::string v);

        void printGraph();
        std::string printGraphAsTable();

        int getNumNodes();
        int getNumVehicles();
        double getTmax();
        std::vector<coord> getNodeCoordSection();

        std::vector<coord> randomSol(unsigned int seed);

        std::map<std::string, std::vector<std::pair<std::string, double>>> getAdjList();

        std::map<std::string, coord> getFullCoordInfo();



};

#endif