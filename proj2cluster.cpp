#include "cubefun.h"
#include "cube_class.h"
#include "fileHandler.h"
#include "hashFunctions.h"
#include "definitions.h"
#include "vHash.h"
#define W 600
#define D 0.1
#define THRESHOLD 0.1
using std::string;
using std::endl;
using std::cout;
using std::pair;
/*
    ./cluster -i nasdaq2017_LQ.csv -c exampleconfig.txt -o cluster2out -update Mean_Frechet -assignment Classic -complete -silhouette
*/

/*
./cluster –i <input file> –c <configuration file> -o <output file> -update <Mean
Frechet or Mean Vector> –assignment <Classic or LSH or Hypercube or LSH_Frechet>
-complete <optional> -silhouette <optional>

*/
int main(int argc,char* argv[]){

	if(argc!=13 && argc!=12&&argc!=11){
		std::cout<<argc<<std::endl;
        std::cout << "Wrong num of arguments , give" << '\n';
        std::cout << " ./cluster –i <input file> –c <configuration file> -o <output file> -update <Mean Frechet or Mean Vector> –assignment <Classic or LSH or Hypercube or LSH_Frechet> -complete <optional> -silhouette <optional> " << '\n';
        return -6;
    }
    int complete = 0,sill =0;
    if(strcmp(argv[1],"-i")!=0 || strcmp(argv[3],"-c")!=0|| strcmp(argv[5],"-o")!=0|| strcmp(argv[7],"-update")!=0|| strcmp(argv[9],"-assignment")!=0){
        std::cout << "Wrong input ,give \n ./cluster –i <input file> –c <configuration file> -o <output file> -update <Mean Frechet or Mean Vector> –assignment <LSH_Frechet or LSH or Hypercube or LSH_Frechet> -complete <optional> -silhouette <optional> " << '\n';
        return -6;

    }

    if( argc==13){
        if (  strcmp(argv[11],"-complete")!=0  || strcmp(argv[12],"-silhouette")!=0 || ( strcmp(argv[10],"LSH")!=0 && strcmp(argv[10],"Classic")!=0 && strcmp(argv[10],"Hypercube")!=0  && strcmp(argv[10],"LSH_Frechet")!=0 )||(strcmp(argv[8],"Mean_Frechet")!=0 && strcmp(argv[8],"Mean_Vector")!=0) )  {
            std::cout << "Wrong input ,give \n ./cluster –i <input file> –c <configuration file> -o <output file> -update <Mean Frechet or Mean Vector> –assignment <Classic or LSH or Hypercube or LSH_Frechet> -complete <optional> -silhouette <optional> " << '\n';
            return -6;
        }
        sill=1;
        complete=1;
    }

    if( argc ==12){
        if (    ((strcmp(argv[11],"-complete")!=0)&&(strcmp(argv[11],"-silhouette")!=0)  )|| ( strcmp(argv[10],"LSH")!=0 && strcmp(argv[10],"Classic")!=0 && strcmp(argv[10],"Hypercube")!=0  && strcmp(argv[10],"LSH_Frechet")!=0 )||(strcmp(argv[8],"Mean_Frechet")!=0 && strcmp(argv[8],"Mean_Vector")!=0) )  {
            std::cout << "Wrong input ,give \n ./cluster –i <input file> –c <configuration file> -o <output file> -update <Mean Frechet or Mean Vector> –assignment <Classic or LSH or Hypercube or LSH_Frechet> -complete <optional> -silhouette <optional> " << '\n';
            return -6;
        }
        if(strcmp(argv[11],"-complete")!=0){
            sill = 1;
        }else{
            complete=1;
        }
    }
    if( argc ==11){
        if (   ( strcmp(argv[10],"LSH")!=0 && strcmp(argv[10],"Classic")!=0 && strcmp(argv[10],"Hypercube")!=0  && strcmp(argv[10],"LSH_Frechet")!=0 )||(strcmp(argv[8],"Mean_Frechet")!=0 && strcmp(argv[8],"Mean_Vector")!=0) )  {
            std::cout << "Wrong input ,give \n ./cluster –i <input file> –c <configuration file> -o <output file> -update <Mean Frechet or Mean Vector> –assignment <Classic or LSH or Hypercube or LSH_Frechet> -complete <optional> -silhouette <optional> " << '\n';
            return -6;
        }
    }

    std::fstream confile(argv[4]);
    if(!confile){
        std::cout << "invalid config file name given" << '\n';
        return -5;
    }
	std::fstream input_file(argv[2]);
    if(!input_file){
        std::cout << "Ιnvalid input file name given" << '\n';
        return -5;
    }


	string filename = string(argv[2]),config = string(argv[4]),outputfile = string(argv[6]),update= string(argv[8]),assignment=string(argv[10]);

	cout<<"FILENAME: "<<filename<<" CONFIG: "<<config<<" OUT: "<<outputfile<<" Update: "<<update<<" Assignment: "<<assignment<<" complete: "<<complete<<endl;
	if((assignment!="Classic")&&(assignment!="LSH")&&(assignment!="Hypercube")&&(assignment!="LSH_Frechet")){
		cout<<"Bad assignment"<<endl;
		return 0;
	}
    if((update!="Mean_Frechet")&&(update!="Mean_Vector")){
		cout<<"Bad update"<<endl;
		return 0;
	}

    std::ofstream output_file;
	std::fstream outf(argv[6]);

	std::ifstream configfile(config);
	int K,L,klsh,M,kvec,probes;
	K=-1;//control value,we dont use negatives anyway
	L=3;
	klsh=4;
	M=10;
	kvec =3;
	probes=2;
	string line;
	if(configfile.is_open()){
		while(std::getline(configfile,line)){
			std::istringstream mystream(line);
			string value,quantity;
			if(std::getline(mystream,value,' ')){
				std::getline(mystream,quantity);
				int num = std::stoi(quantity);
				if(value=="number_of_clusters:"){
					K=num;
				}else if(value=="number_of_vector_hash_tables:"){
					L=num;
				}else if(value=="number_of_vector_hash_functions:"){
					klsh=num;
				}else if(value=="max_number_M_hypercube:"){
					M=num;
				}else if(value=="number_of_hypercube_dimensions"){
					kvec=num;
				}else if(value=="number_of_probes"){
					probes=num;
				}else{
					// cout<<"INVALID CONFIG LINE"<<endl;
				}
			}
		}
	}else{
		cout<<"CONFIG FILE COULD NOT BE OPENED"<<endl;
		return 0;
	}
	if(K==-1){
		cout<<"NO VALUE GIVEN FOR K,FIX CONFIG"<<endl;
		return 0;
	}
	cout<<"number_of_clusters: "<<K<<endl<<"number_of_vector_hash_tables: "<<L<<endl<<"number_of_vector_hash_functions: "<<klsh<<endl<<"max_number_M_hypercube: "<<M<<endl<<"number_of_hypercube_dimensions: "<<kvec<<endl<<"number_of_probes: "<<probes<<endl;
    if(((assignment=="LSH"||assignment=="Hypercube")&&(update=="Mean_Frechet"))||((assignment=="LSH_Frechet") && (update=="Mean_Vector"))){
        std::cout<<"Wrong metric for assignment chosen"<<std::endl;
        return -6;
    }
    std::string metric;
    if(update=="Mean_Vector")metric="L2";
    else metric="discrete";
	srand(time(NULL));
	vector<double> * coord  = NULL;
    vector<std::pair<std::string,vector<double>*>>* vectorSpace= new vector<std::pair<std::string,vector<double>*>>;
    std::string itemId;
	line="";
    while(std::getline(input_file, line)){ // filling our database
        coord= getCoordinates(line, &itemId);

        vectorSpace->push_back(make_pair(itemId,coord));
        coord=NULL;
    }

    //k means initialization
    vector<double>** means;
    means= getKmeansCentroids(means,*vectorSpace, K , metric); //centroids of the programm

    int pos ;
    ///////////////////
    vector<vector<std::pair<std::string,vector<double>*>>*>* clusters= new vector<vector<std::pair<std::string,vector<double>*>>*>(K);
    //init for Lloyds
    if(assignment=="Classic") {
        for(int i=0; i<K; i++) clusters->at(i)=new vector<std::pair<std::string,vector<double>*>>;
    }
    ///////////////////
    //initialization of lsh
    int tableSize;
    if(vectorSpace->size()>32) tableSize= vectorSpace->size()/8;// check if small
    else tableSize= tableSize= vectorSpace->size()/2;
    int w=W;
    double d=D;
    int dim = vectorSpace->at(0).second->size();
    vecHash* vh ;

    if(assignment=="LSH"){
        vh = new vecHash(L, klsh,dim,w, tableSize,0,0); //ds n stuff
        for(int i=0; i<vectorSpace->size(); i++){
            vh->insert(vectorSpace->at(i));
        }
    }
    if(assignment=="LSH_Frechet"){
        vh = new vecHash(L, klsh,dim,w, tableSize,d,d/10);
        for(int i=0; i<vectorSpace->size(); i++){
            vh->insertAndEdit(vectorSpace->at(i),metric);
        }
    }
    ////////////////////
    //initialization of hypercube
    std::unordered_map<int,int> *map;
    vector<hfunc*>* hashes;
    Cube* mycube;
    vector<vector<std::pair<std::string,vector<double>*>>*> * table;
    int tempvalue;
    if(assignment=="Hypercube"){

		mycube = new Cube(pow(2,kvec),kvec);
        mycube->randomset();
        hashes = new vector<hfunc*>;
        for(int i=0;i<kvec;i++){
            int t = rand()%W;
            hfunc* newhash = new hfunc(W,vectorSpace->at(0).second->size(),t);
            hashes->push_back(newhash);
        }

        table = new vector<vector<std::pair<std::string,vector<double>*>>*>;
        for(int loop=0;loop<pow(2,kvec);loop++){
            vector<std::pair<std::string,vector<double>*>>* tempbucket= new vector<std::pair<std::string,vector<double>*>>;
            table->push_back(tempbucket);
        }

        map = new std::unordered_map<int,int>;
		for(int i=0; i<vectorSpace->size(); i++){
            coord= vectorSpace->at(i).second;

            std::string bitstring = "";
            for(int hashfun=0;hashfun<kvec;hashfun++){
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
    }
    //////////////
    std::map<std::string, vector<double>* >rangeClusters[K];
    vector<std::pair<std::string,vector<double>*>>* neigh;
    bool found;

    int diff = 500000;
    int radius=250,iterations;
    int loops=0;
    int newNeigh;
    bool bruteforce=false;
    int threshold = THRESHOLD*vectorSpace->size();
    print("done inserting \n");
//assignment
    auto tstart = std::chrono::system_clock::now() ;
    while(diff>threshold&& radius<20000){
        print("\n");
        if(assignment=="Classic"){
            diff=0;
            for(int i=0;i<vectorSpace->size();i++){
                int closest=-1;
                double closestdist=500000.0;
                std::pair<std::string,vector<double>*> vec = vectorSpace->at(i);
                for(int j=0;j<K;j++){
                    double tempDist =dist(vec.second,means[j],metric);
                    if(tempDist<closestdist){
                        closestdist = tempDist;
                        closest = j;
                    }
                }
                if(closest!=-1){
                    if(loops!=0){
                        for(int j=0;j<K;j++){
                            for(int i=0;i<clusters->at(j)->size();i++){
                                if(clusters->at(j)->at(i).first == vec.first){
                                    if(closest!=j) diff+=1;
                                    clusters->at(j)->erase(clusters->at(j)->begin() + i);
                                }
                            }
                        }
                    }else diff=500000;
                    clusters->at(closest)->push_back(vec);
                }else{
                    cout<<"no closer found"<<endl;
                }

            }

        }else { //RANGE SEARCH
            newNeigh=0;
			for(int i=0; i<K; i++){

                if(bruteforce)neigh=getInRadius(means[i],*vectorSpace,radius,metric)  ;
                else{
                    if(assignment=="Hypercube") neigh=cuberange(M, kvec, probes,vectorSpace, means[i],map, mycube, radius, hashes,table);
                    else neigh=vh->rangeSearch(means[i],radius,metric);
                }
                // print("EP\n");
                //newNeigh+=neigh->size();
				for(int j=0; j<neigh->size(); j++){
					found=false;
					if(rangeClusters[i].count(neigh->at(j).first)) continue;
					for(int u=0; u<K; u++){

						if ( rangeClusters[u].count(neigh->at(j).first) ){
							found=true;
							if( dist(means[u],neigh->at(j).second,metric)>dist(means[i],neigh->at(j).second,metric)){
								rangeClusters[u].erase(neigh->at(j).first);
								rangeClusters[i][neigh->at(j).first]=neigh->at(j).second;

							}
							break;
						}
					}
					if(!found) {rangeClusters[i][neigh->at(j).first]=neigh->at(j).second;	newNeigh++;}
				}
				delete neigh;
			}
            if(newNeigh<threshold && bruteforce==true) radius=20000;
            if(newNeigh<threshold ) bruteforce=true;
            else bruteforce=false;
            for(int i=0; i<K; i++) {
                if(clusters->at(i))delete clusters->at(i);
                clusters->at(i)=new vector<std::pair<std::string,vector<double>*>>;
                for(auto it = rangeClusters[i].cbegin(); it != rangeClusters[i].cend(); ++it){
                    clusters->at(i)->push_back(std::make_pair(it->first,it->second));
			    }
            }
            std::cout<<std::endl;
        }

        ///means recalculation
        if(update=="Mean_Vector"){
            for(int j=0;j<K;j++){
                if(clusters->at(j)->size()){
                    for(int element=0;element<vectorSpace->at(0).second->size();element++){
                        means[j]->at(element)=0;
                    }
                    for(int i=0;i<clusters->at(j)->size();i++){
                        for(int element=0;element<vectorSpace->at(0).second->size();element++){
                            means[j]->at(element) += clusters->at(j)->at(i).second->at(element);
                        }
                    }
                    for(int i=0;i<means[j]->size();i++){

                        means[j]->at(i) = means[j]->at(i) / clusters->at(j)->size();
                    }
                }
            }

            loops++;
            radius=2*radius;
            // print(radius);
            //while
        }else if(update=="Mean_Frechet"){
            for(int j=0;j<K;j++){
                // delete means[j];
                means[j] = Mean_Frechet_Update(clusters->at(j));
                if(means[j]==NULL){
                    means[j] = new vector<double>;
                    for(int element=0;element<vectorSpace->at(0).second->size();element++){
                        means[j]->push_back(0);
                    }
                }
            }
            loops++;
            radius=2*radius;
        }
    }
    std::chrono::duration<double> totalclustering = std::chrono::system_clock::now() - tstart;
    //silouetes



    print("done clustering \n");
    if(sill){
        print("calculating Silhouetes \n");
        Sillhouete(K,clusters,means,outputfile,loops,assignment,update,complete,totalclustering.count(),metric);
    }else{
        if(outf){
            std::cout << "Output file exists,deleting and creating new one" << '\n';
            if( remove( argv[6] ) != 0 ){
                std::cout<<"COULD NOT DELETE EXISTING OUTPUT FILE,EXITING"<<std::endl;
                return 0;
            }

            output_file.open(outputfile+".txt");

        }else{
            std::cout<<"Made new outputfile"<<std::endl;
            output_file.open(outputfile+".txt");
        }

        int totalfound=0;
        output_file<<"Algorithm: "<<assignment<<"-"<<update<<endl;
        for(int i=0;i<clusters->size();i++){
            totalfound+=clusters->at(i)->size();
            output_file<<"CLUSTER-"<<i<<"{size: "<<clusters->at(i)->size()<<",centroid: ";
            for(int j=0;j<means[i]->size();j++){
                output_file<<means[i]->at(j)<<" ";
            }

            output_file<<"}";
            output_file<<endl<<endl;
        }
        output_file<<"Clustering time: "<<totalclustering.count()<<endl;
        print(totalfound);

        if(complete){
            for(int i=0;i<clusters->size();i++){
                output_file<<"CLUSTER-"<<i<<"{ ";
                for(int j=0;j<clusters->at(i)->size();j++){
                    output_file<< clusters->at(i)->at(j).first<<" ,";
                }

                output_file<<"}";
                output_file<<endl<<endl;
            }
        }

    }
// silhouettes finished
    // query_file.close();
    input_file.close();
    output_file.close();
    if(assignment=="LSH" || assignment=="LSH_Frechet") delete vh;



    if(assignment=="Hypercube"){

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

    }
	for(int i=0;i<clusters->size();i++){
		delete clusters->at(i);
	}
    delete[] means;

	delete clusters;
	for(int i=0; i<vectorSpace->size(); i++){
        coord= vectorSpace->at(i).second;
         delete coord;
    }
    delete vectorSpace;
	return 0;
}
