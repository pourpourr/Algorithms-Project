#ifndef CUBE 
#define CUBE
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <bitset>
#include <random>
class bitset{
    public:
        int size;
        int *array;
        int bitsize;
    bitset(int);
    int getint();
    void set(int);
    int check(int);
    int checkint(int );
    ~bitset();
    void print();
    std::string	getasstring();
};
class Cube{
    public:
        bitset** table;
        int size;
        int bitsize;
        Cube(int size,int bitsize);
        void set(int,int);
        int check(int,int);
        ~Cube(); 
        void print();
        int getint(int bucket);
        void randomset();
        int getbucket(std::string bits);
        int getHamming(std::string bit,int position,int maxdist);
};

#endif