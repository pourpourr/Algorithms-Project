#include "cubefun.h"
#include "cube_class.h"
#include "fileHandler.h"
#include "hashFunctions.h"
#include "definitions.h"
#include "vHash.h"
#define W 600
#define D 0.1
#define THRESHOLD 1000
using std::string;
using std::endl;
using std::cout;
using std::pair;
// ./test traversal exampleconfig.txt
// ./test discrete exampleconfig.txt
// ./test filter exampleconfig.txt
// ./test pair exampleconfig.txt

int main(int argc,char* argv[]){
    string config = string(argv[2]);
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

    string tested(argv[1]);
    if(tested=="traversal"){
        vector<double>* vector1 = new vector<double>,*vector2= new vector<double>,*temp;
        vector1->push_back(1.0);
        vector1->push_back(2.0);
        vector1->push_back(3.0);
        vector2->push_back(4.0);
        vector2->push_back(5.0);
        vector2->push_back(6.0);
        vector2->push_back(7.0);
        vector2->push_back(8.0);
        temp = OptimalTraversal(vector1,vector2,0);
        cout<<"Starting vectors: "<<endl;
         cout<<"{";
        for(int i=0;i<vector1->size();i++){
            cout<<vector1->at(i)<<", ";
        }
        cout<<"}"<<endl<<"{";
        for(int i=0;i<vector2->size();i++){
            cout<<vector2->at(i)<<", ";
        }
        cout<<"}"<<endl;
        cout<<"Steps calculated: "<<temp->size()<<endl<<"{";
        for(int i=0;i<temp->size();i++){
            cout<<temp->at(i)<<", ";
        }
        cout<<"}"<<endl;
        delete vector1;
        delete vector2;
        return 0;
    }else if(tested=="discrete"){
		vector<double>* vector1 = new vector<double>,*vector2= new vector<double>;

		for(int i=0; i<5; i++){
			vector1->push_back(i);
			vector2->push_back(10-i);
		}
		print("vector1 : ");
		for(int i=0; i<5; i++){
			std::cout<<vector1->at(i)<<" ";
		}
		print("\n");
		print("vector2 : ");
		for(int i=0; i<5; i++){
			std::cout<<vector2->at(i)<<" ";
		}
		print("\n");
		print("For a vector with its self the dist should be 0 \n");
		print("Discrete dist (vector1 , vector1) =  ");
		printd(discrete(vector1,vector1));
		print("\nDiscrete dist (vector1 , vector2) =  ");
		printd(discrete(vector1,vector2));
		delete vector1;
        delete vector2;
        return 0;
    }else if(tested=="filter"){
		vector<double>* vector1 = new vector<double>;

		for(int i=0; i<5; i++){
			vector1->push_back(i+0.5);
			vector1->push_back(i+0.25);
			
		}
		print("vector1 before filtering: ");
		for(int i=0; i<vector1->size(); i++){
			std::cout<<vector1->at(i)<<" ";
		}
		print("\n");
		print("For d = 2 , t=0.25 , e=1, the vector elements should be interger multiples of 2 , and the same elements should be eleminated  \n");
		pair<std::string,vector<double>> temp= filterOne(std::make_pair("mpla",vector1),2,0.25,1);

        print("vector1 after filtering: ");
		for(int i=0; i<temp.second.size(); i++){
			std::cout<<temp.second.at(i)<<" ";
		}
		delete vector1;
    }else if(tested=="pair"){
		vector<double>* vector1 = new vector<double>;

		for(int i=0; i<5; i++){
			vector1->push_back(i+0.5);
			vector1->push_back(i+0.25);
			
		}
		print("vector1 before filtering: ");
		for(int i=0; i<vector1->size(); i++){
			std::cout<<vector1->at(i)<<" ";
		}
		print("\n");
		print("For d = 5 , t=0.25 the vector elements should be interger multiples of 5 , and the same elements should be eleminated \n");
		
		pair<std::string,vector<double>> temp= pairOne(std::make_pair("mpla",vector1),5,0.25);
        print("vector1 after filtering: ");
		for(int i=0; i<temp.second.size(); i++){
			std::cout<<temp.second.at(i)<<" ";
		}
		delete vector1;
        
    }
}