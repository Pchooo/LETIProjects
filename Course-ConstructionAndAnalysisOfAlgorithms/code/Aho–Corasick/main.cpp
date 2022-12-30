

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>

//------------------------------------------//
class BohrVertex{
public:
    bool flag; //флаг конечной вершины
    char name; //имя вершины
    int suffLink; //суффиксная ссылка
    int parent; //индекс в боре родительской вершины
    
    int offset; //длина паттерна (для конечной вершины)
    int patternNumber; // номер паттерна (для конечной вершины)
    
    std::map<char, int> nextVertex; // контейнер для вершин потомков. key - имя value - индекс в боре.
    std::map<char,int> autoMove; //ходы из вершины key - имя value - индекс в боре.
    
    //forJoker//
    std::vector<int> positionInPattern; // позиция в строке образце
    //---//
    
    BohrVertex(char name, int parent, int offset = -1, int patternNumber = -1, bool flag = false):name(name), parent(parent), offset(offset),patternNumber(patternNumber), flag(flag),suffLink(-1){}
};

//-----------------------------------------//


class Bohr{
public:
    std::vector<BohrVertex> bohr;
    std::vector<std::string> patterns; // контейнер всех паттернов в боре
    std::string text;
    std::map<int, std::vector<int>> result; // key - offset; value - pattern number
    
//--------individualization----------//
    int maxSuffLink; //максимальная длина цепочки из суффиксных ссылок
    int maxEndLink; // максимальная длина цепочки из конечных ссылок
    
//------------for joker------------//
    char joker;
    int patternLenght;
    std::vector<int> pos;
    void initBohrForJoker(char joker, std::string pattern, int textLenght); //функция инициализации бора для строки с джокерами
//---------------------------------//
    
    Bohr();
    void addPattern(std::string pattern, int positionInPattern);//функция добавления паттерна  в бор
    int getSuffLink(int index); //функция получения суффиксной ссылки
    int getAutoMove(int index, char symbol); //функция перехода
    void findAllPos(std::string textT, bool isJoker); // функция поиска всех вхождений
    void printResForStepic(); // функция печати ответа для степика
    void printRes(); // печать ответа
    void printBohr();//печать бора
};


Bohr::Bohr(){ //конструктор
    bohr.push_back(BohrVertex('0', -1)); //создание нулевой вершины
    bohr[0].suffLink = 0;
    maxEndLink = 0;
    maxSuffLink = 0;
}


void Bohr::addPattern(std::string pattern, int positionInPattern = -1){//функция добавления паттерна  в бор
    
    char name; //рассматриваемый символ в паттерне
    int currentVertex = 0;//текущая вершина в боре
    
    patterns.push_back(pattern);//добавление паттерна в вектор всех паттернов
    
    for(int i = 0; i < pattern.size(); i++){
        name = pattern[i];
        auto it = bohr[currentVertex].nextVertex.find(name);
        
        if (it == bohr[currentVertex].nextVertex.end()){//переход невозможен -> создание новой вершины
            bohr[currentVertex].nextVertex.insert(std::pair<char, int>(name, bohr.size()));
            bohr.push_back(BohrVertex(name, currentVertex));
            currentVertex = bohr.size()-1;
        }
        else{//переход по рассматриваемому символу возможен из текущей вершины
            currentVertex = it->second;
        }
    }
    bohr[currentVertex].flag = true;
    bohr[currentVertex].patternNumber = patterns.size();
    bohr[currentVertex].offset = pattern.size();
    
    if(positionInPattern != -1){//for joker
        bohr[currentVertex].positionInPattern.push_back(positionInPattern);
    }
}


int Bohr::getSuffLink(int vertexIndex){//функция получения суффиксной ссылки
    
    std::cout<<std::endl<<"Getting sufflink for vertex "<< bohr[vertexIndex].name<<" ("<<vertexIndex<<")"<<std::endl;
    
    if(bohr[vertexIndex].suffLink == -1){//суффиксная ссылка еще не была определена
        
        std::cout<<"link not yet defined go to parent"<<std::endl;
        
        if(vertexIndex == 0 || bohr[vertexIndex].parent == 0){//текущая вершина является корнем, либо же ее предок корень
            bohr[vertexIndex].suffLink = 0;
            std::cout<<"Link from "<<bohr[vertexIndex].name<<" ("<<vertexIndex<< ") is "<<bohr[vertexIndex].suffLink<<std::endl<<std::endl;
            return 0;
        }
        else{//иначе шаг из суффикс ссылки родителя
            bohr[vertexIndex].suffLink = getAutoMove(getSuffLink(bohr[vertexIndex].parent), bohr[vertexIndex].name);
        }
    }
    
    std::cout<<"Link from "<<bohr[vertexIndex].name<<" ("<<vertexIndex<< ") is "<<bohr[vertexIndex].suffLink<<std::endl<<std::endl;
    return bohr[vertexIndex].suffLink;
}


int Bohr::getAutoMove(int vertexIndex, char symbol){//функция перехода
    
    auto it =  bohr[vertexIndex].autoMove.find(symbol);
    
    if (it == bohr[vertexIndex].autoMove.end()){//нет перехода по данному символу и вершины
        std::cout<<"Search path from "<<bohr[vertexIndex].name<<" ("<<vertexIndex<<") to "<<symbol<<std::endl;
        
        auto it2 = bohr[vertexIndex].nextVertex.find(symbol);
        if(it2 == bohr[vertexIndex].nextVertex.end()){//прямого перехода не было найдено
            std::cout<<"Go by suffLink (no child)"<<std::endl;
            if (vertexIndex == 0){//в корень
                std::cout<<"to the root"<<std::endl;
                bohr[vertexIndex].autoMove.insert(std::pair<char, int>(symbol, 0));
            }
            else{//иначе по суффикс ссылке символа
                int move = getAutoMove(getSuffLink(vertexIndex), symbol);// ход автомата
                bohr[vertexIndex].autoMove.insert(std::pair<char, int>(symbol, move));
            }
            
        }
        else{//был найден прямой переход
            bohr[vertexIndex].autoMove.insert(std::pair<char, int>(it2->first, it2->second));
        }
    }
   
    std::cout<<"Path from "<<bohr[vertexIndex].name<<" ("<<vertexIndex<<") to "<<symbol<<"is found"<<std::endl;
    return bohr[vertexIndex].autoMove[symbol];
}


void Bohr::findAllPos(std::string textT, bool isJoker = false){// функция поиска всех вхождений
    
    this->text = textT;
    int currentVertex = 0;//текущая вершина в боре
    std::cout<<"Pattern entry search started. Start from root"<<std::endl;
    
    for(int i  = 0; i <text.size(); i++){
        std::cout<<"//-------------------//"<<std::endl<<"Step by symbol in text: "<<text[i]<<std::endl;;
        
        currentVertex = getAutoMove(currentVertex, text[i]);//шаг
        
        
        int currentSuffLenght = 0;
        int currentEndLenght = 0;
        
        if(!isJoker){
            //chech for aho-k
            
            
            for(int j = currentVertex; j != 0; j = getSuffLink(j), currentSuffLenght++){//check
                if(bohr[j].flag){
                    currentEndLenght++;
                    result[i+2 - bohr[j].offset].push_back(bohr[j].patternNumber);
                }
            }
            maxEndLink = currentEndLenght>maxEndLink ? currentEndLenght : maxEndLink;
            maxSuffLink = currentSuffLenght>maxSuffLink ? currentSuffLenght : maxSuffLink;

            //end check for aho-k
        }
        
        //check for joker
        else{
            for(int j = currentVertex; j != 0; j = getSuffLink(j), currentSuffLenght++){//check
                if(!bohr[j].positionInPattern.empty()){
                    currentEndLenght++;
                    for(auto it = bohr[j].positionInPattern.begin(); it != bohr[j].positionInPattern.end(); it++){
                        int posIndex = i - (*it) + 1;
                        if(posIndex >= 0 && posIndex  + patternLenght - 1 < pos.size()){
                            pos[posIndex]++;
                        }
                    }
                }
            }
            maxEndLink = currentEndLenght>maxEndLink ? currentEndLenght : maxEndLink;
            maxSuffLink = currentSuffLenght>maxSuffLink ? currentSuffLenght : maxSuffLink;

        //end check for joker
        }
        std::cout<<"//-------------------//"<<std::endl<<std::endl;
    }
    
    printBohr();
    
    //forJoker//
    if(isJoker){
        std::cout<<"Result:"<<std::endl<<"max suff link chain length: "<<maxSuffLink<<" max end link chain length: "<<maxEndLink<<std::endl<<"Positions:"<<std::endl;
        for(int i = 0; i < pos.size(); i++){
            if(pos[i] == patterns.size()){
                std::cout <<i+1<<std::endl;
            }
        }
    }
    else{
        printRes();
    }
}


void Bohr::printRes(){// печать ответа
    for(auto it = result.begin(); it != result.end(); it++){//сортировка по номеру паттерна для одинаковых позиций в тексте
        std::sort(it->second.begin(), it->second.end());
    }
    
    std::cout<<"Result:"<<std::endl<<"Max suff link chain length: "<<maxSuffLink<<"\nMax end link chain length: "<<maxEndLink<<std::endl<<"Positions:"<<std::endl;
    if(result.size() != 0){ // вывод результата
        for(auto it = result.begin(); it != result.end(); it++){
            for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++){
                std::cout<<"Offset in text: "<<it->first<<" Pattern №"<<(*it2)<<": "<<patterns[(*it2)-1]<<std::endl;
            }
        }
    }
    else{
        std::cout<<"there are no matches for your search!"<<std::endl;
    }
   
}


void Bohr::printResForStepic(){// функция печати ответа для степика
    for(auto it = result.begin(); it != result.end(); it++){
        std::sort(it->second.begin(), it->second.end());
    }

    if(result.size() != 0){
        for(auto it = result.begin(); it != result.end(); it++){
            for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++){
                std::cout<<it->first<<" "<<(*it2)<<std::endl;
            }
        }
    }
    else{
        std::cout<<"There are no matches for your search"<<std::endl;
    }
}


void Bohr::printBohr(){// печать бора
    std::cout<<std::endl<<"Bohr:"<<std::endl;
    std::cout << std::setw(8) << "Symbol" << std::setw(8) << "Index"<< std::setw(8) << "Parent"<<std::setw(10) << "  SuffLink  "<< std::setw(10) << "Sons (index)" << std::endl;
    for(int i = 0; i < bohr.size(); i++){
        std::cout<< std::setw(8)<<bohr[i].name<< std::setw(8)<<i<<std::setw(8)<<bohr[i].parent<< std::setw(8)<<bohr[i].suffLink<< std::setw(8);
        for(auto it = bohr[i].nextVertex.begin(); it != bohr[i].nextVertex.end(); it++){
            std::cout<<it->second<<" ";
        }
        std::cout<<std::endl;
    }
}

//-------------joker--------------------------//

void Bohr::initBohrForJoker(char joker, std::string pattern, int textLenght){//функция инициализации бора для строки с джокерами
    this->joker = joker;
    
    patternLenght = pattern.size();
    pos.clear();
    pos.resize(textLenght, 0); // инициализация массива позиций
    std::string tmpPattern = "";
    for(int i = 0; i < pattern.size(); i++){
        if(pattern[i] != joker){
            tmpPattern.push_back(pattern[i]);
        }
        else{
            if(tmpPattern.size() > 0){
                addPattern(tmpPattern, i );//- tmpPattern.size()+1)
            }
            tmpPattern.clear();

        }
    }
    if(tmpPattern.size() > 0){
        addPattern(tmpPattern, pattern.size());//pattern.size() - tmpPattern.size()+1
    }
}
//-------------------------------------------//



int main(int argc, const char * argv[]) {
    Bohr bor;
    std::string text;
    
    //-----Aho–Corasick----//
    std::string pattern;
    int countP = 0;
    std::cin>>text;
    std::cin>>countP;
    for(int i = 0; i < countP; i++){
        std::cin>>pattern;
        bor.addPattern(pattern);
    }
    bor.printBohr();
    bor.findAllPos(text);


    
    //-----------joker---------------//
    
//    std::string patternJoker;
//    char joker;
//    std::cin>>text;
//    std::cin>>patternJoker;
//    std::cin>>joker;
//    bor.initBohrForJoker(joker, patternJoker, text.size());
//    bor.findAllPos(text, true);
    
    return 0;
}
