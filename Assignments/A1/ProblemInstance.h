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

class ProblemInstance
{
    private:
        std::string name;
        std::string type;
        std::string dimension;
        int dimenInt;
        std::string edge_weight_type;
        std::map<std::string, coord> node_coord_section;
        std::map<std::string, std::vector<std::pair<std::string, double>>> adjList;

    public:
        ProblemInstance(std::string filename);
        ~ProblemInstance();

        void addCoord(std::string id, double x, double y);
        double distance(const coord& a, const coord& b);
        void addEdge(std::string u, std::string v);

        void printGraph();

        std::string getName();
        std::string getType();
        std::string getEdgeWeightType();
        int getDimension();
        std::vector<coord> getNodeCoordSection();

        std::vector<coord> randomSol(unsigned int seed);

        std::map<std::string, std::vector<std::pair<std::string, double>>> getAdjList();

        std::map<std::string, coord> getFullCoordInfo();



};

#endif