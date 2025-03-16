#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "../../Utils/structs.h"
#include "../../Utils/Logger.h"

class ProblemInstance
{
    private:
        std::string name;
        std::string type;
        int dimension;
        std::string edge_weight_type;
        std::vector<coord>* node_coord_section;

    public:
        ProblemInstance(std::string filename);
        ~ProblemInstance();

        std::string getName();
        std::string getType();
        std::string getEdgeWeightType();
        int getDimension();
        std::vector<coord> getNodeCoordSection();

        std::string setName(std::string v);
        std::string setType(std::string v);
        std::string setEdgeWeightType(std::string v);
        int setDimension(int v);
        std::vector<coord> setNodeCoordSection(std::vector<coord> v);



};
