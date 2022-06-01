#ifndef __VHASH__
#define __VHASH__

#include "definitions.h"
#include "fileHandler.h"
#include "hashFunctions.h"
#define ACCURACY 100


using std::vector;
using std::pair;




class vecHash
{
private:
    vector<vector<vector<pair<std::string,vector<double>*>>*>*>* hash;
    vector<gfunc*>* gf;    
    int k;
    int l;
    int tableSize;
    double d;
    double e;
    double* t;
   
public:
    vecHash(int l,int k ,int dim, int w,  int tbSiz , double d , double e);
    int insert(pair<std::string,vector<double>*> coord);
    int insertAndEdit(pair<std::string,vector<double>*> coord, std::string act );
    vector<pair<std::string,vector<double>*>>* search (vector<double>* coord, int numOfnearest , std::string method);
    vector<pair<std::string,vector<double>*>>* rangeSearch (vector<double>* coord, int radius ,std::string method);
    ~vecHash();
};





#endif