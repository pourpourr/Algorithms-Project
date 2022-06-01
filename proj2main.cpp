#include "cubefun.h"
#include "cube_class.h"
#include "fileHandler.h"
#include "hashFunctions.h"
#include "definitions.h"
#include "vHash.h"
#define W 400
#define THRESHOLD 100
using std::string;
using std::endl;
using std::cout;
using std::pair;
/* ./search –i <input file> –q <query file> –k <int> -L <int> -M <int> -probes
<int> -ο <output file> -algorithm <LSH or Hypercube or Frechet> -metric <discrete
or continuous | only for –algorithm Frechet> -delta <double>
./search -i nasdaq2017_LQ.csv -q nasdaq2017_LQ.csv -k 3 -L 5 -M 10 -probes 2 -o proj2out -algorithm LSH -delta 1
./search -i nasdaq2017_LQ.csv -q nasdaq2017_LQ.csv -k 3 -L 5 -M 10 -probes 2 -o proj2out -algorithm Hypercube -delta 5.0
*/
int main(int argc,char* argv[]){


    if(strcmp(argv[1],"-i")!=0 || strcmp(argv[3],"-q")!=0|| strcmp(argv[5],"-k")!=0|| strcmp(argv[7],"-L")!=0|| strcmp(argv[9],"-M")!=0|| strcmp(argv[11],"-probes")!=0){
        std::cout << "Wrong input ,give \n  ./search –i <input file> –q <query file> –k <int> -L <int> -M <int> -probes <int> -ο <output file> -algorithm <LSH or Hypercube or Frechet> -metric <discrete or continuous | only for –algorithm Frechet> -delta <double> " << '\n';
        return -6;

    }

    if( argc==20){
        if (  strcmp(argv[17],"-metric")!=0  || strcmp(argv[19],"-delta")!=0  )  {
            std::cout << "Wrong input " << '\n';
            return -6;
        }
    }

    if( argc ==18){
        if (  strcmp(argv[17],"-delta")!=0 )  {
            std::cout << "Wrong input " << '\n';
            return -6;
        }
    }

    string filename = string(argv[2]),queryfile = string(argv[4]),outputfile = string(argv[6]),method = string(argv[16]),metric="L2"; //default value for metric
    double d ;
    if((method!="Frechet")&&(method!="Hypercube")&&(method!="LSH")){
        cout<<"Please give acceptable method."<<endl;
            return 0;
    }
    if(method=="Frechet"){
        metric = string(argv[18]);
        string delta = string(argv[20]);
        d  = ::atof(delta.c_str());
        if((metric!="discrete")&&(metric!="continuous")){
            cout<<"Please give acceptable metric."<<endl;
            return 0;
        }
    }else{
        string delta = string(argv[18]);
        d  = ::atof(delta.c_str());
    }
    std::string outputname(argv[14]);
    std::ofstream output_file;
    std::fstream input_file(argv[2]);
    if(!input_file){
        std::cout << "Ιnvalid input file name given" << '\n';
        return -5;
    }
    std::fstream query_file(argv[4]);
    if(!query_file){
        std::cout << "Ιnvalid config file name given" << '\n';
        return -5;
    }
    std::fstream outf(argv[12]);
    if(outf){
        std::cout << "Output file exists,deleting and creating new one" << '\n';
        if( remove( argv[12] ) != 0 ){
            std::cout<<"COULD NOT DELETE EXISTING OUTPUT FILE,EXITING"<<std::endl;
            return 0;
        }

        output_file.open(outputname+".txt");

    }else{
        output_file.open(outputname+".txt");
    }


	int K,L,M,probes;
	K=int_checker(std::string(argv[6]));
	L=int_checker(std::string(argv[8]));
	M=int_checker(std::string(argv[10]));
	probes=int_checker(std::string(argv[12]));

    int queries=0;
    double trueaverage=0.0,aproxaverage=0.0,MAF=-9999;
    srand(time(NULL));
	vector<double> * coord  = NULL;
    vector<std::pair<std::string,vector<double>*>>* vectorSpace= new vector<std::pair<std::string,vector<double>*>>;
    std::string itemId;
	string line="";

    while(std::getline(input_file, line)){ // filling our database
        coord= getCoordinates(line, &itemId);

        vectorSpace->push_back(make_pair(itemId,coord));

        coord=NULL;
    }

    //intialize
    //HC
    std::unordered_map<int,int> *map;
    vector<hfunc*>* hashes;
    Cube* mycube;
    vector<vector<std::pair<std::string,vector<double>*>>*> * table;
    //LSH
    int tableSize;
    if(vectorSpace->size()>32) tableSize= vectorSpace->size()/8;// check if small
    else tableSize= tableSize= vectorSpace->size()/2;
    int w=W;
    int dim = vectorSpace->at(0).second->size();
    vecHash* vh ;
    if(method=="Hypercube"){


        int tempvalue;
        mycube = new Cube(pow(2,K),K);
        mycube->randomset();
        hashes = new vector<hfunc*>;
        for(int i=0;i<K;i++){
            int t = rand()%W;
            hfunc* newhash = new hfunc(W,vectorSpace->at(0).second->size(),t);
            hashes->push_back(newhash);
        }
        table = new vector<vector<std::pair<std::string,vector<double>*>>*>;
        for(int loop=0;loop<pow(2,K);loop++){
            vector<std::pair<std::string,vector<double>*>>* tempbucket= new vector<std::pair<std::string,vector<double>*>>;
            table->push_back(tempbucket);
        }

        map = new std::unordered_map<int,int>;

        for(int i=0; i<vectorSpace->size(); i++){
            coord= vectorSpace->at(i).second;

            std::string bitstring = "";
            for(int hashfun=0;hashfun<K;hashfun++){
                tempvalue = hashes->at(hashfun)->h(coord);
                int flip = rand()%2;
                auto result = map->find(tempvalue);
                int finalbit;
                if (result != map->end()) {
                    finalbit = result->second;
                } else {
                    std::pair<int,int> temppair =std::make_pair(tempvalue,flip);
                    map->insert(temppair);
                    finalbit = flip;
                }
                bitstring += std::to_string(finalbit);
                //put value with flip on the map,if it isnt there,if it is,use the map's value,and append it to the string that represents the value (01010101 etc)

            }
            int actualbucket = mycube->getbucket(bitstring);
            if(actualbucket!=-1){//if its -1 that means the no bucket matches the string,so its rejected
                table->at(actualbucket)->push_back(make_pair(vectorSpace->at(i).first,coord));
            }
        }


    }else if(method=="LSH"){

        vh = new vecHash(L, K,dim,w, tableSize ,d ,d/10); //ds n stuff
        for(int i=0; i<vectorSpace->size(); i++){
            vh->insert(vectorSpace->at(i));
        }



    }else{
        vh = new vecHash(L, K,dim,w, tableSize ,d ,d/10); //ds n stuff
        for(int i=0; i<vectorSpace->size(); i++){ //metric oxi snap na mpoune ta alla <3
            vh->insertAndEdit(vectorSpace->at(i),metric);
        }
    }
//
    print("insert done , reading queries\n");
    while(std::getline(query_file, line)){//queries < 100 temp

        coord = getCoordinates(line, &itemId);
        std::vector<std::pair<std::string,std::vector<double>*>>* neigh;
        auto Hstart = std::chrono::system_clock::now();
        if(method=="Hypercube"){
            neigh=cubeNnearest(M, K, probes,vectorSpace, coord,map, mycube, 1, hashes,table);
        }else{
            neigh = vh->search(coord,1,metric);
        }

        //printing
        std::chrono::duration<double> tmethod = std::chrono::system_clock::now() - Hstart;
        vector<pair<std::string,vector<double>*>>* trueneighbors;
        auto tstart = std::chrono::system_clock::now();
        trueneighbors = getTrueNear(coord,*vectorSpace,1,metric);
        std::chrono::duration<double> ttrue = std::chrono::system_clock::now() - tstart;

        double methoDist,truedist;
        if(neigh->size()>0){
            output_file<<"Query: "<<itemId<<std::endl;
            output_file<<"Algorithm: "<<method<<std::endl;
            methoDist = dist(coord,neigh->at(0).second,metric);
            truedist = dist(coord,trueneighbors->at(0).second,metric);
            output_file<<"Aproximate Nearest neighbor-"<<": "<<neigh->at(0).first<<std::endl;
            output_file<<"distanceAproximate: "<<methoDist<<std::endl;
            output_file<<"distanceTrue: "<<truedist<<std::endl;
        }
        delete neigh;
        delete trueneighbors;
        queries++;
        trueaverage+=ttrue.count();
        aproxaverage+=tmethod.count();
        double localaf = methoDist/truedist;
        MAF=max(MAF,localaf);
        delete coord;

    }

    output_file<<std::endl<<"TAproximateAverage: "<<aproxaverage/queries<<std::endl;
    output_file<<"TTrueAverage: "<<trueaverage/queries<<std::endl;
    output_file<<"MAF: "<<MAF<<std::endl;



    query_file.close();
    input_file.close();
    output_file.close();
    print("queries done\n") ;
    if(method=="Hypercube"){

        for(int loop=0;loop<table->size();loop++){
            delete table->at(loop);
        }
        delete table;
        for(int i=0; i<hashes->size(); i++){
            hfunc* temp= hashes->at(i);
            delete temp;
        }
        delete hashes;
        delete mycube;
        delete map;

    }else delete vh;
    for(int i=0; i<vectorSpace->size(); i++){
        coord= vectorSpace->at(i).second;
         delete coord;
    }
    delete vectorSpace;

}
