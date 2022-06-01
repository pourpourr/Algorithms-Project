#include "fileHandler.h"
#include "bintree.h"



using std::vector;

void print(std::string str){
    std::cout<<str;
}


void print(long long int num){
    std::cout<<num<<std::endl;
}


void printd(double num){
    std::cout<<num<<std::endl;
}


int int_checker(std::string str_id){
    try{
        int id = stoi(str_id);
        char temp[15];
        sprintf(temp, "%d", id);
        if(strcmp(temp,str_id.c_str())==0)return id;
        else return -1;
    }catch(std::invalid_argument s){
        std::cout << "wrong number input " << '\n';
        return -1;
    }

}


int mod(long long int x , int y){

    if (x>=0) return abs(x%y);
    else return -abs(x%y);
}


vector <double> * getCoordinates( std::string line , std::string * itemId){

    std::istringstream* iss= new std::istringstream(line);

    if(!(*iss>>*itemId)) { delete iss; return NULL;}



    vector<double>* v= new vector<double>;
    std::string cord;
    std::string::size_type sz;

    while(*iss>>cord){
        v->push_back(std::stod(cord,&sz));
    }
    delete iss;

    return v;
}

double dist( vector<double>* a, vector<double>* b ,std::string method){
    if(method=="L2")return euc(a,b);
    if(method=="discrete")return discrete(a,b);
    if(method=="continuous") return continuous(a,b);
    print("wrong method name \n");
    assert(0);

}

double euc( vector<double>* a, vector<double>* b){
    double s=0;
    int x,y;
    if(a->size()!=b->size()){print("dif vector size in euc\n"); assert(0);}
    for(int i=0; i< a->size(); i++){
        x=a->at(i);
        y=b->at(i);
        s+= x*x-2*x*y+y*y;
    }
    return sqrt(s);
}

double smallEuc( double a, double b){
    return sqrt(a*a - 2*a*b +b*b);
}

double max( double a , double b){

    if (a>b)return a;
    return b;
}

double min3(double a , double b , double c){
    if(a<=b && a<=c)return a;
    if(b<=a && b<=c) return b;
    return c;

}

void Sillhouete(int K, vector<vector<std::pair<std::string,vector<double>*>>*>* clusters , vector<double>** means,std::string filename,int loops,std::string method,std::string update,int complete,double time,std::string metric){

    print(clusters->size());
    print(K);
    std::ofstream output_file;
	std::fstream outf(filename);
    if(outf){
        std::cout << "Output file exists,deleting and creating new one" << '\n';
        if( remove( filename.c_str() ) != 0 ){
            std::cout<<"COULD NOT DELETE EXISTING OUTPUT FILE,EXITING"<<std::endl;
            return ;
        }

        output_file.open(filename+".txt");

    }else{
        std::cout<<"Made new outputfile"<<std::endl;
        output_file.open(filename+".txt");
    }
    int totalfound=0;
    output_file<<"LOOPS NEEDED: "<<loops<<std::endl;
    output_file<<"Algorithm: "<<method<<" "<<update<<std::endl;
	for(int i=0;i<clusters->size();i++){
        // if(clusters->size()<4 ){print("egine lathos mikro clust \n");break;}
        totalfound+=clusters->at(i)->size();
		output_file<<"CLUSTER-"<<i<<"{size: "<<clusters->at(i)->size()<<",centroid ";
		for(int j=0;j<means[i]->size();j++){
			output_file<<means[i]->at(j)<<" ";
		}

        output_file<<"}";
		output_file<<std::endl<<std::endl;
	}
    print(totalfound);
    double Sillhouetes[clusters->size()],overall=0.0;
    for(int i=0;i<clusters->size();i++){
        print("clust ");
        print(i);
		Sillhouetes[i] = 0.0;
		double totA=0.0,totB=0.0;

            //    print("eftase1\n");
        int closest ;
		for(int j=0;j<clusters->at(i)->size();j++){
			 closest =-1;
            double mindist=500000.0;
            for(int clust=0;clust<clusters->size();clust++){
                    if(clust!=i){
                        double tempDist = dist(clusters->at(i)->at(j).second,means[clust],metric);
                        if(tempDist<mindist){
                            mindist = tempDist;
                            closest = clust;
                        }
                    }
            }
            if(closest==-1){print("egine lathos\n"); break;}
			double a=0.0,b=0.0,tot=0.0;

            for(int element=0;element<j;element++){
					a+=dist(clusters->at(i)->at(j).second,clusters->at(i)->at(element).second,metric);
			}
			tot = 0.0;
			for(int element=0;element<clusters->at(closest)->size();element++){
					b+=dist(clusters->at(i)->at(j).second,clusters->at(closest)->at(element).second,metric);
			}
			totA+= (2*a)/clusters->at(i)->size();
			totB+= b/clusters->at(closest)->size();
		}

		if(totA>totB){
			Sillhouetes[i] =  totB/totA - 1.0;
		}else if(totA<totB){
			Sillhouetes[i] = 1.0 - totA/totB;
		}else{
			Sillhouetes[i] = 0.0;
		}
        overall+= Sillhouetes[i];
	}

	overall= overall/clusters->size();


	//print sillhouette

    if(time>-1){
        output_file<<"Clustering time: "<<time<<std::endl;
    }
	output_file<<"Sillhouete: [";
	for(int i=0;i<clusters->size();i++){

		output_file<<Sillhouetes[i]<<",";
	}
	output_file<<overall<<"]"<<std::endl;
    if(complete){
        for(int i=0;i<clusters->size();i++){
                output_file<<"CLUSTER-"<<i<<"{ ";
                for(int j=0;j<clusters->at(i)->size();j++){
                    output_file<< clusters->at(i)->at(j).first<<" ,";
                }

                output_file<<"}";
                output_file<<std::endl<<std::endl;
            }
    }

}




double discrete( vector<double>* a, vector<double>* b){ //same dimensions
    if(a->size()!=b->size())print("egine lathos\n");
    double s=0;
    int dim=a->size();
    double** C= new double*[a->size()];
    for (int i =0; i< dim; i++){
        C[i]= new double[a->size()];
    }
    C[0][0]=smallEuc(a->at(0),b->at(0));

    for (int i =1; i< dim; i++){
        C[i][0]= max(C[i-1][0] , smallEuc(a->at(i), b->at(0)) );
    }
    for (int i =1; i< dim; i++){
        C[0][i]= max(C[0][i-1] , smallEuc(b->at(i), a->at(0)) );
    }

    for (int i =1; i< dim; i++){
        for (int j =1; j< dim; j++){
            C[i][j]= max(min3(C[i-1][j-1],C[i-1][j],C[i][j-1]),smallEuc(a->at(i),b->at(j)));
        }
    }
    s=C[dim-1][dim-1];

    for (int i =0; i< dim; i++){
        delete[] C[i];
    }
    delete[] C;
    return s; //ftiajto

}

double continuous( vector<double>* a, vector<double>* b){
    Curve c1(a->size());
    Curve c2(a->size());
    for(int i=0; i<a->size();i++){
        Point p1(1);
        p1.set(0,a->at(i));
        c1.push_back(p1);
        Point p2(1);
        p2.set(0,b->at(i));
        c2.push_back(p2);
    }


    double s= (Frechet::Continuous::distance(c1,c2)).value;


    return s;


}


vector<pair<std::string,vector<double>*>>* getTrueNear(vector<double>* coord ,  vector<pair<std::string,vector<double>*>> &vectorSpace , int numOfnearest , std::string method){


    vector<pair<std::string,vector<double>*>>* neighboors= new vector<pair<std::string,vector<double>*>>(numOfnearest+1);
    vector<double> minDist(numOfnearest+1,1000000+0.1);
    double tempDist;
    for(int i=0;  i<vectorSpace.size() ; i++){
       tempDist=dist(coord, vectorSpace.at(i).second,method);

        if(tempDist<minDist.at(numOfnearest-1)){
            for(int j=0; j<numOfnearest;j++){
                if(tempDist<minDist.at(j)){
                    minDist.insert(minDist.begin()+j,tempDist);
                    neighboors->insert(neighboors->begin()+j,vectorSpace.at(i));
                    minDist.pop_back();
                    neighboors->pop_back();
                    break;
                }
            }
        }
    }///if neigh ==null....

    return neighboors;

}


vector<double>** getKmeansCentroids(vector<double>** centroids,vector<std::pair<std::string,vector<double>*>> vectorSpace , int K , std::string method){

    centroids= new vector<double>*[K];
    int pos = rand()%vectorSpace.size();
    centroids[0] = vectorSpace.at(pos).second;
    vectorSpace.erase(vectorSpace.begin()+pos);

    for(int i=1;i<K;i++){
        double* D= new double[vectorSpace.size()];
        double minDist,tempDist,maxDist=0;

        for(int j=0; j<vectorSpace.size(); j++){
            minDist=dist(vectorSpace.at(j).second,centroids[0],method);
            for(int u=1; u<i; u++){
				tempDist=dist(vectorSpace.at(j).second,centroids[u],method);
				if(tempDist<minDist){
					minDist=tempDist;
				}
			}
            D[j]=minDist*minDist;
            if(D[j]>maxDist)maxDist=D[j];
        }

        double* P= new double[vectorSpace.size()];
        P[0]=D[0]/maxDist;
        for(int j=1; j<vectorSpace.size(); j++){
            P[j]=P[j-1]+D[j]/maxDist;
        }
        double x=P[0]+ rand()/(RAND_MAX/ (P[vectorSpace.size()-1]-P[0]));
        bool flag=false;

        int a=0;
        int b=vectorSpace.size();
        do{
            pos= (a+b)/2;
            if(pos!=0){
                if(P[pos]>=x && P[pos-1]<= x){
                    flag=true;
                }else if(P[pos]>x) b=pos;
                else a=pos;

            }
        }while(!flag);
        centroids[i]=vectorSpace.at(pos).second;
        vectorSpace.erase(vectorSpace.begin()+pos);
        delete[] D;
        delete[] P;
    }
    return centroids;

}


vector<pair<std::string,vector<double>*>>* getInRadius(vector<double>* coord ,  vector<pair<std::string,vector<double>*>> &vectorSpace , int radius ,std::string method){

    vector<pair<std::string,vector<double>*>>* neighboors= new vector<pair<std::string,vector<double>*>>;

    for(int i=0;  i<vectorSpace.size() ; i++){
        if(dist(coord, vectorSpace.at(i).second,method)<radius) neighboors->push_back(vectorSpace.at(i));
    }
    return neighboors;

}


vector<std::pair<std::string,vector< double >>>* pairThem( vector<pair<std::string,vector<double>*>> vectorSpace , double d ){

    vector<vector<std::pair<double , double >>> pairs;
        double t= rand() / ( RAND_MAX / d ) ;

    for(int i=0; i<vectorSpace.size(); i++){
        vector<std::pair<double , double >>temp;
        double a,b;
        int same=0;
        temp.push_back(std::make_pair(delta(d,0+t),delta(d, vectorSpace.at(i).second->at(0)+t))); //+t kapou edw
        int last=0;
        for(int j=1; j<vectorSpace.at(0).second->size(); j++){
            a=delta(d,j+t);
            b=delta(d, vectorSpace.at(i).second->at(j)+t);
            if(!comPairs(temp.at(last),a,b)) {last++; temp.push_back(std::make_pair(a,b));}
            else same++;

        }
        for(int j=0; j<same; j++){
            temp.push_back(std::make_pair(99999,99999));
        }
        pairs.push_back(temp);
    }
    vector<std::pair<std::string,vector< double >>>* final = new vector<std::pair<std::string,vector< double >>>;
    for(int i=0; i<pairs.size(); i++){
        vector<double> temp;
        for(int j=0; j<pairs.at(i).size();j++){
            temp.push_back(pairs.at(i).at(j).first);
            temp.push_back(pairs.at(i).at(j).second);
        }
        final->push_back(std::make_pair(vectorSpace.at(i).first,temp));
    }

    return final;
}

std::pair<std::string,vector< double >> pairOne(pair<std::string,vector<double>*> coord, double d , double t){
    vector<std::pair<double , double >>pairs;
    double a,b;
    int same=0;
    pairs.push_back(std::make_pair(delta(d,0+t),delta(d, coord.second->at(0)+t))); //+t kapou edw
    int last=0;
    for(int j=1; j<coord.second->size(); j++){
        a=delta(d,j+t);
        b=delta(d, coord.second->at(j)+t);
        if(!comPairs(pairs.at(last),a,b)) {last++; pairs.push_back(std::make_pair(a,b));}
        else same++;

    }
    for(int j=0; j<same; j++){
        pairs.push_back(std::make_pair(99999,99999));
    }

    vector<double> temp;
    for(int j=0; j<pairs.size(); j++){
        temp.push_back(pairs.at(j).first);
        temp.push_back(pairs.at(j).second);
    }
    return std::make_pair(coord.first,temp);
}

vector<pair<std::string,vector<double>>>* filterThem( vector<pair<std::string,vector<double>*>> vectorSpace , double d , double e ){

    vector<pair<std::string,vector<double>>>* filtered = new vector<pair<std::string,vector<double>>>;
    double t= rand() / ( RAND_MAX / d ) ;
    for(int i=0; i<vectorSpace.size(); i++){
        vector <double>temp;
        double a;
        int same=0;
        temp.push_back(delta(d, vectorSpace.at(i).second->at(0)+t)); //+t kapou edw
        int last=0;
        for(int j=1; j<vectorSpace.at(0).second->size(); j++){

            a=delta(d, vectorSpace.at(i).second->at(j)+t);
            if(temp.at(last)!=a){last++; temp.push_back(a); }
            else same++;

        }
        for(int j=1; j<temp.size()-1; j++){
            if(abs(temp.at(j)-temp.at(j-1))<e && abs(temp.at(j)-temp.at(j+1))<e ) {
                temp.erase(temp.begin()+j);
                j--;
                same++;
            }
        }
        for(int j=0; j<same; j++){
            temp.push_back(99999);
        }
        filtered->push_back(std::make_pair(vectorSpace.at(i).first,temp));
    }

    return filtered;

}


vector<vector<double>*>* filterCurves( vector<vector<double>*>* vectorSpace , double d , double e ){
  vector<vector<double>*>* filtered = new vector<vector<double>*>;
  double t= rand() / ( RAND_MAX / d ) ;
  for(int i=0; i<vectorSpace->size(); i++){
      vector <double>temp;
      double a;
      int same=0;
      temp.push_back(delta(d, vectorSpace->at(i)->at(0)+t)); //+t kapou edw
      int last=0;

      for(int j=1; j<temp.size()-1; j++){
          if(abs(temp.at(j)-temp.at(j-1))<e && abs(temp.at(j)-temp.at(j+1))<e ) {
              temp.erase(temp.begin()+j);
              j--;
              same++;
          }
      }
      for(int j=0; j<same; j++){
          temp.push_back(99999);
      }
      filtered->push_back(vectorSpace->at(i));
  }

  return filtered;

}
vector<double>* filterOneCurve(vector<double>* coord, double d , double e,int max){
    vector<double>* filtered = new vector<double>;
    double a;
    int erased=0;
    for(int j=0; j<coord->size(); j++){
        filtered->push_back(coord->at(j));
    }
    for(int j=1; j<filtered->size()-1 && (coord->size()-erased>max); j++){
        if(abs(filtered->at(j)-filtered->at(j-1))<e && abs(filtered->at(j)-filtered->at(j+1))<e ) {
           filtered->erase(filtered->begin()+j);
            // j--;
            erased++;
        }
    }

    // std::cout<<"FILTERED SIZE ON FILTER"<<filtered->size()<<std::endl;
    return filtered;
}



std::pair<std::string,vector< double >> filterOne(pair<std::string,vector<double>*> coord, double d , double t, double e){
    vector<double> filtered;
    double a;
    int same=0;
    filtered.push_back(delta(d, coord.second->at(0)+t)); //+t kapou edw
    int last=0;
    for(int j=1; j<coord.second->size(); j++){
        a=delta(d, coord.second->at(j)+t);
        if(filtered.at(last)!=a){last++; filtered.push_back(a); }
        else same++;

    }
    for(int j=1; j<filtered.size()-1; j++){
        if(abs(filtered.at(j)-filtered.at(j-1))<e && abs(filtered.at(j)-filtered.at(j+1))<e ) {
            filtered.erase(filtered.begin()+j);
            j--;
            same++;
        }
    }
    for(int j=0; j<same; j++){
        filtered.push_back(99999);
    }
    return std::make_pair(coord.first,filtered);
}




bool comPairs(std::pair<double , double > prev,double a , double b  ){
   // print("sdsdsd\n");
    return (prev.first==a )&& (prev.second== b);
}


double delta(double d , double x){
    return int(x/d + 0.5)* d;
}




vector<double>* OptimalTraversal(vector<double>*first,vector<double>*second,int mode){
    if(first->size()==0){
        return second;
    }
    if(second->size()==0){
        return first;
    }
    std::deque<std::pair<int,int>> trav;
    vector<double>* final = new vector<double>,*temp,*temp2;
    double** array = new double*[first->size()];
    for(int i=0;i<first->size();i++){
        array[i] = new double[second->size()];

    }
    for(int i=0;i<first->size();i++){
        for(int j=0;j<second->size();j++){
        array[i][j] = 0.0;
        }

    }
    array[0][0] = smallEuc(first->at(0),second->at(0));
    for(int i=1;i<first->size();i++){
        array[i][0] = max(array[i-1][0],smallEuc(first->at(i),second->at(0)));
    }
    for(int j=1;j<second->size();j++){
        array[0][j] = max(array[0][j-1],smallEuc(first->at(0),second->at(j)));
    }
    for(int i=1;i<first->size();i++){
        for(int j=1;j<second->size();j++){
            array[i][j] = max(min3(array[i-1][j],array[i][j-1],array[i-1][j-1]),smallEuc(first->at(i),second->at(j)));
        }
    }
    int P=first->size()-1,Q = second->size()-1;
    trav.push_front(std::make_pair(P,Q));
    while((P!=0)&&(Q!=0)){
        double temp = min3(array[P-1][Q],array[P][Q-1],array[P-1][Q-1]);
        if(temp==array[P-1][Q]){
            trav.push_front(std::make_pair(P,Q));
             P=P-1;
        }else if(temp==array[P][Q-1]){
            trav.push_front(std::make_pair(P,Q));
            Q=Q-1;
        }else{
            trav.push_front(std::make_pair(P,Q));
            P=P-1;
            Q=Q-1;
        }
    }

    //create the optimum path with MDFC
    for(int i=0;i<trav.size();i++){
        final->push_back(MDFC(first->at(trav.at(i).first-1),second->at(trav.at(i).second-1)));
    }


    double d = first->size(),e=10.0;
    if(mode){
        temp = filterOneCurve(final,d,e,first->size());
        while(temp->size()>first->size()){
            temp2=temp;
            temp = filterOneCurve(temp,d,e,first->size());
            delete temp2;
            e = e*2;
        }
        final->clear();
        for(int i=0;i<temp->size();i++){
            final->push_back(temp->at(i));
        }
        delete temp;
    }
    for(int i=0;i<first->size();i++){
         delete[] array[i];
    }
    delete[] array;

    return final;
}

double MDFC(double first,double second){
    return (first+second)/2 ;

}

vector<double>* Mean_Frechet_Update(vector<std::pair<std::string,vector<double>*>>* cluster){
    if(cluster->size()!=0){
        bintree* temptree = new bintree(cluster);
        vector<double>* newmean = temptree->mean();
        return newmean;
    }else{
       return NULL;
    }
}
