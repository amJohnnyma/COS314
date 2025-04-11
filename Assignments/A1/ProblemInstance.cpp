#include "ProblemInstance.h"



ProblemInstance::ProblemInstance(std::string filename)
{
    Logger::info("Constructing problem instance fn: " + filename);

    std::fstream file("Assignments/A1/Data/" + filename);

    if(!file.is_open())
    {
        Logger::error("Could not open file");

        return;
    }

    std::string line;
    bool readingCoords = false;

    while(std::getline(file, line))
    {

        Logger::info(line);
        std::istringstream iss(line);
        std::string kw;
        iss >> kw;

      //  std::cout << kw << std::endl;

        if(kw == "NAME:")
        {
            iss >> name;
        }   
        if(kw == "TYPE:")
        {
            iss >> type;

        }  
        if(kw == "DIMENSION:")
        {
            iss >> dimension;

            std::ostringstream d;
            d << dimension;
            std::istringstream(d.str()) >> dimenInt;

        }  
        if(kw == "EDGE_WEIGHT_TYPE:")
        {
            iss >> edge_weight_type;

        }  
        if(kw == "NODE_COORD_SECTION")
        {
            readingCoords = true;
            continue;
        }  
        if(kw == "EOF:")
        {
            readingCoords = false;
            continue;
        } 
        if(readingCoords)
        {
            std::string id;
            double x, y;
            id = (kw);
            iss >> x >> y;
            if(id != "EOF")
            {
          //      std::cout << id << ": " << x << ", " << y << std::endl;
            addCoord(id, x, y);
            }
            
        }
       
    }

    Logger::info("Dimension: " + dimension);
    Logger::info("Adding edges");
    for (int i = 1; i <= dimenInt; i++) {
        for (int j = i + 1; j <= dimenInt; j++) {
            std::ostringstream is;
            std::ostringstream js;
            is << i;
            js << j;
            std::string iStr = is.str();
            std::string jStr = js.str();
           // Logger::info(iStr + ": " + jStr);
            addEdge(iStr, jStr);
        }
    }

}

ProblemInstance::~ProblemInstance()
{
   
}

void ProblemInstance::addCoord(std::string id, double x, double y)
{
    coord n;
    n.name = id;
    n.x = x;
    n.y = y;
    node_coord_section[id] = n;
    Logger::info("Added cord " + id);
}

double ProblemInstance::distance(const coord &a, const coord &b)
{
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y- b.y, 2));
}

void ProblemInstance::addEdge(std::string u, std::string v)
{
    double dist = distance(node_coord_section[u], node_coord_section[v]);
    adjList[u].push_back({v, dist});
    adjList[v].push_back({u, dist});
}

void ProblemInstance::printGraph()
{
    Logger::info("Printing graph");
    for(const auto& [id, neighbours] : adjList)
    {
        Logger::info("Coord " + id);
        auto coordIt = node_coord_section.find(id);
        if (coordIt != node_coord_section.end()) {
            const coord& c = coordIt->second;
            Logger::info("Coord " + id + " (" + std::to_string(c.x) + ", " + std::to_string(c.y) + ")");
        } else {
            Logger::info("Coord not found for id " + id);
        }
        
        for(const auto& [neighbour, weight] : neighbours)
        {
            Logger::info("-> " + neighbour);       
            std::ostringstream ws;
            ws << weight;   
            Logger::info("Distance: " + std::to_string(weight));
        }
        Logger::info("\n");
        
    }
    
}

std::string ProblemInstance::getName()
{
    return name;
}

std::string ProblemInstance::getType()
{
    return type;
}

std::string ProblemInstance::getEdgeWeightType()
{
    return edge_weight_type;
}

int ProblemInstance::getDimension()
{
    return dimenInt;
}

std::vector<coord> ProblemInstance::getNodeCoordSection()
{
       
    std::vector<coord> coords;

    for (const auto& entry : node_coord_section) {
        coords.push_back(entry.second);  
    }
    //std::reverse(coords.begin(), coords.end());
    return coords;
}

std::vector<coord> ProblemInstance::randomSol(unsigned int seed)
{
    Logger::info("Creating random solution");
    Logger::info("Retrieving route");
    std::vector<coord> route = getNodeCoordSection();
    Logger::info("Generating seed");
    std::mt19937 gen(seed);
    Logger::info("shuffling route");
    std::shuffle(route.begin()+1, route.end(), gen);
    Logger::info("returning route");
    return route;
}

std::map<std::string, std::vector<std::pair<std::string, double>>> ProblemInstance::getAdjList()
{
    return adjList;
}

std::map<std::string, coord> ProblemInstance::getFullCoordInfo()
{
    return node_coord_section;
}
