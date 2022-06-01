#ifndef __HASHF__
#define __HASHF__

#include "definitions.h"
#include "fileHandler.h"
using std::vector;


class hfunc
{
private:
    vector<double>* randV;
    int w;
    double t;
    int dim;
public:
    hfunc(int w , int d, double t);
    ~hfunc();
    int h(vector<double>* coord );
};

class gfunc{

    private:
        vector<int>* r;
        vector<hfunc*>* h_fun;
        int k;
        int max_int;
        int tableSize;
    public:
        gfunc(int k , int w , int d , int tableSize );
        ~gfunc();
        int g(vector<double>* coord );
        int id(vector<double>* coord );
};





#endif
