#include "ProblemInstance.h"



ProblemInstance::ProblemInstance(std::string filename, param params)
{
    Logger::info("Constructing problem instance: " + filename,debugfile);

    std::fstream file("Assignments/A2/Data_TOP/" + filename);

    if(!file.is_open())
    {
        Logger::error("Could not open file",debugfile);

        return;
    }
    imageName = filename;
    this->params = params;
    std::string line;
    bool first = true;

    while(std::getline(file, line))
    {
        Logger::info(line, debugfile);
        
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
          //  if( x != 0 && y != 0)
                addCoord(x, y, score); 

            if(first)
            {
                depot = {std::to_string(id), coord(score, x, y)};
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
          // if (iStr != "0" && jStr != "0")
                addEdge(iStr, jStr);
        }
    }

    //Do seed
    if(params.seed == 0) //random seed
    {   
        auto timeSeed = std::chrono::system_clock::now().time_since_epoch().count();

        // Combine with process ID to further differentiate the seeds
        auto pidSeed = getpid();
    
        // Combine time and process ID to create a unique seed
        auto seed = timeSeed ^ pidSeed;
        rng.seed(seed);
    }
    else
    {
        rng.seed(params.seed);
    }
    

    params.maxIt *= numVehicles;
    params.maxIt *= numNodes;
    params.maxItNoImpro *= numVehicles;
    params.maxItNoImpro *= numNodes;

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
    
    Logger::info("Added cord\t" + n.to_string() + "\tID: " + std::to_string(id), debugfile);
    
    id++;
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
   // Logger::info("Added edge: " + u + "->" + v + ": " + std::to_string(dist),debugfile);
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
bool areCoordsEqual(const coord& a, const coord& b, double epsilon = 1e-4) {
    return std::abs(a.x - b.x) < epsilon && std::abs(a.y - b.y) < epsilon;
}
std::vector<std::pair<std::string, coord>> ProblemInstance::getFeasibleNodes()
{
    std::vector<std::pair<std::string ,coord>> nodes = getNodeCoordSection();
    for(auto it = nodes.begin(); it != nodes.end();)
    {
        bool isVisited = std::any_of(visited.begin(), visited.end(), [&](const coord& v) {
            return areCoordsEqual(it->second, v);
        });

        if (isVisited)
        {
            it = nodes.erase(it);
        }
        else
        {
            ++it;
        }
    }
    return nodes;
}
coord ProblemInstance::getNodeScore(coord n)
{

  //  double rscore = 0;
    //should be feasible nodes
    for(const auto &entry : node_coord_section)
    {
        if(areCoordsEqual(n,entry.second))
        {
        coord c = entry.second;
      //  Logger::info("Pushed back: " + c.to_string(), "erh.txt");
         return c;
        }
    }
    return coord();
}
bool ProblemInstance::isVisited(const coord& c) {
    return std::any_of(visited.begin(), visited.end(),
        [&](const coord& v) {
            return areCoordsEqual(v, c);
        });
}
double ProblemInstance::pheromoneCalc(double Tij, double Nij,std::vector<std::pair<std::pair<coord,double>, double>> scores)
{
    double top = std::pow(Nij, params.pheromoneImportance) * std::pow(Tij, params.heuristicImportance);
    double bottom = 0.0;
    for(auto & i : scores)
    {
      //  Logger::info("i.first.second: " + std::to_string(i.first.second), "wfge.txt");
        if(std::isnan(i.first.second))
        {
            i.first.second = i.second;
        }
       // Logger::info("i.second: " + std::to_string(i.second), "wfge.txt");
        bottom += (std::pow(i.first.second, params.heuristicImportance) * std::pow(i.second, params.pheromoneImportance));
    }

    return top / bottom;
}
bool edgeExists(
    const std::vector<std::pair<double, std::pair<coord, sf::Vertex>>>& alledges,
    const std::pair<double, std::pair<coord, sf::Vertex>>& candidate,
    double epsilon = 1e-4
) {
    for (const auto& e : alledges) {
        // Check if coord matches (score, x, y)
        const coord& a = e.second.first;
        const coord& b = candidate.second.first;
        if(areCoordsEqual(a,b))
        {
            return true;
        }
    }
    return false;
}

void ProblemInstance::solveProblem()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "ACO Visualizer");

    std::vector<std::pair<std::string, coord>> nodes = getNodeCoordSection();    //start with all nodes and edges available

//init///////////////////////////////////
    std::vector<sf::CircleShape> nCircles;
    std::vector<std::pair<std::string, coord>> initPath = {{depot}, {depot}};
    for(int i =0; i < numVehicles; i++)
    {
        vehicle v;
        v.tmax = tmax;
        v.path = initPath;
        v.score = 0;
        vehicles.push_back(v);
    }



    std::map<std::string, coord> nodeCoordMap;
    std::set<std::pair<std::string, std::string>> drawn;

    for(const auto& n : nodes)
    {
       // Logger::info("Node: " + n.first, "solveProblem.txt");
        sf::CircleShape ns(std::max(1, n.second.score/10));
        ns.setFillColor(sf::Color::Green);
        
        ns.setPosition(n.second.x, n.second.y);
        nCircles.push_back(ns);       

    }


    for(const auto& i : nodes)
    {
        coord c = i.second;
      //  Logger::info("Map Node: " + i.first + " = " + c.to_string(), "solveProblem.txt");
        nodeCoordMap[i.first] = i.second;
    }
 




    for(const auto & [id ,otherNode] : adjList)
    {      
        //id, vector<{id , distance}> 
        //adjlist
        //fromCoord, vector<{pheromone, {toCoord, Vertex}}
        //        std::vector<std::pair<coord, std::vector<std::pair<double, std::pair<coord, sf::Vertex>>>>> alledges;
        coord fromCoord = nodeCoordMap[id];
        std::pair<coord, std::vector<std::pair<double, std::pair<coord, sf::Vertex>>>> newEdges;
        std::vector<std::pair<double, std::pair<coord, sf::Vertex>>> connEdges = {};
        for(const auto & [nid, distance]:  otherNode)
        {
           // std::vector<std::pair<double, std::pair<coord, sf::Vertex>>> connEdges;
            coord nCoord = nodeCoordMap[nid];
            std::pair<double, std::pair<coord, sf::Vertex>> newEdge = {params.pheromones, {nCoord, sf::Vertex(sf::Vector2f(nCoord.x, nCoord.y), sf::Color::Red)}};
            connEdges.push_back(newEdge);
        }
        newEdges.first = fromCoord;
        newEdges.second = connEdges;
        alledges.push_back(newEdges);     
        



    }


    auto start = std::chrono::high_resolution_clock::now();
    int iterations = 0;
    while (window.isOpen()) {
       
       handleEvents(window);
       
        if(params.maxIt > iterations && (maxItNoImpro) < params.maxItNoImpro) 
        {
            update(nCircles);
            render(window, nCircles);
          //  sf::sleep(sf::milliseconds(1000));
            iterations ++;
        }
        else{
            auto end = std::chrono::high_resolution_clock::now();
            sf::Texture texture;
            texture.create(window.getSize().x, window.getSize().y);
            texture.update(window);

            sf::Image screenshot = texture.copyToImage();
            if (imageName.size() >= 4 && imageName.substr(imageName.size() - 4) == ".txt") {
                imageName = imageName.substr(0, imageName.size() - 4) + ".png";
            }            
            screenshot.saveToFile(imageName);

            result.seed = params.seed;
            double bestScore = 0;
            double bestDuration = 0;
            std::vector<coord> bestPath;
            for(auto & v : vehicles)
            {
                if(v.score > bestScore)
                {
                    bestScore = v.score;
                    for(auto & p : v.path)
                    {
                        bestPath.push_back(p.second);
                    }
                    bestDuration = v.travelDistance;
                }

            }
            result.cost = bestScore;
            result.solution = bestPath;
            result.duration = bestDuration;
            result.runtime = (start - end);
            result.iterations = runNum;
            

            window.close();
        }
        
        
    }
}

void ProblemInstance::handleEvents(sf::RenderWindow& window)
{    
    sf::Event event;
    while (window.pollEvent(event))
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }


}

double ProblemInstance::getBestScore()
{
    double bestScore = 0;
    for(auto & v : vehicles)
    {
        if(v.score > bestScore)
        {
            bestScore = v.score;
        }
    }
    return bestScore;
}

seedResult ProblemInstance::getResult()
{
    return result;
}

void ProblemInstance::update(std::vector<sf::CircleShape> &nCircles)
{   
    // Define RNG
    std::mt19937 rng(params.seed); // seed is an int

    // Example usage
    std::uniform_real_distribution<> dist(0.0, 1.0);
    double r = dist(rng); // random number between 0 and 1
    std::string update = "update.txt";
  //  Logger::info("Updating...", update);
    std::vector<std::pair<std::string, coord>> nodes;    
   // edges = {};
    //rather retrieve "best solution" and edit that instead of nodes
    
    
    //for each vehicle calculate the scores
    for(auto & v : vehicles)
    {

        Logger::info("Vehicle: ", update);
        Logger::info(v.to_string(), update);

        nodes = getFeasibleNodes(); //make sure cant go to visited nodes

        bool atDepot = true;
        std::vector<std::pair<std::string, coord>> curPath = v.path;
        curPath.pop_back();
        coord selected;
        //the score when travelling to a coord
        std::vector<std::pair<std::pair<coord,double>, double>> scores = {};
        std::vector<std::pair<coord, double>> calculatedScore = {};
        //check edge vertexes to get pheromone value of a node
        auto cp = curPath.back();
        {
        //    Logger::info("Checking current path: " + cp.second.to_string(), update);
            //if we are at the depot check if we are starting or ending
            if(cp.second == depot.second)
            {
                if(curPath.size() > 1) // We want to go to the next node and process that instead
                {
                    continue;
                }
             //   Logger::info("At a depot", update);
                if(atDepot)
                {
             //       Logger::info("First depot", update);
                    atDepot = false; //Going to move away from depot

                }
                else{
                    //We have finished the path checking
              //      Logger::info("Second depot", update);
                    break;
                }
            }
            else{
          //      Logger::info("Not at depot", update);
        double randVal = dist(rng);
        if(randVal < params.newPathCoeff)
        {
        Logger::info("Node replace", update);
        
        
        for(auto & c : curPath)
        {
            visited.erase(std::remove(visited.begin(), visited.end(), c.second), visited.end());
        }
        if (curPath.size() > 1) {curPath.erase(curPath.begin() + 1, curPath.end());}
            
        
        v.score = 0;
            for(int i = 0; i < runNum; i ++)
            {
                std::uniform_real_distribution<> dist(0.0, 1.0);
                double randVal = dist(rng);
                double cumulative = 0.0;
                for(const auto& p : calculatedScore)
                {
                    cumulative += p.second;
                    if(randVal <= cumulative)
                    {
                    //    Logger::info("Randomly chose score ", update);
                        //select this node to use
                        //add this node
                        //update pheromones
                        visited.push_back(p.first);
                        coord added = p.first;
                        Logger::info("Adding to visited " +added.to_string(), update);
        
                        v.travelDistance += distance(selected, p.first) + distance(p.first,depot.second);
                        v.score += p.first.score;
        
                        std::string id = "";
                        for (const auto& node : nodes) {
                            if (node.second == p.first) {
                                id = node.first;
                                break;
                            }
                        }
                        curPath.push_back({id, added});
                        
        
                        //update pheromones for p.first
                        for(int ec = 0; ec < alledges.size(); ec++)
                        {
                            /*
                            Connected edges:
                            0-1
                            2-3
                            4-5  
                            */
                            coord edgePos(alledges[ec].first);
                            for(auto & eu : alledges[ec].second)
                            {
                            if(areCoordsEqual(eu.second.first,added))
                            {
                                eu.first = (params.Q*(eu.first)/p.second);
                                continue;
                            }
                            eu.first *= (1-params.evaporationRate);
        
                            }   
        
                            
                            
                        }                
                        changes = true;
                        //curPath.push_back(depot);
                        v.path = curPath;
                        break;
                    }
                    }
            
            
        }
            continue;
        }
        }
            selected = cp.second;
        //    Logger::info("Selected " + selected.to_string() , update);
            //now go through nodes and calculate scores
            //Start at cp (current node)
            //compare to the edges
            for(int ec = 0; ec < alledges.size(); ec++)
            {
                /*
                Connected edges:
                0-1
                2-3
                4-5  
                
                */               
                //the vertex of the edge (a vertex matches then the edge is connected)
                        //        std::vector<std::pair<coord, std::vector<std::pair<double, std::pair<coord, sf::Vertex>>>>> alledges;

                coord edgePos(alledges[ec].first);
                coord destinationEdge;
            //   Logger::info("edgepos: " + edgePos.to_string(), update);

                if(areCoordsEqual(edgePos, selected))
                {
                //    Logger::info("Matching edge for " + selected.to_string(), update);

                    for(auto & destEdge : alledges[ec].second)
                    {
                        destinationEdge = destEdge.second.first;
                        if(areCoordsEqual(destinationEdge, depot.second) && atDepot)
                        {
                   //         Logger::info("At last depot", update);
                            continue;
                        }
                        else{
                    //        Logger::info("At first depot finding edges", update);
                        }
                        //We are connected to this edge from cp so calculate heuristic
                        //We can still travel here
                        //Vehicles total travel cant exceed tmax
                        // depot -> nodes -> depot
                        // total distance travelled + distance to node + distance to depot from next node <= tmax
                        if((v.travelDistance + distance(destinationEdge,cp.second) + distance(destinationEdge, depot.second)) <=tmax && !isVisited(destinationEdge))
                        {
                       //   Logger::info("Can travel to edge ", update);

                            //find the nodes score
                            double dist = distance(destinationEdge, cp.second);
                            if(dist == 0 || std::isnan(dist))
                            {
                                dist = distance(cp.second, depot.second);
                            }
                            double bot = (destinationEdge.score / dist);
                            if(bot==0)
                            {
                                bot = 1;
                            }
                            std::pair<std::pair<coord,double>, double> thisScore = {
                                {destinationEdge, bot},
                                    destEdge.first                            
                            };
                            scores.push_back(thisScore);
                        //    Logger::info("Pushed score: " + thisScore.first.first.to_string(), update);
                        //   Logger::info("Distance: " + std::to_string(v.travelDistance + distance(destinationEdge,cp.second) + distance(destinationEdge, depot.second)), update);
                            
                        }
                        else{
                            // Not a valid option
                        //   Logger::info("Cant travel to edge " , update);

                        }
                    }

                   


                    break;
                }
            }


        }

        //now compute scores
        for(const auto & i : scores)
        {
            std::pair<coord, double> Pij;
            Pij.first = i.first.first;
            Pij.second = pheromoneCalc(i.second, i.first.second, scores);            
            calculatedScore.push_back(Pij);
            Logger::info("Raw Pheromone scores: " + Pij.first.to_string() + "\t" + std::to_string(Pij.second), update);
        }

            // 2. Normalize
        double totalScore = 0.0;
        for (const auto& p : calculatedScore) {
            totalScore += p.second;
        }

        // Safety check to avoid division by zero or NaN
        if (totalScore == 0.0 || std::isnan(totalScore)) {
            Logger::error("Total pheromone score is zero or NaN — check pheromoneCalc output", update);
            //  assign uniform probability if broken score
            double uniform = 1.0 / calculatedScore.size();
            for (auto& p : calculatedScore) {
                p.second = uniform;
            }
        } else {
            for (auto& p : calculatedScore) {
                p.second /= totalScore;
            }
        }

        v.scores = calculatedScore;

        std::uniform_real_distribution<> dist(0.0, 1.0);
        double randVal = dist(rng);
        double cumulative = 0.0;
        for(const auto& p : calculatedScore)
        {
            cumulative += p.second;
            if(randVal <= cumulative)
            {
            //    Logger::info("Randomly chose score ", update);
                //select this node to use
                //add this node
                //update pheromones
                visited.push_back(p.first);
                coord added = p.first;
                Logger::info("Adding to visited " +added.to_string(), update);

                v.travelDistance += distance(selected, p.first) + distance(p.first,depot.second);
                v.score += p.first.score;

                std::string id = "";
                for (const auto& node : nodes) {
                    if (node.second == p.first) {
                        id = node.first;
                        break;
                    }
                }
                curPath.push_back({id, added});
                

                //update pheromones for p.first
                for(int ec = 0; ec < alledges.size(); ec++)
                {
                    /*
                    Connected edges:
                    0-1
                    2-3
                    4-5  
                    */
                   coord edgePos(alledges[ec].first);
                   for(auto & eu : alledges[ec].second)
                   {
                    if(areCoordsEqual(eu.second.first,added))
                    {
                        eu.first = (params.Q*(eu.first)/p.second);
                        continue;
                    }
                    eu.first *= (1-params.evaporationRate);

                   }   

                   
                  
                }                
                changes = true;
                curPath.push_back(depot);
                v.path = curPath;
                break;
            }
            
        }


    }

   // std::pair<double, sf::Vertex> depotEdge = {depot.second.score, sf::Vertex(sf::Vector2f(depot.second.x, depot.second.y))};
    if(edges.size() > 0)
        edges.clear();

    for(auto & v : vehicles)
    {
        for(auto & p : v.path)
        {
            std::pair<double, sf::Vertex> newEdge;
            newEdge.first = p.second.score;
            newEdge.second = sf::Vertex(sf::Vector2f(p.second.x, p.second.y));
            edges.push_back(newEdge);
         //   Logger::info("(Pushed back)Edge for vehicle " + p.first + "\t" + p.second.to_string(), update);
        }
        Logger::info("Vehicles after it: " + v.to_string(), update);
    }

    /*

    for(auto & i : visited)
    {
        Logger::info("Visted: " + i.to_string(), update);

    }



    for(auto & i : edges)
    {
        coord ec(i.first, i.second.position.x, i.second.position.y);        
        Logger::info("Edges added: " + ec.to_string(), "edges.txt");

    }
    */
    




    double bs = getBestScore();
  //  Logger::info("BestScore: " + std::to_string(bs) + "\\tPrev: " + std::to_string(prevBestScore), "Iterationtrack.txt");

    if(bs <= prevBestScore)
    {
        maxItNoImpro++;
    }
    else{
        maxItNoImpro = 0;
        prevBestScore = bs;
    }
 //   Logger::info("MaxItNoImpro: " + std::to_string(params.maxItNoImpro) + "\tCount: " + std::to_string(maxItNoImpro) + "\tIt: " + std::to_string(runNum), "Iterationtrack.txt");

    

    runNum++;
}

void ProblemInstance::render(sf::RenderWindow& window, const std::vector<sf::CircleShape>& nodes/*, const std::vector<std::pair<double, sf::Vertex>>& edges*/)
{

    if (nodes.empty()) return;

/*

    for(auto & i :vehicles)
    {
        Logger::info("Vehicles: " + i.to_string(), "update.txt");
        std::vector<std::pair<std::string, coord>> path = i.path;
        
    }

    for(auto & i : alledges)
    {
        Logger::info("Coord" + i.first.to_string(), "P.txt");
        for(auto& j : i.second)
        {
            Logger::info("Pheromones: " + std::to_string(j.first), "P.txt");
        }
    }
*/
    
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    // Step 1: Find bounds (min/max X and Y from node positions)
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    sf::Font font;
    if(!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {

    }

    for (const auto& node : nodes) {
        auto pos = node.getPosition();
        minX = std::min(minX, pos.x);
        minY = std::min(minY, pos.y);
        maxX = std::max(maxX, pos.x);
        maxY = std::max(maxY, pos.y);
    }

    float dataWidth = maxX - minX;
    float dataHeight = maxY - minY;

    float padding = 40.f; // Space around the edges
    float scaleX = (windowWidth - 2 * padding) / dataWidth;
    float scaleY = (windowHeight - 2 * padding) / dataHeight;

    sf::Vector2f offset(padding - minX * scaleX, padding - minY * scaleY);


    // Step 2: Draw everything
    window.clear();

    // Transform and draw edges
    bool started = false;
    bool ended = false;
    std::vector<sf::Vertex>transformedEdges;
    std::vector<int> size;
    int count = 0;
    std::uniform_int_distribution<int> dist(0,255);
    sf::Color randomColour;
    for (size_t i = 0; i < edges.size(); ++i) {
        
        auto pos = edges[i].second.position;
        if(areCoordsEqual(coord(0,pos.x, pos.y), depot.second))
        {
           if(!started)
           {
            count = 0;
            started = true;           
            randomColour = sf::Color(dist(rng), dist(rng), dist(rng));
           }
           else{
            ended = true;
           }

           
        }

        if(started && !ended)
        {
    //    Logger::info("hasDepot: " + coord(0,pos.x, pos.y).to_string(), "draw.txt");
        sf::Vector2f newPos(pos.x * scaleX + offset.x, pos.y * scaleY + offset.y);
        if(!(newPos.x == 0 && newPos.y == 0))
        {           

            transformedEdges.push_back( sf::Vertex(newPos, randomColour));
            count++;
        }
        }
        if(ended)
        {
            started = false;
            ended = false;
            size.push_back(count);
        }
        
    }


    if (!transformedEdges.empty())
    {
        /*
        0,1,2 - 2
        3,4,5 - 2
        6,7,8 - 2
        9,10,11 - 2
        */
       //runNum + 1 
       //runNum * numVehicles = inner nodes      

       int startIndex = 0;
       for (size_t i = 0; i < size.size(); ++i) {
           int count = size[i];
           if (startIndex + count <= transformedEdges.size()) {
               window.draw(&transformedEdges[startIndex], count, sf::LineStrip);
           } else {
               Logger::warning("Invalid edge size for vehicle " + std::to_string(i));
           }
           startIndex += count;
       }


        
       // window.draw(&transformedEdges[0], transformedEdges.size(), sf::Lines);

        sf::Text label;
        label.setFont(font);
        label.setCharacterSize(12);
        label.setFillColor(sf::Color::Blue);

      for (size_t i = 0; i + 1 < transformedEdges.size(); i += 2)
      {
                  // Compute the midpoint between the two vertices
            sf::Vector2f p1 = transformedEdges[i].position;
            sf::Vector2f p2 = transformedEdges[i + 1].position;
            sf::Vector2f midPoint((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
        
            // Create a label with coordinates or other edge info
            std::ostringstream posStream;
            posStream << std::fixed << std::setprecision(2)
                      << "(" << edges[i].first << ")";
            label.setString(posStream.str());
        
            // Slight offset so it doesn't overlap exactly on the edge
            label.setPosition(midPoint.x + 5, midPoint.y - 5);
        
            // Draw the label at midpoint
            window.draw(label);
      }



    }


    for (const auto& node : nodes) {
        sf::CircleShape transformed = node;
        auto pos = node.getPosition();
        transformed.setPosition(pos.x * scaleX + offset.x, pos.y * scaleY + offset.y);
        window.draw(transformed);

        sf::Text label;
        label.setFont(font);
        label.setCharacterSize(12);
        label.setFillColor(sf::Color::White);
        std::ostringstream posStream;
        posStream << std::fixed << std::setprecision(2)
                << "(" << node.getPosition().x << ", " << node.getPosition().y << ")";
        label.setString(posStream.str());

        
        label.setPosition(transformed.getPosition().x + 5, transformed.getPosition().y - 5); // Offset from node center
        window.draw(label);
    }
    window.display();
}


std::vector<std::pair<std::string ,coord>> ProblemInstance::getNodeCoordSection()
{
       
    std::vector<std::pair<std::string ,coord>> coords;

    for (const auto& entry : node_coord_section) {
        if(entry.second.x != 0 && entry.second.y != 0)
        {
            coords.push_back(entry);  
            coord c = entry.second;
        //    Logger::info("Pushed back: " + c.to_string(), "nodecoord.txt");
        }

    }
    //std::reverse(coords.begin(), coords.end());
    return coords;
}

std::vector<std::pair<std::string, coord>> ProblemInstance::randomSol(unsigned int seed)
{
    std::vector<std::pair<std::string, coord>> c;
    return c;
}

std::map<std::string, std::vector<std::pair<std::string, double>>> ProblemInstance::getAdjList()
{
    return adjList;
}

std::map<std::string, coord> ProblemInstance::getFullCoordInfo()
{
    return node_coord_section;
}