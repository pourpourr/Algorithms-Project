#include "cube_class.h"
void Cube::randomset(){
	srand(time(NULL));
	for(int i=0;i<this->size;i++){
		for(int k=0;k<this->bitsize;k++){
				if(rand()%2){
					this->set(k,i);
				}
            }
	}

}
int Cube::getint(int bucket){
	return this->table[bucket]->getint();
}
int Cube::getbucket(std::string bits){
	unsigned long long int temp = std::stoll(bits),result=0,b=1;
	while(temp){
		int last = temp%10;
		temp=temp/10;
		result+= last*b;
		b=b*2;
	}
	for(int i=0;i<this->size;i++){
		if(result == this->table[i]->getint()){
			return i;
		}
	}
	return -1;

}
int Cube::getHamming(std::string bits,int position,int maxdistance){
		int pos=1;
		for(int i=0;i<this->size;i++){

            if(strcmp(this->table[i]->getasstring().c_str(),bits.c_str())==maxdistance){
				if(pos>position){
					if(this->getbucket(this->table[i]->getasstring())>-1){
						return this->getbucket(this->table[i]->getasstring()) ;
					}
				}else{
					pos++;
				}

			}
        } 
		return -1;
}
Cube::Cube(int size,int bitsize){

        this->size = size;
        this->table = new bitset*[size];
		this->bitsize = bitsize;
        for(int i=0;i<size;i++){
            this->table[i] = new bitset(bitsize);
        }
    
}
void Cube::set(int bit,int bucket){
    this->table[bucket]->set(bit);
}
int Cube::check(int bit,int bucket){
    return this->table[bucket]->check(bit);
}
Cube::~Cube(){
    for(int i=0;i<this->size;i++){
            delete this->table[i];
        } 
    delete[] this->table;
}
void Cube::print(){
    for(int i=0;i<this->size;i++){
        this->table[i]->print();
    }
}
	bitset::bitset(int bits){
		this->size = bits/32 + ((bits%32)>0);
		this->bitsize = bits;
		//std::cout << "bitset size in bits is " << sz*8 << std::endl;
		this->array = new int [this->size];
		for(int i = 0;i<this->size;i++){
			array[i] = 0;
		}
	}

	void bitset::set(int startpos){
		int i = startpos/32 , offset = startpos%32;
		unsigned int bit = 1;
		bit = bit << offset;
		// std::cout<< "setting bit " << i*32+offset <<" offset "<< offset<<" "<< std::endl;
		array[i] = array[i]^bit;
        
	}
	
	int bitset::check(int startpos){
		int i = startpos/32 , offset = startpos%32;
		unsigned int bit = 1;
		bit = bit << offset;
		// std::cout<< "checking bit " << i*32+offset << std::endl;
		if(array[i]&bit){
			return 1;
		}else{
			return 0;
		}
	}
	int bitset::getint(){
		return this->array[0];
	}
    void bitset::print(){
		if(this->bitsize<32){
			for(int k=0;k<this->bitsize;k++){
                std::cout<<this->check(k)<<" ";
				}
				std::cout<<std::endl;
		}else{

			for(int i=0;i<this->bitsize;i++){
					std::cout<<this->check(i)<<" ";
			}
			std::cout<<std::endl;
		}
        
    }
std::string	bitset::getasstring(){
		std::string temp="";
		
			for(int k=0;k<this->bitsize;k++){
                temp+=std::to_string(this->check(k));
				}
		return temp;
	}
	bitset::~bitset(){
		delete[] this->array;
		
	}

