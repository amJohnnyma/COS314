#include "../../Utils/Logger.h"
#include "ProblemInstance.h"
#include <chrono>
#include "SA.h"
#include "TS.h"
#include <iostream>

std::map<std::map<std::string, unsigned int>, std::map<double, std::string>> SARuns(int numRuns, unsigned int seedIn)
{
  //"
    std::vector<std::string> files = {"8.txt", "12.txt","15.txt", "20.txt", "25.txt"};
    double initTemp = 0.0; // 10x average edge cost
    double finalTemp = 0.1;
    double coolRate = 0.95;
    int maxIt = 5000;

    std::map<std::map<std::string, unsigned int>, std::map<double, std::string>> allBestRunsMap;
    std::map<std::string, unsigned int> bestRunKey;
    std::map<double, std::string> bestRunMap;
    std::vector<double> allRuns;
   

    int tempFact = 50;

    for (auto i:files)
    {
      unsigned int bestSeed;      
      auto start = std::chrono::high_resolution_clock::now();
      allRuns.clear();
      double bestCost = INFINITY;
        std::string bestRun= "";
        std::string filename = i;
        std::string runData = "";
        ProblemInstance pi(filename);

        double sum = 0.0;
        int count =0;
        initTemp = 0.0;
    
        for (const auto& [key, neighbors] : pi.getAdjList()) {
            for (const auto& [neighbor, temperature] : neighbors) {
                sum += temperature;
                count++;
            }
        }
        if(count >0)
            initTemp = sum/count;

        initTemp*=tempFact;
    
        Logger::info("Init temp: " + std::to_string(initTemp), "runData.txt");
        
        for(int k =0; k < numRuns; k++)
        {
          unsigned int seed;
          if(seedIn == 0)
          {
            seed = static_cast<unsigned int>(std::time(0)+k);
          }
          else{
            seed = seedIn;
          }
          runData = "SA -> File: " + filename + "\tseed: " + std::to_string(seed);         
          
          SA sa(pi, seed);
          std::vector<coord> bestSA = sa.solve(initTemp, finalTemp,coolRate,maxIt);

          allRuns.push_back(sa.totalDistance(bestSA));
          if(sa.totalDistance(bestSA) < bestCost)
          {
            bestCost = sa.totalDistance(bestSA);
            bestRun = sa.getPath(bestSA, pi);
            bestSeed = seed;
          }

        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        runData += "\tBestRun: " + std::to_string(bestCost) + "\tRun: " + (bestRun);
        runData += "\nTime taken: " + std::to_string(duration.count());  
        std::string allRunString = "";
        for(auto r:allRuns)
        {
          allRunString += std::to_string(r) + ", ";
        }   
        Logger::info(allRunString, "runData.txt");
        Logger::info(runData+ "\n", "runData.txt");
        bestRunMap[bestCost] = bestRun;  
        
        
        bestRunKey[filename] = bestSeed;
        
        allBestRunsMap[bestRunKey] = bestRunMap;

       

    }  

    return allBestRunsMap;
    
}

std::map<std::map<std::string, unsigned int>, std::map<double, std::string>> TSRuns(int numRuns, unsigned int seedIn)
{
  //"
    std::vector<std::string> files = {"8.txt", "12.txt","15.txt", "20.txt", "25.txt"};
    int maxIt = 5000;
    double tabuLength = 0;


    std::map<std::map<std::string, unsigned int>, std::map<double, std::string>> allBestRunsMap;
    std::map<std::string, unsigned int> bestRunKey;
    std::map<double, std::string> bestRunMap;
    std::vector<double> allRuns;

    for (auto i:files)
    {
      unsigned int bestSeed;
      tabuLength = 0;
      auto start = std::chrono::high_resolution_clock::now();
      allRuns.clear();
      double bestCost = INFINITY;
        std::string bestRun= "";
        std::string filename = i;
        std::string runData = "";
        ProblemInstance pi(filename);
        tabuLength = pi.getDimension() / 2;
        Logger::info("Tabu list length: " + std::to_string(tabuLength+1), "runData.txt");


        for(int k =0; k < numRuns; k++)
        {
          unsigned int seed;
          if(seedIn == 0)
          {
            seed = static_cast<unsigned int>(std::time(0)+k);
          }
          else{
            seed = seedIn;
          }
          runData = "TS -> File: " + filename + "\tseed: " + std::to_string(seed);  
          
          
          
          TS ts(pi, seed);
          std::vector<coord> bestTS = ts.solve(maxIt, tabuLength);

          allRuns.push_back(ts.totalDistance(bestTS));
          if(ts.totalDistance(bestTS) < bestCost)
          {
            bestCost = ts.totalDistance(bestTS);
            bestRun = ts.getPath(bestTS, pi);
            bestSeed = seed;
          }

        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        runData += "\tBestRun: " + std::to_string(bestCost) + "\tRun: " + (bestRun);     
        runData += "\nTime taken: " + std::to_string(duration.count());  
        std::string allRunString = "";
        for(auto r:allRuns)
        {
          allRunString += std::to_string(r) + ", ";
        }   
        Logger::info(allRunString, "runData.txt");
        Logger::info(runData + "\n", "runData.txt");


        bestRunMap[bestCost] = bestRun;          
        
        bestRunKey[filename] = bestSeed;
        
        allBestRunsMap[bestRunKey] = bestRunMap;
    }  

    return allBestRunsMap;
    
}


int main(int argc, char* argv[]) {
    Logger::info("\n\nStarting program");
    Logger::info("Starting main", "runData.txt");

    unsigned int seed = 0; // Default seed
    int runs = 10; // Default number of runs

    if (argc > 1) {  // Command-line mode
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "--seed" && i + 1 < argc) {
                seed = std::stoi(argv[++i]);
                Logger::info("Seed: " + std::to_string(seed), "runData.txt");
            } else if (arg == "--runs" && i + 1 < argc) {
                runs = std::stoi(argv[++i]);
                Logger::info("Num runs: " + std::to_string(runs), "runData.txt");
            }
        }
    } else {  // Interactive Mode
        std::cout << "Enter seed (0 for random, any seed will be 1 run): ";
        std::cin >> seed;
        Logger::info("Seed: " + std::to_string(seed), "runData.txt");

        if(seed == 0)
        {
          std::cout << "Enter number of runs: ";
          std::cin >> runs;
          Logger::info("Num runs: " + std::to_string(runs), "runData.txt");
        }
   
    }

    // Set seed if not provided
    if (seed == 0) {
        seed = static_cast<unsigned int>(std::time(0));
    }

        auto saRuns = SARuns(runs, seed);
        auto tsRuns = TSRuns(runs, seed);

    return 0;
}

