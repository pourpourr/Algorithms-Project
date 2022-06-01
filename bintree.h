#ifndef _TREE_
#define _TREE_
#include "definitions.h"
#include "fileHandler.h"
using std::vector;
class bintree{
    public:
        vector<std::pair<std::string,vector<double>*>>* clust;
        vector<double>* mean();
        bintree(vector<std::pair<std::string,vector<double>*>>*);
};
#endif