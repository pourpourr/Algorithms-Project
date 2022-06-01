#ifndef __FILHAN__
#define __FILHAN__

#include "definitions.h"
#include "curve.hpp"
#include "frechet.hpp"

using std::vector;
using std::pair;

int int_checker(std::string str_id);

void print(std::string str);

void print(long long int num);

void printd(double num);


std::vector <double> * getCoordinates( std::string line , std::string * itemId);

vector<pair<std::string,vector<double>*>>* getTrueNear(vector<double>* coord ,  vector<pair<std::string,vector<double>*>> &vectorSpace , int numOfnearest , std::string method);

vector<pair<std::string,vector<double>*>>* getInRadius(vector<double>* coord ,  vector<pair<std::string,vector<double>*>> &vectorSpace , int radius ,std::string method);


double euc( vector<double>* a, vector<double>* b);


vector<double>** getKmeansCentroids(vector<double>** centroids,vector<std::pair<std::string,vector<double>*>> vectorSpace , int K , std::string method );

vector<std::pair<std::string,vector< double >>>* pairThem( vector<pair<std::string,vector<double>*>> vectorSpace , double d );

std::pair<std::string,vector< double >> pairOne(pair<std::string,vector<double>*> coord, double d , double t);


vector<pair<std::string,vector<double>>>* filterThem( vector<pair<std::string,vector<double>*>> vectorSpace , double d , double e );

vector<vector<double>*>* filterCurves(  vector<vector<double>*>* vectorSpace , double d , double e );

bool comPairs(std::pair<double , double > prev,double a , double b  );

double delta(double d , double x);

int mod(long long int x , int y);

double smallEuc( double a, double b);

double max( double a , double b);

double min3(double a , double b , double c);
void Sillhouete(int K, vector<vector<std::pair<std::string,vector<double>*>>*>* clusters , vector<double>** means,std::string filename,int loops,std::string method,std::string update,int complete,double t ,std::string metric);

std::pair<std::string,vector< double >> pairOne(pair<std::string,vector<double>*> coord, double d , double t);

std::pair<std::string,vector< double >> filterOne(pair<std::string,vector<double>*> coord, double d , double t, double e);

vector<double>* filterOneCurve(vector<double>* coord, double d , double e,int m);

double continuous( vector<double>* a, vector<double>* b);

double dist( vector<double>* a, vector<double>* b ,std::string method);


double discrete( vector<double>* a, vector<double>* b);
vector<double>* OptimalTraversal(vector<double>*,vector<double>*,int mode);
double MDFC(double,double);
vector<double>* Mean_Frechet_Update(vector<std::pair<std::string,vector<double>*>>*);
#endif
