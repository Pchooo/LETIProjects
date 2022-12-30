

#include <iostream>
#include <string>
#include <vector>

void createPi(std::string& templateP, std::vector<int>& pi){//функция вычисляющая значения префикс-функции для строки шаблона
    pi.resize(templateP.size());
    int j = 0;
    int i = 1;
    pi[0] = 0;
     std::cout<<"\nPrefix function calculation\nVisualization indexes:\n";
  for(int i = 0; i < pi.size(); i++){
      std::cout<<templateP[i]<<" "<<i<<std::endl;//visualization indexes
  }
  std::cout<<std::endl<<std::endl;
    
    for(;i < templateP.size();){
        if(templateP[j] == templateP[i]){// symbols are equal, indexes increased

            std::cout<<"Symbols with indexes "<<i<<" and "<<j<<" are equal. Indexes was increased by 1"<<std::endl;
            std::cout<<"Prefix function for symbol in position "<<i<<" is "<<j+1<<std::endl<<std::endl;

            pi[i] = j+1;//Prefix function for symbol i
            j++;
            i++;
        }
        else{//symbols not equal
            std::cout<<"Symbols with indexes "<<i<<" and "<<j<<" are not equal"<<std::endl;
            std::cout<<"Prefix function for symbol in position "<<i<<" is "<<0<<std::endl;

            if(j == 0){
                pi[i]= 0;//Prefix function for symbol i
                i++;
            }
            else{
                j = pi[j-1];
            }
            
        }
    }

    std::cout<<"Visualization prefix function:\n"<<std::endl;
    for(int i = 0; i < pi.size(); i++){
        std::cout<<templateP[i]<<" "<<pi[i]<<std::endl;//visualization prefix function
    }
    std::cout<<std::endl<<std::endl;
}


void KMP(std::string& textT, std::string& templateP, bool isForShift = false){//функция реализующая алгоритм Кнута-Морриса-Пратта
//    std::cout<<"\nKMP Working on \ntext (t):"<<textT<<"\nand template text (P): "<<templateP<<std::endl;
    std::vector<int> pi;//контейнер для прекфик-функции
    createPi(templateP, pi);
    int i = 0; //индекс для строки-текста
    int j = 0; //индекс для строки-шаблона
    std::vector<int> answer;
    bool isFound = false;
    std::cout<<"Index j for tamplate text (P), index i for text (T); Start j = 0; i = 0"<<std::endl;
    for(;i< textT.size();){//основной цикл, проходящий по всей строке-тексту
        if (textT[i] == templateP[j]){//рассматриваемые символы у строки-текста и строки-шаблона совпдаюат
            std::cout<<"The symbols with indexes i: "<<i<<" ("<<textT[i]<<"), and j: "<<j<<" ("<<templateP[j]<<") are equal, indexes increased by 1"<<std::endl;
            i++;
            j++;
            if(j == templateP.size()){
                std::cout<<"\n!The match is founded! Index:"<<i-templateP.size()<<std::endl<<std::endl;
               // std::cout<<(isFound?",":"")<<i-templateP.size();
                answer.push_back(i-templateP.size());
                isFound = true;
                if(isForShift){
                    std::cout<<"It is a cycle shift!\nIndex: "<<i-templateP.size()<<std::endl;
        
                    return;
                }
            }
        }
        else{//рассматриваемые символы у строки-текста и строки-шаблона не совпадают
            std::cout<<"The symbols with indexes i: "<<i<<" ("<<textT[i]<<"), and j: "<<j<<" ("<<templateP[j]<<") are NOT equal"<<std::endl;
            if(j == 0){
                i++;
                std::cout<<"Next: j = 0, i = "<<i<<std::endl;
            }
            else{
                j = pi[j-1];
                std::cout<<"Next: j = "<<j<< ", i = "<<i<<std::endl;

            }
        }
    }
    std::cout<<std::endl;
    if(isFound == false){//вхождений не было
        std::cout<<"There is no such template in string!"<<std::endl;
        std::cout<<-1<<std::endl;
    }
    else{//вывод всех индексов, с которых начинаются вхождения
        std::cout<<"All indexes: "<<answer[0];
        for(int i = 1; i < answer.size(); i++){
            std::cout<<","<<answer[i];
        }
        std::cout<<std::endl;

    }
}


void cyclicShift(std::string& strA, std::string& strB){//функция для проверки циклического сдвига
    if(strA == strB){//проверка на то, равны ли строки
        std::cout<<"Strings are equal!"<<std::endl;
        std::cout<<0<<std::endl;
        return;
    }
    if(strA.size() != strB.size()){//проверка: являются ли строки одинакового размера
        std::cout<<"Strings have different size!"<<std::endl;
        std::cout<<-1<<std::endl;
        return;
    }
    strA+=strA;
    KMP(strA, strB, true);
    
}

int main() {
    std::string templateP;
    std::string textT;
    std::cin>>templateP;
    std::cin>>textT;
//    if(textT.size()<templateP.size()){//проверка не длиннее ли строка-шаблон  строки-текста
//        std::cout<<"Text cannot be shorter than template\n-1"<<std::endl;
//        return -1;
//    }

    KMP(textT, templateP);
  //  cyclicShift(templateP,  textT);
    return 0;
}
