
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>



class Edge{
public:
    char start;
    char end;
    int capacity;
    int flow;
    int residualCapacity;
    bool check;
    Edge(char start, char end, int capacity): start(start), end(end), capacity(capacity), flow(0), residualCapacity(capacity), check(false){}
};

class edgeComp{
public:
    bool operator()(const std::pair<char, Edge*>& right, const std::pair<char, Edge*>& left){
        if(left.first == right.first){
            return left.second->end < right.second->end;
        }
        return left.first < right.first;
    }
};

bool cmp(const std::pair<char, Edge*>& right, const std::pair<char, Edge*>& left){
    if(left.first == right.first){
        return left.second->end > right.second->end;
    }
    return left.first > right.first;
}

class Network{
public:
    
    std::multimap<char, Edge*> network;
    int maxFlow;
    char sourse;
    char sink;
    
    Network():maxFlow(0){
        int quantity;
        std::cin>>quantity;
        std::cin>>sourse;
        std::cin>>sink;
        char v1;
        char v2;
        int capacity;
        for(int i = 0; i < quantity; i++){
            std::cin>>v1>>v2>>capacity;
            network.insert(std::pair<char, Edge*> (v1,(new Edge(v1, v2, capacity))));
            network.insert(std::pair<char, Edge*> (v2,(new Edge(v2, v1, 0))));
        }
    }
    
    void visualization(bool with){
        std::cout<<std::endl<<"Start-End   Capacity  Flow  Residual capacity"<<std::endl;
        for(auto it = network.begin(); it != network.end(); it++){
            if(it->second->capacity != 0){
                std::cout<<"  "<<it->second->start<<"----"<<it->second->end<<"        "<<it->second->capacity<<"       "<<it->second->flow<<"          "<<it->second->residualCapacity<<std::endl;
            }
        }
        if(with){
            std::cout<<std::endl<<"REVERSE"<<std::endl<<"Start-End   Capacity  Flow  Residual capacity"<<std::endl;
            for(auto it = network.begin(); it != network.end(); it++){
                if(it->second->capacity == 0){
                    std::cout<<"  "<<it->second->start<<"----"<<it->second->end<<"        "<<it->second->capacity<<"       "<<it->second->flow<<"          "<<it->second->residualCapacity<<std::endl;
                }
            }
        }
    }
    
    
    void visualozationForStepic(){
        std::cout<<maxFlow<<std::endl;
        std::vector<std::pair<char, Edge*>> tmp;
        for(auto it = network.begin(); it!=network.end(); it++){
            if(it->second->capacity != 0){
                tmp.push_back(std::pair<char, Edge*>(it->first, it->second));
            }
        }
        std::sort(tmp.begin(), tmp.end(), cmp);
        for(auto it = tmp.begin(); it != tmp.end(); it++){
            std::cout<<it->second->start<<" "<<it->second->end<<" "<<(it->second->flow < 0?0:it->second->flow )<<std::endl;

        }
    }
    
    
    void pathVisualization(std::vector<Edge*>& path, int minFlow){
        std::cout<<"Find path. Min flow "<<minFlow<<std::endl;
        for(auto it = path.begin(); it != path.end(); it++){
            std::cout<<(*it)->start<<"->"<<(*it)->end<<" ";
        }
        std::cout<<std::endl;
    }
    
    Edge* getEdge(char start, char end){
        for(auto it = network.lower_bound(start); it != network.upper_bound(start); it++){
            if(it->second->end == end){
                return it->second;
            }
        }
        return nullptr;
    }
    void changeCap(char start, char end, int flow){
        Edge* tmp = getEdge(start, end);
        tmp->flow +=flow;
        tmp->residualCapacity = tmp->capacity - tmp->flow;
        tmp = getEdge(end, start);
        tmp->flow -=flow;
        tmp->residualCapacity = tmp->capacity - tmp->flow;
    }
//
//    Edge* getEdgeMaxFlow(char start){
//        int maxFlow = 0;
//        Edge* tmp = nullptr;
//        for(auto it = network.lower_bound(start); it != network.upper_bound(start); it++){
//            if(it->second->residualCapacity > maxFlow && it->second->check == false){
//                maxFlow = it->second->residualCapacity;
//                tmp = it->second;
//            }
//        }
//        return tmp;
//    }

    Edge* getEdgeMaxFlow(char start){
        int maxFlow = 0;
        Edge* tmp = nullptr;
        for(auto it = network.lower_bound(start); it != network.upper_bound(start); it++){
            if(it->second->residualCapacity >= maxFlow && it->second->check == false && it->second->residualCapacity != 0){
                if(it->second->residualCapacity == maxFlow){
                    if(tmp == nullptr || it->second->end < tmp->end){
                        maxFlow = it->second->residualCapacity;
                        tmp = it->second;
                        it = network.lower_bound(start);
                        it--;
                    }
                }
                else{
                    maxFlow = it->second->residualCapacity;
                    tmp = it->second;
                }
            }
        }
        return tmp;
    }
    
    void rebootNetwork(){
        for(auto it = network.begin(); it != network.end(); it++){
            it->second->check = false;
        }
    }
    
    
    int findPath(std::vector<Edge*>& path){
        char currentV = sourse;
        Edge* nextEdge = nullptr;
        path.clear();
        while(currentV != sink){
            nextEdge = getEdgeMaxFlow(currentV);
            if(nextEdge == nullptr){
                if(path.empty()){
                    return 0;
                }
                else{
                    nextEdge = path.back();
                    currentV = nextEdge->start;
                    path.pop_back();
                }
                
            }
            else{
                nextEdge->check = true;
//                for(auto it = network.begin(); it != network.end(); it++){
//                    if(it->second->end == nextEdge->start){
//                        it->second->check = true;
//                    }
//                }
                //getEdge(nextEdge->end, nextEdge->start)->check = true;
                currentV = nextEdge->end;
                path.push_back(nextEdge);
            }
        }
        int minFlow = -1;
        for(auto it = path.begin(); it != path.end(); it++){
            if((*it)->residualCapacity < minFlow || minFlow == -1){
                minFlow = (*it)->residualCapacity;
            }
        }
        pathVisualization(path, minFlow);
        return minFlow;
    }
    
    
    void fordF(){
        std::vector<Edge*> path;
        int currentFlow = 0;
        while(1){
            currentFlow = findPath(path);
            if(path.empty()){
                break;
            }
            for(auto it = path.begin(); it != path.end(); it++){
                changeCap((*it)->start, (*it)->end, currentFlow);
            }

            maxFlow+=currentFlow;
            rebootNetwork();
            visualization(1);
        }
    }
};





int main(int argc, const char * argv[]) {
    Network n = Network();
    n.visualization(0);
    n.fordF();
    n.visualozationForStepic();
    return 0;
}
