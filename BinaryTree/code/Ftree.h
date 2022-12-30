
#ifndef Ftree_h
#define Ftree_h

#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

template <class Elem>////Template class
class FTree{
private:
    typedef struct treeNode ///Core structure
    {
        vector<treeNode*> links;//vector of subtrees
        Elem data;//root
    }treeNode;
    treeNode* root;
    ///constructor
public:
    FTree()///Constructor
    {
        root = NULL;
    }

    FTree(treeNode* _root)///Constructor from ref
    {
        root =_root;
    }

    FTree(Elem _el){///Constructor from root only
        root = new treeNode;
        
        root->data=_el;
        
    }

    ///prototypes
    bool isNull() const {
        return root == NULL;
        
    }///Empty tree
    Elem RootFT(){///Root value
        if(this->isNull()){
            cerr << "Error: RootBT(null) \n";
            exit(1);
            
        }
        else
            return root->data;
    }
    
    void destroy(){///Destroy this tree
        if(!isNull()){
            (root->links).clear();
            delete root;
        }
    }
    void addLink(FTree &tree){//add subtree
        (root->links).push_back(tree.root);
    }
    
    vector<FTree<Elem> > getSubtreeVec(){///getting subtree vector
        if(!isNull()){
            auto _links = this->root->links;
            vector<FTree<Elem> > vec;
            for(auto it = _links.begin();it!=_links.end();it++){
                vec.push_back(FTree(*it));
            }
            return vec;
        }else{
            cerr << "Error: getSubtreeVec(null) \n";
            exit(1);
        }
    }
};

#endif /* Ftree_h */
