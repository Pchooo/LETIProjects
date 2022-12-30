#include <iostream>
#include <fstream>

#include <queue>
#include <vector>
#include <cstdlib>
#include "Btree_.h"
#include "Ftree.h"
#include <ctime>
#define INS ( readFromFile?(*inFile):cin )

using namespace std;


istream *inFile = NULL; //for name input file
bool readFromFile = false;

void setIFile(istream* istr){
    inFile=istr;
}


BinaryTree<char> ConsBT(const char &x, BinaryTree<char> lst, BinaryTree<char> rst) {
    return BinaryTree<char>(x, lst, rst);//*(new BinaryTree<char>(x,lst,rst));
}

BinaryTree<char> ConsBT(const char &x) {
    return BinaryTree<char>(x);//*(new BinaryTree<char>(x));
}



int _enterBT(BinaryTree<char> &b);//Input

void _outBT(BinaryTree<char> b);//Formatter str tree output

void displayBT(BinaryTree<char> b, int n);//Fancy tree graph
void displayFF(vector<FTree<char> > forest);
void displayFT(FTree<char> b, int n=1);
void BFS(vector<FTree<char> > forest);
int toTreeNode(FTree<char> &root, BinaryTree<char> subtree);
int toForest(BinaryTree<char> b, vector<FTree<char>> &forest);

int menu(string& iFileName);
///Utility stuff
void printKLP(BinaryTree<char> b);//Print tree Rlr
void printLKP(BinaryTree<char> b);//Print tree lRr
void printLPK(BinaryTree<char> b);//Print tree lrR



int main() {
    //Variables
    string iFileName;//for file name
    BinaryTree<char> b;// for input binary tree
    
    ifstream iFile;
    
    if(menu(iFileName)){// call menu 1 - input from file
        iFile.open(iFileName);
        if (!iFile){
            cout<<"Cannot open file: "<< iFileName <<"\n";
            exit(1);
        }
        setIFile(&iFile);
    }
    else{//  0 - input from console
        cout<<"Enter tree sequense:\n";
    }
    //Main inpit-process-output-out cycle
    
    //Input
    if(_enterBT(b)){//call _enterBT. 1 - errors
        if(INS.eof()){
            cout<<"Input is over!\n";
            return 0;
            
        }
        if(INS.fail()){
            cout<<"Input stuck!\n";
            return 0;
        }
        cout<<("Wrong input data!\n");
        return 0;
    }
    cout<<"Input: ";
    _outBT(b);//Prints formatted string tree
    cout<<"\n";

    ///Representation
    cout<<"Flipped bin tree : \n";
    displayBT(b, 1);//displaying the tree in a visual form of the list

    ///Real processinп
    cout<<"\nCreating forest:\n";
    vector<FTree<char>> forest;


    toForest(b, forest);//creating a forest from binary tree
    displayFF(forest);//displaying the forest in a visual form of the list
    BFS(forest);//output of forest elements in horizontal order
    b.destroy();
    return 0;
}
//-------------------------------------

int toTreeNode(FTree<char> &root, BinaryTree<char> subtree){//non-binary tree creation function
    FTree<char> tree = FTree<char>(subtree.RootBT());//creating a subtree
    root.addLink(tree);                              //adding it to the list of subtrees of a given root
    cout<<"\tAdd the subtree " <<subtree.RootBT()<<" in the root "<<root.RootFT()<<endl;
    if(!subtree.Left().isNull()){//the left subtree of the subtree is the son of this subtree
        toTreeNode(tree, subtree.Left());//subrtee.Left - another subtree
    }
    if(!subtree.Right().isNull()){//the right subtree of the subtree is the brother of this subtree
        toTreeNode(root, subtree.Right());
    }
    return 0;
}
//-----------------------
int toForest(BinaryTree<char> b, vector<FTree<char>> &forest){//forest creation function
    if(b.isNull()){
        return 1;
    }
    FTree<char> tree = FTree<char>(b.RootBT());//creating a tree from a sent root
    forest.push_back(tree);//adding tree to forest trees vector
    cout<<"\tAdd root "<<b.RootBT()<<" of tree in forest "<< endl;
    if(!b.Left().isNull()){//left subtree of a binary tree - the son of tree
        toTreeNode((forest[forest.size()-1]), b.Left());
       // toTreeNode(tree, b.Left());//&
    }
    if(!b.Right().isNull()){//right binary tree subtree - another forest tree
        toForest(b.Right(), forest);
    }
    return 0;
}

//----------------
int menu(string& iFileName){//user interface
    int inputN;//for command
    while(true){
        cout<<"Сonversion of a binary tree into a forest\n Select input. \n 1 - input from the console \n 2 - input from a file"<<endl;
        cin>>inputN;
        cin.clear();
        cin.ignore(10000,'\n');
        if(cin.fail()||((inputN != 1)&&(inputN !=2))){
            cout<<"\nInvalid comand, try again.\n";
            continue;
        }
        if(inputN == 2){
            cout<<"Enter the name of file\n";
            cin>>iFileName;
            readFromFile = true;
            return 1;
            }
        if(inputN == 1){
            readFromFile=false;
            return 0;
        }
    }
}

//--------------------------------------
///Read tree expression. Called from _enterBT
int _readExpr(BinaryTree<char> &b) {
    // BinaryTree<char> b;
    char ch;
    char root;
    BinaryTree<char> l;
    BinaryTree<char> r;

    ///Extra spaces read
    INS >> ch;
    while (ch == ' ')
        INS >> ch;
    if (ch == ')') {
        b = BinaryTree<char>();
        return 0;
    }
    ///Read root
    
    root = ch;

    INS >> ch;
    
    while (ch == ' ')
        INS >> ch;
    if (ch == ')') {
        b = BinaryTree<char>(root);
        return 0;
    }
    ///Read left
    if (ch == '(') {
        if (_readExpr(l))
            return 1;
    }
    else if (ch == '/')
        l = BinaryTree<char>();
    else
        l = ConsBT(ch);

    INS >> ch;
    
    while (ch == ' ')
        INS >> ch;
    if (ch == ')') {
        b = BinaryTree<char>(root, l, BinaryTree<char>());
        return 0;
    }
    ///Read right
    if (ch == '(') {
        if (_readExpr(r))
            return 1;
        
    }
    else
        if (ch == '/')
            r = BinaryTree<char>();
    else
        r = ConsBT(ch);
    ///Read to the end
    INS >> ch;
    while (ch == ' ')INS >> ch;
    if (ch == ')') {
        b = BinaryTree<char>(root, l, r);
        return 0;
    } else {
        ///WROOOOOOOONG
        return 1;
    }

}

///User function for tree reading
int _enterBT(BinaryTree<char> &b) {//Input

    char ch;
    INS >> ch;
    while (ch == ' ')
        INS >> ch;
    if (ch == '(') {
        return _readExpr(b);
    }
    else {
        //Everything is bad...
        b = BinaryTree<char>();
        return 1;
    }
}
//---------------------------------------

void _outBT(BinaryTree<char> b) {///Prints formatted string tree
    if (!b.isNull()) {
        //if(b.Left().isNull() && b.Right().isNull()){cout<<b.RootBT()<<" ";return;}
        cout<<"( ";
        cout<<b.RootBT();
        cout<<" ";//b.RootBT()
        _outBT(b.Left());
        _outBT(b.Right());
        cout<<") ";
    }
    else
        cout<<"/ ";
}


//---------------------------------------
void displayBT(BinaryTree<char> b, int n) {///Prints graphic bin tree
    if (!b.isNull()) {
        cout<< " ";
        cout<<b.RootBT();
        if (!b.Right().isNull()) {
            displayBT(b.Right(), n + 1);
            
        }
        else cout<<"\n";
        if (!b.Left().isNull()) {
            for (int i = 1; i <= n; i++)
                cout<<"  ";
            displayBT(b.Left(), n + 1);
        }
    } ;
}

//---------------------------------------
///Main task stuff
void BFS(vector<FTree<char> > forest){//output of forest elements in horizontal order
    cout<<"Forest elements in horizontal order: \n";
    queue<FTree<char>> q;//create a queue
    int k = forest.size();
    for(int i = 0; i < k; i++){
        q.push(forest[i]);//push forest tree roots
    }
    while(!q.empty()){
        FTree<char> c = q.front();
        q.pop();                 //the root of the tree is pop at the top of the queue
        cout<<c.RootFT()<<" ";  //prints this root
        vector<FTree<char>> subtree = c.getSubtreeVec();//get subtrees
        k = subtree.size();
        for(int i = 0; i<k; i++){
            q.push(subtree[i]);//pushing subtrees
        }
    }
    cout<<endl;
}

void displayFF(vector<FTree<char> > forest){
    cout<<"Flipped forest : \n";
    for(int i=forest.size()-1;i>=0;i--){//prints all trees
        displayFT(forest[i]);
    }
}

void displayFT(FTree<char> b, int n) {///Prints graphic tree
    if (!b.isNull()) {
        cout<< " ";
        cout<<b.RootFT();
        auto vec = b.getSubtreeVec();//getting all subtrees
        
        if(vec.size()!=0){
            auto rit=vec.rbegin();
            displayFT(*rit, n+1);
        }else{
            cout<<endl;
        }
        for(int j = vec.size()-2; j>=0 ; j--){
            for (int i = 1; i <= n; i++)
                cout<<"  ";//indent printing
            displayFT(vec[j], n+1);
        }
       
    }
}

//---------------------------------------




///Utility stuff

//---------------------------------------
void printKLP(BinaryTree<char> b) {//Print tree Rlr
    if (!b.isNull()) {
        cout<<b.RootBT();
        printKLP(b.Left());
        printKLP(b.Right());
    }
}

//---------------------------------------
void printLKP(BinaryTree<char> b) {//Print tree lRr
    if (!b.isNull()) {
        printLKP(b.Left());
        cout<<b.RootBT();
        printLKP(b.Right());
    }
}

//---------------------------------------
void printLPK(BinaryTree<char> b) {//Print tree lrR
    if (!b.isNull()) {
        printLPK(b.Left());
        printLPK(b.Right());
        cout<<b.RootBT();
    }
}
//---------------------------------------
