#include "vHash.h"


vecHash::vecHash(int l,int k, int dim , int w, int tbSiz, double d , double e){

    this->tableSize=tbSiz;
    this->k=k;
    this->l=l;
    this->d=d;
    this->e=e;
    this->t= new double[l];
    this->hash= new vector<vector<vector<pair<std::string,vector<double>*>>*>*>;
    this->gf= new vector<gfunc*>;

    for(int i=0; i<l; i++){  // num of hashes
        t[i]=rand() / ( RAND_MAX / d );
        hash->push_back( new vector<vector<pair<std::string,vector<double>*>>*>);
        gf->push_back(new gfunc(k,w,dim , tableSize));

        for (int j=0; j< tableSize; j++){ // num of buckets
            hash->at(i)->push_back(NULL);
        }

    }
}


vecHash::~vecHash(){

    for(int i=0; i<l; i++){  // num of hashes
        for (int j=0; j< tableSize; j++){ // num of buckets

            delete hash->at(i)->at(j);
        }
        delete gf->at(i);
        delete hash->at(i);
    }
    delete[] t;
    delete hash;
    delete gf;
}

int vecHash::insert(pair<std::string,vector<double>*> coord){

    int bucket;
    for(int i=0; i<l; i++){ 
        bucket= gf->at(i)->g(coord.second);
     
        if(hash->at(i)->at(bucket)==NULL) hash->at(i)->at(bucket)= new vector<pair<std::string,vector<double>*>>;
        hash->at(i)->at(bucket)->push_back(coord);

    
    }


    return 0;

}

int vecHash::insertAndEdit(pair<std::string,vector<double>*> coordOriginal, std::string act ){

    if (act!="discrete" && act!="continuous"){
        print("wrong act given\n");
        assert(0);
    }
    int bucket;
    pair<std::string,vector<double>> coordEdited;
                  // filtering
    for(int i=0; i<l; i++){ 
        if(act=="discrete") coordEdited=pairOne(coordOriginal,this->d,this->t[i]); //snaping  
        else coordEdited=filterOne(coordOriginal,this->d,this->t[i],this->e);
        bucket= gf->at(i)->g(&(coordEdited.second));
        if(hash->at(i)->at(bucket)==NULL) hash->at(i)->at(bucket)= new vector<pair<std::string,vector<double>*>>;
        hash->at(i)->at(bucket)->push_back(coordOriginal);

    
    }


    return 0;

}



vector<pair<std::string,vector<double>*>>* vecHash::search (vector<double>* coord , int numOfnearest , std::string method){
    int bucket;
    bool found;
    vector<pair<std::string,vector<double>*>>* close;
    pair<std::string,vector<double>*> test;
    vector<pair<std::string,vector<double>*>>* neighboors= new vector<pair<std::string,vector<double>*>>(numOfnearest);
    vector<double> minDist(numOfnearest,1000000+0.1);
    double tempDist;

    for(int i=0; i<l; i++){ 
        if(method=="L2")bucket= gf->at(i)->g(coord);
        else if(method=="discrete"){
            
            pair<std::string,vector<double>> coordEdited=pairOne(std::make_pair("mpla",coord),this->d,this->t[i]);
            bucket= gf->at(i)->g(&(coordEdited.second));
        }else{
            pair<std::string,vector<double>> coordEdited=filterOne(std::make_pair("mpla",coord),this->d,this->t[i],this->e);
            bucket= gf->at(i)->g(&(coordEdited.second));
        }
        

        if(hash->at(i)->at(bucket)==NULL) continue;
        close= hash->at(i)->at(bucket);
        for(int j=0;   j<close->size() ; j++){
            tempDist=dist(coord, close->at(j).second,method);
            if(tempDist<minDist.at(numOfnearest-1)){
                for(int u=0; u<numOfnearest;u++){
                    if(close->at(j).first==neighboors->at(u).first)break;
                    if(tempDist<minDist.at(u)){
                        minDist.insert(minDist.begin()+u,tempDist);
                        neighboors->insert(neighboors->begin()+u,close->at(j));
                        minDist.pop_back();
                        neighboors->pop_back();
                        break;
                    }
                }
            }
        }
    }
    for(int i=numOfnearest-1; i>=0; i--){
        if(minDist.at(i)==1000000+0.1)neighboors->pop_back();
    }

    return neighboors;


}


vector<pair<std::string,vector<double>*>>* vecHash::rangeSearch (vector<double>* coord, int radius ,std::string method){

    int bucket;
    bool found;
    vector<pair<std::string,vector<double>*>>* close;
    pair<std::string,vector<double>*> test;
    vector<pair<std::string,vector<double>*>>* neighboors= new vector<pair<std::string,vector<double>*>>;
    double tempDist;

    for(int i=0; i<l; i++){ 
        if(method=="L2")bucket= gf->at(i)->g(coord);
        else if(method=="discrete"){
            
            pair<std::string,vector<double>> coordEdited=pairOne(std::make_pair("mpla",coord),this->d,this->t[i]);
            bucket= gf->at(i)->g(&(coordEdited.second));
        }else{
            pair<std::string,vector<double>> coordEdited=filterOne(std::make_pair("mpla",coord),this->d,this->t[i],this->e);
            bucket= gf->at(i)->g(&(coordEdited.second));
        }

        if(hash->at(i)->at(bucket)==NULL) continue;
        close= hash->at(i)->at(bucket);
        for(int j=0;   j<close->size() ; j++){
            if(dist(coord, close->at(j).second,method)<radius)neighboors->push_back(close->at(j));
        }
    }
    return neighboors;

}
