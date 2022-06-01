#include "bintree.h"
#include "fileHandler.h"
bintree::bintree(vector<std::pair<std::string,vector<double>*>>* clust){
    this->clust = clust;

}

vector<double>* bintree::mean(){
    vector<double>* temp;
    vector<vector<double>*>* temparray = new vector<vector<double>*>;
    std::set<int> tempset;
    srand(time(0));
    
    for(int i=0;i<this->clust->size();i++){
        int pos = rand()%this->clust->size();
        if(!tempset.count(pos)){
            tempset.insert(pos);
            temparray->push_back(this->clust->at(pos).second);
        }else{
            while((!tempset.count(pos))&&(pos<this->clust->size())){
                pos++;
            }
            if(pos==this->clust->size()){
                while((!tempset.count(pos))&&(pos>-1)){
                    pos--;
                }
            }
            if(pos!=-1){
                tempset.insert(pos);
                temparray->push_back(this->clust->at(pos).second);
            }
        }
    }
    vector<double>* tempmean;
    vector<vector<double>*>* newarray = new vector<vector<double>*>,*tempos;
    double d=clust->at(0).second->size(),e=10.0;
    while(temparray->size()>1){
         newarray->clear();   
        tempmean = NULL;    
        for(int j=0;j<temparray->size()-1;j=j+2){
            if(j>temparray->size()){

            }else if(j+1 > temparray->size()){
                tempmean = temparray->at(j);//edw
                newarray->push_back(tempmean);
                
            }else{
                tempmean = OptimalTraversal(temparray->at(j),temparray->at(j+1),1);//na einai idies diastaseis me to arxiko
                newarray->push_back(tempmean);
            } 
        }

        if(temparray->size()!=clust->size()){//2nd loop and after
          for(int j=0;j<temparray->size();j++){
              delete temparray->at(j);
         }
        }

        temparray->clear();
        tempos= newarray;
        newarray = temparray;
        temparray = tempos;
        
    }
     for(int j=1;j<temparray->size();j++){
              delete temparray->at(j);
         }
         delete newarray;
    temp= temparray->at(0);
    delete temparray;
    return temp;//h xwris delete temparray,return temparray->at(0);
}