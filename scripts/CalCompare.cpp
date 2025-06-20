// Small program to compare channel by channel possible calibrations, not compiled by default
// g++ `root-config --cflags` -o CalCompare scripts/CalCompare.cpp -I. -I./include `root-config --glibs` -L./bin -lSurrogateSort 

#include <TApplication.h>
#include <TInterpreter.h>
#include <TSystem.h>
#include <TROOT.h>
 #include <TFile.h>
 #include <TH1.h>
 #include <TH2.h>
 #include <TH3.h>
 #include <TProfile.h>
 #include <TGraphErrors.h>
 #include <TF1.h>
 #include <TMath.h>
 #include <TVector3.h>
 #include <TTree.h>
 #include <TRandom2.h>
 #include <TChain.h>
 #include <TCanvas.h>
 #include <iostream>
 #include <fstream>
 #include <bitset>
 #include <iomanip>
 #include <vector>
 #include <sstream>
#include "TPad.h"
 
 #include "detectors.h"
 #include "IO.h"


using namespace std;



int main(int argc, char *argv[]){

	int dummy=1;
	TApplication *app = new TApplication("app", &dummy, argv);

	if(argc<2)return 1;
	
	vector<TFile*> InFiles;
	
	for(int i=1;i<argc;i++){
		TString Opt(argv[i]);
		if(Opt.EndsWith(".root")){
			TFile* in=new TFile(Opt,"READ");
			if(in->IsOpen()){
				InFiles.push_back(in);
			}else{
				delete in;
			}
		}
	}
	
	int Nf=InFiles.size();
	
	if(Nf<2)return 0;
	if(Nf>4)return 0;
	
	TCanvas* Canvas=new TCanvas("","",1920,1080);
	
	if(Nf>2){
		Canvas->Divide(2,2);
		
	}else Canvas->Divide(2,1);
	
	string titles[2][2]={{"dE_A","E_A"},{"dE_B","E_B"}};
	
	for(int j=0;j<2;j++){ // loop A B	
		
		TString HBase;
		if(j==0)HBase="Calibrated/dEdX_E_at_dE_SumCalibrated3HeAv_A";
		if(j==1)HBase="Calibrated/dEdX_E_at_dE_SumCalibrated3HeAv_B";
// 		if(j==0)HBase="Calibrated/dEdX_E_at_dE_SumCalibrated3He_A";
// 		if(j==1)HBase="Calibrated/dEdX_E_at_dE_SumCalibrated3He_B";
		
		
		for(int k=0;k<2;k++){// Loop dE E
			for(int i=0;i<16;i++){

				TString HStr;
				
				if(k==0&&j==0)HStr=Form("Calibrated/dE_Chan_cal/dedxmid_dE_calA_%d",i);
				if(k==0&&j==1)HStr=Form("Calibrated/dE_Chan_cal/dedxmid_dE_calB_%d",i);
				if(k==1&&j==0)HStr=Form("Calibrated/E_Chan_cal/dedxmid_E_calA_%d",i);
				if(k==1&&j==1)HStr=Form("Calibrated/E_Chan_cal/dedxmid_E_calB_%d",i);
				
				for(int n=0;n<Nf;n++){
					Canvas->cd(n+1);
					gPad->SetLogz();
					TH2* Hb=(TH2*)InFiles[n]->Get(HBase);
					TH2* Hist=(TH2*)InFiles[n]->Get(HStr);
					if(Hist&&Hb){
						Hist->Draw("col");
						Hb->Draw("same");
						gPad->Update();
					}
				}
				
				
				
// 				TString("A "
				stringstream tt;
				tt<<titles[j][k]<<" "<<i;
					Canvas->SetTitle(tt.str().c_str());

				Canvas->cd();
				Canvas->WaitPrimitive();

			} // loop A B
		} // Loop dE E
	} // i index
	
	
	
	return 0;
}
