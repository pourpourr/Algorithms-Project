#include "hashFunctions.h"


hfunc::hfunc(int w , int d, double t){
    using namespace std;
    this->w=w;
    this->dim=d;
    this->t = t;
    this->randV= new vector<double>;
    
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<double> distribution(0.0, 1.0);
    for(int i=0; i<dim; i++){
        randV->push_back(distribution(generator));
    }
}


hfunc::~hfunc(){
    delete randV;
}

int hfunc::h(vector<double>* coord){

    double temp=0;
    for(int i=0; i<this->dim; i++){
        temp+= coord->at(i)*this->randV->at(i);

    }
    return abs(int ((temp+this->t)/this->w));

}

gfunc::gfunc(int k , int w , int dim , int tableSize ){

    this->k=k;
    this->h_fun= new vector<hfunc*>;
    this->r = new vector<int>;
    this->max_int= pow(2,31)-5;
    this->tableSize=tableSize;
    for (int i=0; i<k; i++){
        this->h_fun->push_back(new hfunc(w,dim,(rand()/(rand()+1))%w))  ;
        this->r->push_back(rand()%10000);
    }
}

gfunc::~gfunc(){


    for (int i=0; i<k; i++){
        delete this->h_fun->at(i);
    }
    delete this->h_fun;
    delete this->r;

}

int gfunc::g(vector<double>* coord ){


    long int temp=0;
    for (int i = 0; i < k; i++){
      temp=mod( mod(  mod(r->at(i),max_int)  *  mod(h_fun->at(i)->h(coord)  ,max_int ) , max_int) + temp,max_int); ///mods mods mods
    }


    return abs(mod(temp,tableSize));


}

int gfunc::id(vector<double>* coord ){

    long int temp=0;
    for (int i = 0; i < k; i++){
      temp=mod( mod(  mod(r->at(i),max_int)  *  mod(h_fun->at(i)->h(coord)  ,max_int ) , max_int) + temp,max_int); ///mods mods mods
    }
    return temp;


}
