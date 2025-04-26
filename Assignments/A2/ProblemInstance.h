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
#include <unistd.h>  /

#include <SFML/Graphics.hpp>


class ProblemInstance
{
    private:
        int numNodes; 
        int numVehicles;
        std::string tMaxString;
        double tmax;
        int score;
        std::vector<coord> visited;
        std::vector<vehicle> vehicles;
        std::pair<std::string, coord> depot;
        std::map<std::string, coord> node_coord_section;
        std::string imageName;

        //id, vector<{id , distance}>
        std::map<std::string, std::vector<std::pair<std::string, double>>> adjList;
        std::string debugfile = "TOP";
        int id = 1;
        bool changes = true;        
        double n; //heuristic value
        double s; //score
        double c; // travel cost

        std::vector<std::pair<double, sf::Vertex>> edges; 
      //  std::vector<std::pair<double, std::pair<coord,sf::Vertex>>> alledges;   
        std::vector<std::pair<coord, std::vector<std::pair<double, std::pair<coord, sf::Vertex>>>>> alledges;
        int runNum = 1;  
        param params;
        std::mt19937 rng;
        double getBestScore();
        double prevBestScore = 0;
        int maxItNoImpro = 0;
        seedResult result;



        




        void update(std::vector<sf::CircleShape>& nCircles);
        void handleEvents(sf::RenderWindow &window);
        void render(sf::RenderWindow &window, const std::vector<sf::CircleShape> &nodes/*, const std::vector<std::pair<double, sf::Vertex>>& edges*/);

    public:
        ProblemInstance(std::string filename, param params);
        ~ProblemInstance();

        void addCoord(double x, double y, int score);
        double distance(const coord& a, const coord& b);
        void addEdge(std::string u, std::string v);
        coord getNodeScore(coord n);
        seedResult getResult();
        bool isVisited(const coord &c);

        void printGraph();
        std::string printGraphAsTable();

        int getNumNodes();
        int getNumVehicles();
        double getTmax();
        void solveProblem();
        double pheromoneCalc(double Tij, double Nij,std::vector<std::pair<std::pair<coord,double>, double>> scores);

        std::vector<std::pair<std::string, coord>> randomSol(unsigned int seed);

        std::vector<std::pair<std::string ,coord>> getNodeCoordSection();
        std::vector<std::pair<std::string ,coord>> getFeasibleNodes();
        std::map<std::string, std::vector<std::pair<std::string, double>>> getAdjList();

        std::map<std::string, coord> getFullCoordInfo();



};

#endif
