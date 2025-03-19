#include "SA.h"
#include <algorithm>
void SA::swapCities(std::vector<coord>& route, size_t i, size_t j)
{
    std::swap(route[i], route[j]);
}
double SA::distanceR(const coord &a, const coord &b)
{
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y- b.y, 2));
}

double SA::totalDistance(std::vector<coord> &route)
{
    double distance = 0.0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        distance += distanceR(route[i], route[i + 1]);
    }
    // Add distance back to the start city to complete the cycle
    distance += distanceR(route.back(), route[0]);
    return distance;
}

std::string SA::getPath(std::vector<coord> &route, ProblemInstance pi)
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
             //   found = true;

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

SA::SA(ProblemInstance pI, unsigned int seed)
{
    Logger::info("SA starting");
    seed = seed;
    //Logger::info("Creating SA problem " + seed);
    currentRoute = pI.randomSol(seed);
    bestRoute = currentRoute;



    Logger::info("Seed: " + std::to_string(seed));    
    for (const auto& city : currentRoute) {
        std::string coord = "(" + std::to_string(city.x) + "),(" + std::to_string(city.y) + ")";
        Logger::info(coord);
    }
    
}


std::vector<coord> SA::solve(double initTemp, double finTemp, double coolRate, int maxIt)
{
    double bestDistance = totalDistance(bestRoute);
    int maxNoImpro = maxIt/ 20;
    Logger::info("Max it without improvement: " + std::to_string(maxNoImpro));

    double temp = initTemp;
    
    int noImproCount = 0;

    int numRuns = 0;

    std::mt19937 gen(seed);

    std::uniform_int_distribution<> dis(0, currentRoute.size() - 1);
    std::uniform_real_distribution<> realDis(0.0, 1.0);

    //seed gen
    for(int it = 0; it < maxIt && temp > finTemp && noImproCount < maxNoImpro; it++)
    {
        numRuns++;
        size_t i = dis(gen);
        size_t j = dis(gen);

        while(i==0){i = dis(gen);}
        while(j==0){j = dis(gen);}
        while(i==j) {j = dis(gen); while(j==0){j=dis(gen);}}

        
        double oldDistance = totalDistance(currentRoute);

        //NEXT <- randomly chosen adjacent state to current
        swapCities(currentRoute, i, j);             
        
        double newDist = totalDistance(currentRoute);

        //^L <- L(next) - L(current)
        double delta = newDist - bestDistance;

        //if ^L < 0, and boltzmann factor
        if(delta < 0 || realDis(gen) < std::min(1.00, std::exp(-delta / temp)))
        {
            if(newDist < bestDistance)
            {
                bestRoute = currentRoute;
                bestDistance = newDist;
                noImproCount = 0;
            }
            else{
                noImproCount ++;
            }
        }
        else{
            //swap back if no improvement
            swapCities(currentRoute, i, j);
            noImproCount++;
        }
        
        temp = initTemp / std::log(it+1);
      //  temp*=coolRate;

    }

    Logger::info("Iterations used: " + std::to_string(numRuns));
    Logger::info("Iterations used: " + std::to_string(numRuns), "runData.txt");
    return bestRoute;
}



