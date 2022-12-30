
#include <iostream>
#include <vector>
#include <map>


class Edge;

class Vertex{
public:
    char name;
    bool check; // была ли вершина обработана
    double pathLength; // длина пути
    double heuristic;// эвристическая оценка
    double priority;
    Vertex* cameFrom;
    std::vector<Edge*> neighbors;
    Vertex(char name):name(name), check(false), pathLength(0), priority(0) {}
    
    void heuristicF(Vertex* goal){
        heuristic =  goal->name - this->name;
    }
    
    void pushNeughbor(Edge* neighbor){
        neighbors.push_back(neighbor);
    }
};

class Edge{
public:
    Vertex* start;
    Vertex* end;
    double weight;
    Edge(Vertex* start, Vertex* end, double weight):start(start), end(end), weight(weight){}
};



class Graph{
public:
    std::vector<Edge*> edgeList;
    std::vector<Vertex*> vertexList;
    
    void pushEdge(char start, char end, double weight){
        edgeList.push_back(new Edge(pushVertex(start), pushVertex(end), weight));
        edgeList.back()->start->pushNeughbor(edgeList.back());
    }
    
    
    Vertex* pushVertex(char name){
        for(int i = 0; i < vertexList.size(); i++){
            if (vertexList[i]->name == name){
                return vertexList[i];
            }
        }
        vertexList.push_back(new Vertex(name));
        return vertexList.back();
    }
    
    void input(){
        char start;
        char end;
        double weight;
      //  std::cout<<"input start vertex, end vertex and weight"<<std::endl;
        while (std::cin >> start >> end >> weight){
           this->pushEdge(start, end, weight);
        }
      //  std::cout<<"Input end"<<std::endl;
        
    }

    void visualization(){
        std::cout<<"Graph's vertex list"<<std::endl;
        for(int i = 0; i < vertexList.size(); i++){
            std::cout<<vertexList[i]->name<<std::endl;
        }
        std::cout<<std::endl<<"Graph's edge list"<<std::endl;
        for(int i = 0; i < edgeList.size(); i++){
            std::cout<<edgeList[i]->start->name<<" ---> "<<edgeList[i]->end->name<<" "<<edgeList[i]->weight<<std::endl;
        }
        std::cout<<std::endl;
    }
    
    
    Vertex* getVertex(char name){
        for( auto iter = vertexList.begin(); iter != vertexList.end(); iter++){
            if ((*iter)->name == name){
                return *iter;
            }
        }
        return nullptr;
    }
};




void path(Vertex* goal){//функция вывода пути до вершины qoal
    std::vector<Vertex*> path;
    while(goal != nullptr){
        path.push_back(goal);
        goal = goal->cameFrom;
    }

    std::cout<<"Path:"<<std::endl;
    for(auto it = path.rbegin(); it != path.rend(); it++){
        std::cout<<(*it)->name;
    }
    std::cout<<std::endl;
}





void greedyAlgorithm(Graph& graph, char start, char end){// реализация жадного алгоритма
    Vertex* current = graph.getVertex(start);
    Vertex* goal = graph.getVertex(end);
    if (current == nullptr || goal == nullptr){
        std::cout<<"Error! There is no start/end vertex in graph"<<std::endl;
        return;
    }
    current->cameFrom = nullptr;
    while(1){
        if (current == nullptr){
            break;
        }
        if(current->name == goal->name){
            break;
        }
        current->check = true;
        std::cout<<"Go to vertex " << current->name<<"\nChecking neighbors"<<std::endl;
        Vertex* tmp = nullptr;
        double maxCost = 10000;
        for(auto it = current->neighbors.begin(); it != current->neighbors.end(); it++){//выбор оптимального по версии жадного алгоритма пути
            std::cout<<(*it)->end->name<<" weight: "<<(*it)->weight<<std::endl;
            if((*it)->end->check != true && (*it)->weight < maxCost){
                maxCost = (*it)->weight;
                (*it)->end->pathLength = current->pathLength+(*it)->weight;
                (*it)->end->cameFrom = current;
                tmp = (*it)->end;
            }
        }
        if(tmp == nullptr){
            current = current->cameFrom;
        }
        else{
            current = tmp;
        }
    }
    std::cout<<"Path lenght: "<<current->pathLength<<std::endl;
    path(current);
}

void aStar(Graph& graph, char start, char end){// реализация алгоритма А*

    std::multimap<double, Vertex*> frontier;//контейнер для вершин, до которых уже проложен путь и которые могут быть обработаны следующими; ключ - приоритет
    Vertex* current = graph.getVertex(start);
    Vertex* goal = graph.getVertex(end);

    if (current == nullptr || goal == nullptr){
        std::cout<<"Error! There is no start/end vertex in graph"<<std::endl;
        return;
    }
    current->heuristicF(goal);
    current->cameFrom = nullptr;
    current->priority = current->heuristic+current->pathLength;
    frontier.insert(std::pair<double,Vertex*>(current->priority, current));
    
    while(!frontier.empty()){

        std::cout<<"\nChoosing vertex from frontier "<<std::endl; //вывод вершин, которые могут быть обработаны следующими
           for(auto it = frontier.begin(); it != frontier.end(); it++){
               std::cout<<"Priority: "<<it->first<<" name: "<<it->second->name<<std::endl;
           }

        auto it = frontier.begin();//выбирается вершина с приоритетом с наименьшим значением
        current = it->second;//присваивается обрабатываемой вершине
        frontier.erase(it);//и удаляется из контейнера
        current->check = true;

        std::cout<<std::endl<<"Go to vertex "<<current->name<<std::endl<<std::endl;

        if(current->name == goal->name){
            break;
        }

        for(auto it = current->neighbors.begin(); it != current->neighbors.end(); it++){ //цикл в котором обрабатываются все вершины
            if(((*it)->end->check != true)){                                    // до которых можно дойти из текущей вершины;  если вершина не была обработана раньше -
                if(((*it)->end->pathLength > current->pathLength + (*it)->weight) || (*it)->end->pathLength == 0 ){// - проверяется, нашелся ли для нее путь лучше текущего
                    (*it)->end->heuristicF(goal);                                                   //если да у вершины обновляются соответствующие переменные
                    (*it)->end->pathLength = current->pathLength+(*it)->weight;
                    (*it)->end->cameFrom = current;
                    (*it)->end->priority = (*it)->end->pathLength+(*it)->end->heuristic;
                    frontier.insert(std::pair<double,Vertex*>((*it)->end->priority,(*it)->end));// и она заносится в контейнер обрабатываемых вершин
                    std::cout<<"Update frontire vertex: "<< (*it)->end->name<<" path lenght: "<< (*it)->end->pathLength << " priority: "<< (*it)->end->priority<<std::endl;
                    path((*it)->end);
                }
            }
        }
    }
    std::cout<<"Path lenght: "<<current->pathLength<<std::endl;
    path(current);//вывод пути для конечной вершины
    }


void Dijkstra(Graph& graph, char start){//реализация алгоритма Дейкстры на основе кода А*

    std::multimap<double, Vertex*> frontier;//контейнер для вершин, до которых уже проложен путь и которые могут быть обработаны следующими
    Vertex* current = graph.getVertex(start);

    if (current == nullptr ){
        std::cout<<"Error! There is no start vertex in graph"<<std::endl;
        return;
    }
    
    current->cameFrom = nullptr;
    frontier.insert(std::pair<double,Vertex*>(current->priority, current));
    
    while(!frontier.empty()){
    
        // в отличие от А* нет проверки на то, является ли текущая вершина последней
        
        std::cout<<"\nChoosing vertex from frontier"<<std::endl;//вывод вершин, которые могут быть обработаны следующими
           for(auto it = frontier.begin(); it != frontier.end(); it++){
               std::cout<<"Priority: "<<it->first<<" name: "<<it->second->name<<std::endl;
           }

        auto it = frontier.begin();//выбирается вершина с приоритетом с наименьшим значением
        current = it->second;//присваивается обрабатываемой вершине
        frontier.erase(it);//и удаляется из контейнера
        current->check = true;

       std::cout<<std::endl<<"Go to vertex "<<current->name<<std::endl<<std::endl;


        for(auto it = current->neighbors.begin(); it != current->neighbors.end(); it++){
            if(((*it)->end->check != true)){
                if(((*it)->end->pathLength > current->pathLength + (*it)->weight) || (*it)->end->pathLength == 0 ){
                    (*it)->end->pathLength = current->pathLength+(*it)->weight;
                    (*it)->end->cameFrom = current;
                    (*it)->end->priority = (*it)->end->pathLength;// в отличие от А* к приоритету не прибавляется эврестическая оценка
                    frontier.insert(std::pair<double,Vertex*>((*it)->end->priority,(*it)->end));
                    std::cout<<"Update frontire vertex: "<< (*it)->end->name<<"\npath lenght to this vertex: "<< (*it)->end->pathLength << " priority: "<< (*it)->end->priority<<std::endl;
                    path((*it)->end);
                }
            }
        }
    }
    
    std::cout<<"All paths from vertex "<<start<<std::endl;//в отличие от А* выводятся пути ко всем вершинам
    for(auto it = graph.vertexList.begin(); it != graph.vertexList.end(); it++){
        std::cout<<"to "<<(*it)->name<<" path lenght: "<<(*it)->pathLength<< std::endl;
        path(*it);
    }
    
}



int main() {
    Graph grapg;
    char start;
    char end;
    int input = 0;
    
   // std::cout<<"Enter command: 0 - Dijkstra’s Algorithm,  1 - Greedy Algorithm 2 - aStar, default - 0 \nThen start vertex and end vertex(if needed) "<<std::endl;
    std::cin>>input;
    switch (input) {
        case 1:
            std::cout<<"Greedy Algorithm"<<std::endl;
            std::cin>>start>>end;
            grapg.input();
            grapg.visualization();
            greedyAlgorithm(grapg, start, end);
            break;
        case 2:
            std::cout<<"A*"<<std::endl;
            std::cin>>start>>end;
            grapg.input();
            grapg.visualization();
            aStar(grapg, start, end);
            break;
        case 0:
        default:
            std::cout<<"Dijkstra’s Algorithm"<<std::endl;
            std::cin>>start;
            grapg.input();
            grapg.visualization();
            Dijkstra(grapg, start);
            break;
    }

    
    return 0;
}
