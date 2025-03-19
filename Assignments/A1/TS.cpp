#include "TS.h"

TS::TS(ProblemInstance pI, unsigned int seed)
{
    Logger::info("TS starting");
    seed = seed;
    //Logger::info("Creating SA problem " + seed);
    currentSolution = pI.randomSol(seed);

    Logger::info("Seed: " + std::to_string(seed));    
    for (const auto& city : currentSolution) {
        std::string coord = "(" + std::to_string(city.x) + "),(" + std::to_string(city.y) + ")";
        Logger::info(coord);
    }
}

std::vector<coord> TS::solve(int maxIt, double maxTL)
{
    maxTabuListLength = std::round(maxTL+1);

   // double bestDistance = totalDistance(currentSolution);
    Logger::info("Max iterations: " + std::to_string(maxIt));
    Logger::info("Tabu list length: " + std::to_string(maxTabuListLength));
    int maxNoImpro = maxIt/ 5;
    int noImproCount = 0;
    Logger::info("Max it without improvement: " + std::to_string(maxNoImpro));

   // double temp = initTemp;

    int numRuns = 0;

    std::mt19937 gen(seed);

    std::uniform_int_distribution<> dis(0, currentSolution.size() - 1);
    std::uniform_real_distribution<> realDis(0.0, 1.0);
    std::vector<coord> s = currentSolution;

    for(int it =0; it < maxIt && noImproCount < maxNoImpro; it++)
    {
        tabuList.push_back(currentSolution);
        size_t i = dis(gen);
        size_t j = dis(gen);

        while(i==0){i = dis(gen);}
        while(j==0){j = dis(gen);}
        while(i==j) {j = dis(gen); while(j==0){j=dis(gen);}}

       
        //random neighbour s'
        swapCities(currentSolution, i,j);
        int count = std::count(tabuList.begin(), tabuList.end(), currentSolution);
        if(count <= 0)
        {
            if(tabuList.size() > maxTabuListLength)
            {
                tabuList.erase(tabuList.begin());
                
            }

            tabuList.push_back(currentSolution);

            if(totalDistance(currentSolution) < totalDistance(s))
            {
                noImproCount = 0;
                s = currentSolution;
            }
            else{
                noImproCount++;
            }
        }

      numRuns++;

    }

    Logger::info("Iterations used: " + std::to_string(numRuns), "runData.txt");
    return s;
}

void TS::swapCities(std::vector<coord> &route, size_t i, size_t j)
{
    std::swap(route[i], route[j]);
}

double TS::distanceR(const coord &a, const coord &b)
{
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y- b.y, 2));
}

double TS::totalDistance(std::vector<coord> &route)
{
    double distance = 0.0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        distance += distanceR(route[i], route[i + 1]);
    }
    // Add distance back to the start city to complete the cycle
    distance += distanceR(route.back(), route[0]);
    return distance;
}

std::string TS::getPath(std::vector<coord> &route, ProblemInstance pi)
{
    std::string path ="";
    std::map<std::string, std::vector<std::pair<std::string, double>>> adjList = pi.getAdjList();
    std::map<std::string, coord> originalPath = pi.getFullCoordInfo();
    std::string first;

    auto routeBegin = route.begin(); 
    auto routeEnd = route.end();  
    for (auto i = route.begin(); i != routeEnd; ++i) {
     //   bool found = false;
        
        for (auto j : originalPath) {
            if (j.second.x == i->x && j.second.y == i->y) {
                path += j.first;
                if (std::next(i) != routeEnd) {
                    path += "-> ";
                }
              //  found = true;

                if(i == routeBegin)
                {
                    first = j.first;
                }
                break;  
            }
        }     

       
    }

    path += " -> " + first;
    return path;
}
