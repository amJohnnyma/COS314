#include "ProblemInstance.h"



ProblemInstance::ProblemInstance(std::string filename)
{
    Logger::info("Constructing problem instance: " + filename,debugfile);

    std::fstream file("Assignments/A2/Data_TOP/" + filename);

    if(!file.is_open())
    {
        Logger::error("Could not open file",debugfile);

        return;
    }

    std::string line;
    bool first = true;

    while(std::getline(file, line))
    {
      //  Logger::info(line, debugfile);
        
        // Replace tabs with spaces to ensure proper parsing
        std::replace(line.begin(), line.end(), '\t', ' ');  // Convert all tabs to spaces

        std::istringstream iss(line);
        std::string kw;
        iss >> kw;
    
        if(kw == "n")
        {
            iss >> numNodes;
            continue;
        } 
        else if(kw == "m")
        {
            iss >> numVehicles;
            continue;
        } 
        else if(kw == "tmax")
        {
            iss >> tMaxString;
            std::istringstream(tMaxString) >> tmax;  
            continue;
        }  
        else
        {
        // Manually search for floating-point numbers (x, y, and score)
        std::regex rgx("([+-]?[0-9]+(?:\\.[0-9]+)?)");
        std::smatch match;
        std::string temp = line;

        // Find all matches for decimal numbers in the line
        std::vector<double> numbers;
        while (std::regex_search(temp, match, rgx))
        {
         //   Logger::info(temp,debugfile);
            numbers.push_back(std::stod(match.str()));
            temp = match.suffix().str();  // Move to the next part of the string
        }

       // Logger::info(numbers.size(), debugfile);
        if (numbers.size() == 3)  // We expect exactly three numbers (x, y, score)
        {
            double x = numbers[0];
            double y = numbers[1];
            int score = numbers[2];
          //  Logger::info(coord(score,x,y).to_string(),debugfile);
            addCoord(x, y, score); 

            if(first)
            {
                depot = coord(score, x, y);
                first = false;
            }
        }
        else
        {
            Logger::error("Invalid coordinate/score format in line: '" + line + "'", debugfile);
          //  std::cerr << "Error: Invalid coordinate/score format in line: " << line << std::endl;
        }
        }
    }

    Logger::info("Adding edges",debugfile);
    for (int i = 1; i <= numNodes; i++) {
        for (int j = i + 1; j <= numNodes; j++) {
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

void ProblemInstance::addCoord(double x, double y, int score)
{
    coord n;
    n.score = score;
    n.x = x;
    n.y = y;
    node_coord_section[std::to_string(id)] = n;
    id++;
    Logger::info("Added cord\t" + n.to_string(), debugfile);
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
    Logger::info("Added edge: " + u + "->" + v + ": " + std::to_string(dist),debugfile);
}

void ProblemInstance::printGraph()
{
    Logger::info("Printing graph",debugfile);
    for(const auto& [id, neighbours] : adjList)
    {
        Logger::info("Coord " + id,debugfile);
        auto coordIt = node_coord_section.find(id);
        if (coordIt != node_coord_section.end()) {
            const coord& c = coordIt->second;
            Logger::info("Coord " + id + " (" + std::to_string(c.x) + ", " + std::to_string(c.y) + ")", debugfile);
        } else {
            Logger::info("Coord not found for id " + id,debugfile);
        }
        
        for(const auto& [neighbour, weight] : neighbours)
        {
            Logger::info("-> " + neighbour,debugfile);       
            std::ostringstream ws;
            ws << weight;   
            Logger::info("Distance: " + std::to_string(weight),debugfile);
        }
        Logger::info("\n",debugfile);
        
    }
    
}

std::string ProblemInstance::printGraphAsTable()
{
    std::ostringstream oss;
    std::set<std::string> nodes;

    // Gather all unique nodes
    for (const auto& [from, neighbors] : adjList) {
        nodes.insert(from);
        for (const auto& [to, _] : neighbors)
            nodes.insert(to);
    }

    std::vector<std::string> nodeList(nodes.begin(), nodes.end());
    const int cellWidth = 10;
    const double INF = std::numeric_limits<double>::infinity();

    // Build lookup matrix
    std::map<std::string, std::map<std::string, double>> matrix;
    for (const auto& [from, neighbors] : adjList) {
        for (const auto& [to, cost] : neighbors) {
            matrix[from][to] = cost;
        }
    }

    auto drawSeparator = [&]() {
        oss << '+';
        for (size_t i = 0; i <= nodeList.size(); ++i)
            oss << std::string(cellWidth, '-') << '+';
        oss << '\n';
    };

    // Header row
    drawSeparator();
    oss << "|" << std::setw(cellWidth) << "" << "|";
    for (const auto& to : nodeList)
        oss << std::setw(cellWidth) << to << "|";
    oss << '\n';
    drawSeparator();

    // Matrix body
    for (const auto& from : nodeList) {
        oss << "|" << std::setw(cellWidth) << from << "|";
        for (const auto& to : nodeList) {
            if (from == to)
                oss << std::setw(cellWidth) << "0.0" << "|";
            else if (matrix[from].count(to))
                oss << std::setw(cellWidth) << matrix[from][to] << "|";
            else
                oss << std::setw(cellWidth) << "INF" << "|";
        }
        oss << '\n';
        drawSeparator();
    }

    return oss.str();
}

int ProblemInstance::getNumNodes()
{
    return numNodes;
}

int ProblemInstance::getNumVehicles()
{
    return numVehicles;
}

double ProblemInstance::getTmax(){
    return tmax;
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
    Logger::info("Creating random solution",debugfile);
    Logger::info("Retrieving route",debugfile);
    std::vector<coord> route = getNodeCoordSection();
    Logger::info("Generating seed",debugfile);
    std::mt19937 gen(seed);
    Logger::info("shuffling route",debugfile);
    std::shuffle(route.begin()+1, route.end(), gen);
    Logger::info("returning route",debugfile);
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
