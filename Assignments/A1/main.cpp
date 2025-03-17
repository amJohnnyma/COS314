#include "../../Utils/Logger.h"
#include "ProblemInstance.h"
#include "SA.h"
#include <iostream>

std::map<std::string, std::map<double, std::string>> SARuns(int numRuns)
{
  //"
    std::vector<std::string> files = {"8.txt", "12.txt","15.txt", "20.txt", "25.txt"};
    double initTemp = 100;
    double finalTemp = 0.1;
    double coolRate = 0.995;
    int maxIt = 5000;

    std::map<std::string, std::map<double,std::string>> allBestRunsMap;
    std::map<double, std::string> bestRunMap;

    for (auto i:files)
    {
      double bestCost = INFINITY;
        std::string bestRun= "";
        std::string filename = i;
        std::string runData = "";
        ProblemInstance pi(filename);
        
        for(int k =0; k < numRuns; k++)
        {
          unsigned int seed = static_cast<unsigned int>(std::time(0)+k);
          runData = "SA -> File: " + filename + "\tseed: " + std::to_string(seed);         
          
          SA sa(pi, seed);
          std::vector<coord> bestSA = sa.solve(initTemp,finalTemp,coolRate,maxIt);

          if(sa.totalDistance(bestSA) < bestCost)
          {
            bestCost = sa.totalDistance(bestSA);
            bestRun = sa.getPath(bestSA, pi);
          }

        }
        runData += "\tBestRun: " + std::to_string(bestCost) + "\tRun: " + (bestRun);     
        Logger::info(runData, "runData.txt");
        bestRunMap[bestCost] = bestRun;   
        
        allBestRunsMap[filename] = bestRunMap;
    }  

    return allBestRunsMap;
    
}

int main() {
  Logger::info("\n\nStarting program");
  
  std::map<std::string, std::map<double, std::string>> saRuns = SARuns(100);

  


    return 0;
}