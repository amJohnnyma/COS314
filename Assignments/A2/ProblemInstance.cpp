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
        Logger::info("Pushed back: " + c.to_string(), "erh.txt");
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
    double top = std::pow(Tij, pheromoneImportance) * std::pow(Nij, heuristicImportance);
    double bottom = 0.0;
    for(const auto & i : scores)
    {
        bottom += (std::pow(i.first.second, heuristicImportance) * std::pow(i.second, pheromoneImportance));
    }

    return top / bottom;
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
        const std::string& fID = id;
        const coord & nodePos = nodeCoordMap[fID];
        //get matching ID in node_coord_section
        for(const auto & neighbour : otherNode)
        {


            auto it = nodeCoordMap.find(neighbour.first);
            if(it != nodeCoordMap.end())
            {
                auto pairKey = std::minmax(fID, neighbour.first);
                if (drawn.count(pairKey)) continue;
                drawn.insert(pairKey);

                std::pair<double, sf::Vertex> start = {pheromones, sf::Vertex(sf::Vector2f(nodePos.x, nodePos.y), sf::Color::Red)};  //start
                std::pair<double, sf::Vertex> end = {pheromones, sf::Vertex(sf::Vector2f(nodeCoordMap[neighbour.first].x, nodeCoordMap[neighbour.first].y), sf::Color::Red)}; //end
                
                alledges.push_back(start);
                alledges.push_back(end);
            }
            else{
                std::cout << neighbour.first << std::endl;
                std::cout << "COORD NOT FOUND" << std::endl;
            }

        }

    }
 


    while (window.isOpen()) {
        handleEvents(window); 
        update(nCircles);   
        if(changes)
        {
            render(window, nCircles); 
            changes = false;
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

void ProblemInstance::update(std::vector<sf::CircleShape>& nCircles)
{   
    std::string update = "update.txt";
    Logger::info("Updating...", update);
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
        for(auto & cp : curPath)
        {
            Logger::info("Checking current path", update);
            //if we are at the depot check if we are starting or ending
            if(cp.second == depot.second)
            {
                Logger::info("At a depot", update);
                if(atDepot)
                {
                    Logger::info("First depot", update);
                    atDepot = false; //Going to move away from depot
                }
                else{
                    //We have finished the path checking
                    Logger::info("Second depot", update);
                    break;
                }
            }
            selected = cp.second;
          //  Logger::info("Selected " + selected.to_string() , update);
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
                coord edgePos(selected.score,alledges[ec].second.position.x,alledges[ec].second.position.y);
                coord destinationEdge;
               // Logger::info("edgepos: " + edgePos.to_string(), update);

                if(areCoordsEqual(edgePos, selected))
                {
                 //   Logger::info("Matching edge for " + selected.to_string(), update);

                    //Get the other end of the edge
                    if(ec%2==0)
                    {
                        //Take next edge as destination
                        destinationEdge = coord(getNodeScore(coord(0,alledges[ec+1].second.position.x, alledges[ec+1].second.position.y)).score,alledges[ec+1].second.position.x, alledges[ec+1].second.position.y);
                    //    Logger::info("GIMME THE Score: " + std::to_string(getNodeScore(coord(0,alledges[ec+1].second.position.x, alledges[ec+1].second.position.y)).score), update);

                    }
                    else{
                        //take previous edge as destination
                        destinationEdge = coord(getNodeScore(coord(0,alledges[ec-1].second.position.x, alledges[ec-1].second.position.y)).score,alledges[ec-1].second.position.x, alledges[ec-1].second.position.y);
                     //   Logger::info("GIMME THE Score: " + std::to_string(getNodeScore(coord(0,alledges[ec-1].second.position.x, alledges[ec-1].second.position.y)).score), update);

                    }
                    Logger::info("Destination edge " + destinationEdge.to_string(), update);

                    for(const auto& cur : nodes)
                    {
                        if(areCoordsEqual(coord(0,cur.second.x, cur.second.y), coord(0,destinationEdge.x, destinationEdge.y)))
                        {
                        //    Logger::info("Destination edge found " + destinationEdge.to_string(), update);
                            break;
                        }
                    }
                   


                    //We are connected to this edge from cp so calculate heuristic

                    //We can still travel here
                    //Vehicles total travel cant exceed tmax
                    // depot -> nodes -> depot
                    // total distance travelled + distance to node + distance to depot from next node <= tmax
                    if((v.travelDistance + distance(destinationEdge,cp.second) + distance(destinationEdge, depot.second)) <=tmax && !isVisited(destinationEdge))
                    {
                      //  Logger::info("Can travel to edge ", update);

                        //find the nodes score
                        std::pair<std::pair<coord,double>, double> thisScore = {                            
                                {destinationEdge, destinationEdge.score / distance(destinationEdge, cp.second)},
                                alledges[ec].first                            
                        };
                        scores.push_back(thisScore);
                        Logger::info("Pushed score: " + thisScore.first.first.to_string(), update);
                        Logger::info("Distance: " + std::to_string(v.travelDistance + distance(destinationEdge,cp.second) + distance(destinationEdge, depot.second)), update);
                        
                    }
                    else{
                        // Not a valid option
                      //  Logger::info("Cant travel to edge " , update);

                    }
                }
            }


        }

        //now compare scores
        for(const auto & i : scores)
        {
            std::pair<coord, double> Pij;
            Pij.first = i.first.first;
            Pij.second = pheromoneCalc(i.second, i.first.second, scores);
            calculatedScore.push_back(Pij);
            Logger::info("Pheromone scores: " + Pij.first.to_string() + "\t" + std::to_string(Pij.second), update);
        }

        double randVal = ((double) rand() / RAND_MAX);
        double cumulative = 0.0;
        for(const auto& p : calculatedScore)
        {
            cumulative += p.second;
            if(randVal <= cumulative)
            {
                Logger::info("Randomly chose score ", update);
                //select this node to use
                //remove last node in curPath
                //add this node
                //add depot node
                //update pheromones
                visited.push_back(p.first);
                coord added = p.first;
                Logger::info("Adding to visited " +added.to_string(), update);

                v.travelDistance += distance(selected, p.first);

                std::string id = "";
                for (const auto& node : nodes) {
                    if (node.second == p.first) {
                        id = node.first;
                        break;
                    }
                }
                curPath.push_back({id, added});
                curPath.push_back(depot);

                //update pheromones for p.first
                for(int ec = 0; ec < alledges.size(); ec++)
                {
                    /*
                    Connected edges:
                    0-1
                    2-3
                    4-5  
                    */
                   coord edgePos(0,alledges[ec].second.position.x,alledges[ec].second.position.y);
                   if(areCoordsEqual(edgePos,added))
                   {
                       Logger::info("Equal coords: " + added.to_string(), update);
                       //Get the other end of the edge
                       if(ec%2==0)
                       {
                           //Take next edge as destination
                           //update pheromone
                           alledges[ec].first += (1/p.second); 
                           alledges[ec+1].first += (1/p.second);

                           edges.push_back(alledges[ec]);
                           edges.push_back(alledges[ec+1]);
                           
                       }
                       else{
                           //take previous edge as destination
                           //update pheromone
                           alledges[ec].first += (Q/p.second); 
                           alledges[ec-1].first += (Q/p.second);

                           edges.push_back(alledges[ec]);
                           edges.push_back(alledges[ec-1]);
   
                       }
                       
                       break;
                   }
                   
                  
                }                
                changes = true;
                v.path = curPath;
                break;
            }
            
        }

       // Logger::info(v.to_string(), "VehicleData.txt");
    }

    /*
        for(auto & i : visited)
    {
        Logger::info("Visited: " + i.to_string(), update);
    }

    for(auto & i : edges)
    {
        coord ec(i.first, i.second.position.x, i.second.position.y);        
        Logger::info("Edges added: " + ec.to_string(), update);

    }
    */


    for(auto & i : alledges)
    {
        i.first *= (1-evaporationRate); 
    }

    /*
        for(auto & v : vehicles)
    {
        Logger::info("Vehicle: ", update);
        Logger::info(v.to_string(), update);
    }
    */


}

void ProblemInstance::render(sf::RenderWindow& window, const std::vector<sf::CircleShape>& nodes/*, const std::vector<std::pair<double, sf::Vertex>>& edges*/)
{

    if (nodes.empty()) return;

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
    std::vector<sf::Vertex>transformedEdges;
    for (size_t i = 0; i < edges.size(); ++i) {
        auto pos = edges[i].second.position;
        sf::Vector2f newPos(pos.x * scaleX + offset.x, pos.y * scaleY + offset.y);
        if(!(newPos.x == 0 && newPos.y == 0))
        {
            transformedEdges.push_back( sf::Vertex(newPos, edges[i].second.color));
        }
        
    }

    if (!transformedEdges.empty())
    {
        
        window.draw(&transformedEdges[0], transformedEdges.size(), sf::Lines);

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

      //  window.display();
     //   sf::sleep(sf::seconds(0.05));

      //  Logger::info("Drawing:(" + std::to_string((double)node.getPosition().x) + "," + std::to_string((double)node.getPosition().y) + ")", "drawlog.txt");
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
            Logger::info("Pushed back: " + c.to_string(), "nodecoord.txt");
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