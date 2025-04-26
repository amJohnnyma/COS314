#include <iostream>
#include "ProblemInstance.h"
#include <cstdlib>  // For std::atoi

unsigned int generateSeed() {
    std::random_device rd; // non-deterministic random number generator
    auto timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    auto pidSeed = static_cast<unsigned int>(getpid());
    auto rdSeed = static_cast<unsigned int>(rd());

    // Combine using XOR and bit shifting to improve mixing
    unsigned int seed = static_cast<unsigned int>(timeSeed) ^ (rdSeed << 1) ^ (pidSeed << 2);
    return seed;
}

void parameterTuning(std::vector<std::string> f)
{
std::vector<double> alphaValues = {0.1, 0.2, 0.5, 1.0};
std::vector<double> betaValues = {0.1, 0.3, 0.6, 1.0};
std::vector<double> evaporationRates = {0.3, 0.5, 0.7};
std::vector<int> qValues = {1, 2, 5};

for(auto &filename : f)
{    
    std::vector<TuneResult> tuningResults;
  //  for(int k =0; k < 10;k++)
    {
        for (double alpha : alphaValues) {
            for (double beta : betaValues) {
                for (double evap : evaporationRates) {
                    for (int q : qValues) {
        
                        param p = {
                            0.1,        
                            alpha,
                            beta,
                            evap,
                            q,
                            generateSeed(),
                            10,
                            4,
                            0.05
                        };
        
                        
                        ProblemInstance pb(filename, p);
                        pb.solveProblem();
                        seedResult r = pb.getResult();
                        TuneResult tr =
                        {
                            p,
                            r.cost,
                            r.runtime
                        };
        
                        tuningResults.push_back(tr);
        
        
                    }
                }
            }
        }       
        

    }
    std::sort(tuningResults.begin(), tuningResults.end(), [](const TuneResult& a, const TuneResult& b) {
        return a.score > b.score; // or < for minimization
    });
    
    Logger::info("TOP 5 RESULTS: " + filename, "ParamTuning.txt");
    int limit = std::min(5, static_cast<int>(tuningResults.size()));
    for (int i = 0; i < limit; ++i) {
        auto& tr = tuningResults[i];
        Logger::naked("Params: " + tr.p.to_string() +
                      "\tScore: " + std::to_string(tr.score) +
                      "\tRuntime: " + std::to_string(tr.runtime.count()),
                      "ParamTuning.txt");
    }
    
}




}

void piRuns(unsigned int seed)
{
    std::vector<std::string> filenames = {
        "p3.2.a.txt",
        "p3.2.c.txt",
        "p3.3.a.txt",
        "p3.3.c.txt",
        "p3.4.s.txt",
        "p3.4.t.txt",
        "p7.2.a.txt",
        "p7.2.m.txt",
        "p7.3.h.txt",
        "p7.4.g.txt",
        "p7.4.q.txt"
    };


    std::vector<allResult> aR;    
    for (const auto& f : filenames)
    { 
        std::vector<seedResult> results;
    
        for (int k = 0; k < 10; k++)
        {
            param initP = {
                0.1, //pheromones
                0.525, //pheromone imp
                0.775, //heuristic imp
                0.5, //evaporation rate
                1.5,   //Q
                generateSeed(), // seed generated per run
                10,  // max it
                4,   // max it no impro
                0.1 // random replacement
            };
    
            ProblemInstance pb(f, initP);
            pb.solveProblem();    
          //  Logger::info("\n" + pb.printGraphAsTable(), "table" + f);
            results.push_back(pb.getResult());
        }
    
        allResult a;
        a.name = f; // use file name instead of seed if you want clarity
        a.results = results;
    
        aR.push_back(a);
    }

    int count = 0;
    seedResult bestR;
    bestR.duration = 0;
    for(const auto & ar : aR)
    {
        Logger::naked("---------------------------------------------------------------","results.txt");
        count++;
        double totalCost = 0;
        double totalDuration = 0;
        std::chrono::duration<double> totalRuntime;
        for(const auto & r : ar.results)
        {        
            seedResult t = r; 
            Logger::naked
            (
                std::to_string(count)
                + "\t"
                + filenames[count-1]
                + "\t"
                + std::to_string(r.seed)
                + "\t"
                + t.to_string()
                + "\t"
                + std::to_string(t.cost)
                +"\t"
                + std::to_string(t.duration)
                +"\t"
                +  std::to_string(t.runtime.count())
                +"\t"
                +std::to_string(t.iterations)
                , "results.txt");

                totalCost += t.cost;
                totalDuration += t.duration;
                totalRuntime += t.runtime;

        }
        Logger::naked
        (
            std::to_string(count)
            + "\t"
            + filenames[count-1]
            + "\t"
            + ar.name
            + "\tTC"
            + std::to_string(totalCost)
            + "\tTD"
            + std::to_string(totalDuration)
            +"\tTR"
            + std::to_string(totalRuntime.count())
            , "results.txt");

    }
}

int main(int argc, char* argv[]) {    // Create the problem instance from the input file
    
    int seed = 0;

    if (argc >= 2) {
        seed = std::atoi(argv[1]);
    }

    std::vector<std::string> filenames = {
        "p3.2.a.txt",
        "p3.2.c.txt",
        "p3.3.a.txt",
        "p3.3.c.txt",
        "p3.4.s.txt",
        "p3.4.t.txt",
        "p7.2.a.txt",
        "p7.2.m.txt",
        "p7.3.h.txt",
        "p7.4.g.txt",
        "p7.4.q.txt"
    };
   // parameterTuning(filenames);
    piRuns(seed);
    
    return 0;
}


   



    

