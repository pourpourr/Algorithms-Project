#include "cubefun.h"

std::vector<std::pair<std::string,std::vector<double>*>>* cuberange(int M, int d, int probes, std::vector<std::pair<std::string, std::vector<double> *>> *vectorSpace, std::vector<double>*query, std::unordered_map<int, int> *map, Cube *mycube,int R,std::vector<hfunc*>* hashes,std::vector<std::vector<std::pair<std::string,std::vector<double>*>>*> * table)
{
   std::vector<std::pair<std::string,std::vector<double>*>>* results = new std::vector<std::pair<std::string,std::vector<double>*>>;
   std::vector<std::pair<std::string,std::vector<double> *>> *neighbors = new std::vector<std::pair<std::string,std::vector<double> *>>;
   std::vector<std::pair<std::string,std::vector<double> *>> *temp = new std::vector<std::pair<std::string,std::vector<double> *>>;
   int checked=0,probeschecked=0;
    std::string bitstring = "";
    for (int hashfun = 0; hashfun < d; hashfun++)
    {
        int tempvalue = hashes->at(hashfun)->h(query); //edw
        int flip = rand() % 2;
        auto result = map->find(tempvalue);
        int finalbit;
        if (result != map->end())
        {
            finalbit = result->second;
        }
        else
        {
            std::pair<int, int> temppair = {tempvalue, flip};
            map->insert(temppair);
            finalbit = flip;
        }
        bitstring += std::to_string(finalbit);
    }
    int actualbucket = mycube->getbucket(bitstring);
    if (actualbucket != -1){
        for (int j = 0; j < table->at(actualbucket)->size(); j++)
        {

            results->push_back(table->at(actualbucket)->at(j));
            checked++;
            if (checked == M)
            {
                break;
            }
        }
        int j = 1, bucket, maxdist = 1;
        while (checked < M && probeschecked < probes)
        {
            if(maxdist==hashes->size()){//hashes->size is equal to the max hamming distance allowed between the buckets
                break;
            }
            bucket = mycube->getHamming(bitstring, j, maxdist);
            if (bucket != -1)
            {
                for (int i = 0; i < table->at(bucket)->size(); i++)
                {

                    results->push_back(table->at(bucket)->at(i));
                    checked++;
                    if (checked == M)
                    {
                        probeschecked++;
                        break;
                    }
                }
                probeschecked++;
                j++;
            }
            else
            { //get higher hamming distance
                j = 0;
                maxdist += 1;
                bucket = mycube->getHamming(bitstring, j, maxdist);

            }


        }
    //    std::cout<<"SEND RESULTS"<<std::endl;
        for (int i = 0; i < results->size(); i++)
        {
            if(euc(query, results->at(j).second)<R){
                neighbors->push_back(results->at(j));
                results->erase(results->begin() + j);
            }

        }
        
    }else{//no bucket found,return brute force
        delete neighbors;
        neighbors = getInRadius(query,*vectorSpace,R,"L2");
    }
    // std::cout<<"RETURNING NEIGHBORS OF #: "<<neighbors->size()<<std::endl;
    delete results;

    delete temp;
    return neighbors;

    }




    std::vector<std::pair<std::string,std::vector<double>*>>* cubeNnearest(int M, int d, int probes, std::vector<std::pair<std::string, std::vector<double> *>> *vectorSpace, std::vector<double>*query, std::unordered_map<int, int> *map, Cube *mycube,int N,std::vector<hfunc*>* hashes,std::vector<std::vector<std::pair<std::string,std::vector<double>*>>*> * table)
{
   std::vector<std::pair<std::string,std::vector<double>*>>* results = new std::vector<std::pair<std::string,std::vector<double>*>>;
   std::vector<std::pair<std::string,std::vector<double> *>> *neighbors = new std::vector<std::pair<std::string,std::vector<double> *>>;
   std::vector<std::pair<std::string,std::vector<double> *>> *temp = new std::vector<std::pair<std::string,std::vector<double> *>>;
   std::pair<std::string,std::vector<double> *> * coord;
//    std::cout<<"HIIII"<<std::endl;
   int checked=0,probeschecked=0;
    std::string bitstring = "";
    for (int hashfun = 0; hashfun < d; hashfun++)
    {
        int tempvalue = hashes->at(hashfun)->h(query); //edw
        int flip = rand() % 2;
        auto result = map->find(tempvalue);
        int finalbit;
        if (result != map->end())
        {
            finalbit = result->second;
        }
        else
        {
            std::pair<int, int> temppair = {tempvalue, flip};
            map->insert(temppair);
            finalbit = flip;
        }
        bitstring += std::to_string(finalbit);
    }
    int actualbucket = mycube->getbucket(bitstring);
    if (actualbucket != -1){
        // std::cout<<"STARTINSERT"<<std::endl;
        for (int j = 0; j < table->at(actualbucket)->size(); j++)
        {

            results->push_back(table->at(actualbucket)->at(j));
            checked++;
            if (checked == M)
            {
                break;
            }
        }
        int j = 1, bucket, maxdist = 1;
        // std::cout<<"MID"<<std::endl;
        while (checked < M && probeschecked < probes)
        {
            if(maxdist==hashes->size()){//hashes->size is equal to the max hamming distance allowed between the buckets
                break;
            }
            bucket = mycube->getHamming(bitstring, j, maxdist);
            if (bucket != -1)
            {
                for (int i = 0; i < table->at(bucket)->size(); i++)
                {

                    results->push_back(table->at(bucket)->at(i));
                    checked++;
                    if (checked == M)
                    {
                        probeschecked++;
                        break;
                    }
                }
                probeschecked++;
                j++;
            }
            else
            { //get higher hamming distance
                j = 0;
                maxdist += 1;
                bucket = mycube->getHamming(bitstring, j, maxdist);

            }


        }
    //    std::cout<<"SEND RESULTS"<<std::endl;

        for (int i = 0; i < results->size(); i++)
        {
            // std::cout<<" "<<i<<" :"<<results->at(i).first<<" ";

        }

    }else{//no bucket found,return brute force
        delete neighbors;
        neighbors = getTrueNear(query,*vectorSpace,N,"L2");
        delete results;
        delete temp;
        return neighbors;
    }
    //sort
        double leastdist = 10000000000000000.0,tempdist=0.0;
         int closest = -1;
        for (int i = 0; i < results->size(); i++)
        {
            coord = &(results->at(i));
            if(tempdist=euc(query, coord->second)<leastdist){
                closest = i;
                leastdist = tempdist ;
            }

        }
        if(closest!=-1){
            temp->push_back(results->at(closest));
        }else{
            std::cout<<"ERROR IN FINDING CLOSEST"<<std::endl;
            delete neighbors;
            delete results;
            return NULL;
        }
    delete neighbors;
    delete results;
    // delete temp;
    return temp;

    }
