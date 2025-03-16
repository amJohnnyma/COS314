#include "ProblemInstance.h"

ProblemInstance::ProblemInstance(std::string filename)
{
    Logger::info("Constructing problem instance fn: " + filename);
    node_coord_section = new std::vector<coord>;

    std::fstream file("Assignments/A1/Data/" + filename);

    if(!file.is_open())
    {
        Logger::error("Could not open file");

        return;
    }

    std::string line;

    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string kw;
        iss >> kw;

        Logger::info(kw);
    }

}

ProblemInstance::~ProblemInstance()
{
    delete node_coord_section;
}

std::string ProblemInstance::getName()
{
    return std::string();
}

std::string ProblemInstance::getType()
{
    return std::string();
}

std::string ProblemInstance::getEdgeWeightType()
{
    return std::string();
}

int ProblemInstance::getDimension()
{
    return 0;
}

std::vector<coord> ProblemInstance::getNodeCoordSection()
{
    return std::vector<coord>();
}

std::string ProblemInstance::setName(std::string v)
{
    return std::string();
}

std::string ProblemInstance::setType(std::string v)
{
    return std::string();
}

std::string ProblemInstance::setEdgeWeightType(std::string v)
{
    return std::string();
}

int ProblemInstance::setDimension(int v)
{
    return 0;
}

std::vector<coord> ProblemInstance::setNodeCoordSection(std::vector<coord> v)
{
    return std::vector<coord>();
}
