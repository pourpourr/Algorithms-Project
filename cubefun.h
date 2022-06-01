#ifndef CUBEFUN
#define CUBEFUN

#include "cube_class.h"
#include "definitions.h"
#include "vHash.h"
std::vector<std::pair<std::string,std::vector<double>*>>* cuberange(int M, int d, int probes, std::vector<std::pair<std::string, std::vector<double> *>> *vectorSpace, std::vector<double>* query, std::unordered_map<int, int> *map, Cube *cube,int R,std::vector<hfunc*>* hashes,std::vector<std::vector<std::pair<std::string,std::vector<double>*>>*> * table);
std::vector<std::pair<std::string,std::vector<double>*>>* cubeNnearest(int M, int d, int probes, std::vector<std::pair<std::string, std::vector<double> *>> *vectorSpace, std::vector<double>* query, std::unordered_map<int, int> *map, Cube *cube,int R,std::vector<hfunc*>* hashes,std::vector<std::vector<std::pair<std::string,std::vector<double>*>>*> * table);

#endif