#include <iostream>
#include "ProblemInstance.h"
#include <cstdlib>  // For std::atoi

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
    int count = 0;

    for(const auto&f : filenames)
    {
        param initP = {
            0.1, //pheromones
            0.2, //pheromone imp
            0.4, //heuristic imp
            0.5, //evaporation rate
            2, //Q
            seed, //seed
            30, //max it
            5,//max it no impro
            0.01 //randomly replace a node //1% chance to replace node
        };


        ProblemInstance pb(f, initP);
        pb.solveProblem();
        Logger::info("\n" + pb.printGraphAsTable(), "table"+f);
        //pb.getResults
        //pb.getBestResult
    }

    




    return 0;


}
