#include <iostream>
#include "ProblemInstance.h"

int main() {
    // Create the problem instance from the input file
    
    ProblemInstance pb("p3.4.s.txt");

    // Call getAdjList and do something with the result
   // Logger::info('\n'+pb.printGraphAsTable(), "table1.txt");
    pb.solveProblem();




    return 0;


}
