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

#include <SFML/Graphics.hpp>


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
        int id = 1;
        bool changes = true;
        double pheremones = 0.1;


        void update(std::vector<sf::CircleShape>& nCircles, std::vector<sf::Vertex>& edges);
        void handleEvents(sf::RenderWindow &window);
        void render(sf::RenderWindow &window, const std::vector<sf::CircleShape> &nodes, const std::vector<sf::Vertex> &edges);

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
        void solveProblem();

        std::vector<coord> randomSol(unsigned int seed);

        std::vector<std::pair<std::string ,coord>> getNodeCoordSection();
        std::map<std::string, std::vector<std::pair<std::string, double>>> getAdjList();

        std::map<std::string, coord> getFullCoordInfo();



};

#endif