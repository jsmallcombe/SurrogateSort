#ifndef SurrogateSortIOHead
#define SurrogateSortIOHead

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TSystem.h>
#include <TString.h>
#include <TKey.h>
#include <TCutG.h>
#include <filesystem>

using namespace std;

class SurrogateSortIO{
    
    public:
    static void WriteCalibration(string filename="cal.txt");
    static void ReadCalibration(string filename="cal.txt");
    
    private:
	vector<string> store;
    vector<double> NumericInputs;
    vector<TString> NumericInputNames;
	
    public:
        
    vector<TString> InputFiles;
    TString OutFilename;
    vector<long> Entries;
    vector<TCutG*> CutGates;
    vector<UShort_t> GateID;
	
    vector<vector<double>> ArrayInputs;
	

	stringstream infostream;
	void Rewind();
	
	SurrogateSortIO(){};
	SurrogateSortIO(int argc, char *argv[]);	
	virtual ~SurrogateSortIO(){
		for(auto g : CutGates)delete g;
	};
	
	SurrogateSortIO( const SurrogateSortIO &obj){	store=obj.store;Rewind();}//copy constructor
	SurrogateSortIO& operator=(const SurrogateSortIO& obj){//assignment operator
		store=obj.store;Rewind();
		return *this;
	}
	
	void ReadInfoFile(string filename);
	void ProcessInputs();
	void ProcessOption(TString str);

	string ReturnFind(string compare);
	bool IsPresent(string compare);
	double Next(string compare);
	string NextString(string compare);
	void Next(string compare,double &ret){ret=Next(compare);}
	void NextTwo(string compare,double& ret,double& retB);
    
    bool TestInput(TString InputName);
    double GetInput(TString InputName);
	
    TChain* DataTree(TString TreeName);

};

//I'm still suprised this worked
// template <typename T>
// SurrogateSortIO& operator>>(SurrogateSortIO& is, T& obj){
// 	is.infostream>>obj;
// 	return is;
// }

//This new doesnt work something something
template <typename T>
stringstream& operator>>(SurrogateSortIO& is, T& obj){
	is.infostream>>obj;
	return is.infostream;
}

template <typename T>
stringstream& operator<<(SurrogateSortIO& is, T& obj){
	is.infostream<<obj;
	return is.infostream;
}


void WriteCal(string filename="cal.txt");
void ReadCal(string filename="cal.txt");

bool stringToInt(const std::string& str, int& result);

TString StripFileName(TString str);
	
#endif
