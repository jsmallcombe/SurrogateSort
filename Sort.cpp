#include <TApplication.h>
#include <TInterpreter.h>
#include <TSystem.h>


// #include <TSystem.h>
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
 
 #include <detectors.h>
 #include <IO.h>

#if defined(CALIBRATE)
	#include "jphysmaster.h"
#endif

using namespace std;


// Should move this to IO.cpp not hardcode
int ModChanList[5]={8,32,32,32,16};

double PhiOffset(double Phi){
    Phi+=TMath::Pi()*0.5;
    if(Phi<0)Phi=TMath::Pi()*2+Phi;
    return Phi;
}



/////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
	SurrogateSortIO Inputs(argc, argv);
	
    int totalchans=0;
    for(int i=0;i<5;i++){
        int t=ModChanList[i];
        ModChanList[i]=totalchans;
        totalchans+=t;
    }

	cout<<endl<<"Opening output file for writing :"<<Inputs.OutFilename<<endl;
	TFile out(Inputs.OutFilename,"RECREATE");
	out.cd();
		TH2D* ChanADC=new TH2D("ChanADC","ChanADC;Channel;ADC Charge",totalchans,0,totalchans,1000,0,8000);
		TH2D* ChanE=new TH2D("ChanE","ChanE;Channel;Energy (MeV)",totalchans,0,totalchans,1000,0,50);
	gROOT->cd();

	TChain* DataChain = Inputs.DataTree("OutTTree01");
	TChain *tree  = DataChain;


  UShort_t          tNum = 0;
  vector <Double_t> *tTs = 0; // [Num]
  vector <UShort_t> *tMod = 0;
  vector <UShort_t> *tCh = 0;
  vector <UShort_t> *tAdc = 0;
  //  vector <UShort_t> tWav= 0;
  Double_t          TsDiff= 0;
  Long64_t          RecordNum = 0;
  //
  tree->SetBranchAddress("Num", &tNum);
  tree->SetBranchAddress("Ts",  &tTs );
  tree->SetBranchAddress("Mod", &tMod);
  tree->SetBranchAddress("Ch",  &tCh);
  tree->SetBranchAddress("Adc", &tAdc);
  long nentries = DataChain->GetEntries();

	Short_t FileI=0;
    Short_t FileN=Inputs.Entries.size();
	long NextRun=0;
	if(FileN)NextRun=Inputs.Entries[0];
	
    #if defined(DEBUG)
		#include "sortcode/DebugHistList.h"
    #elif defined(CALIBRATE)
		#include "sortcode/CalHistList.h"
    #else
        #include "sortcode/DiceHistList.h"
//         #include "sortcode/SortHistogramList.h"
    #endif

	vector<DetHit> HPGe,SiERaw,SidERaw,Solar,LaBr,DICE;
	vector<TelescopeHit> SiHits;
	vector<bool> IDGateTest(Inputs.CutGates.size(),false);
	
	cout<<endl;
	
	bool EndOfRun=false;
	bool PartSort=Inputs.TestInput("PartSort");
	
	// Main Loop 
	 
    for(long jentry=0;jentry<nentries;jentry++){
        DataChain->GetEntry(jentry);
		
// // 		//Quick hard coded test
// // 		if(!(((( tMod->at(0) )==2)||(( tMod->at(0) )==3)) && (tCh->at(0))<16 ))continue;
		
		EndOfRun=false;
		if(jentry+1==NextRun){
			if(FileI+1<FileN){
				FileI++;
				NextRun=Inputs.Entries[FileI];
			}
			EndOfRun=true;//Indicates the final even of a runfile			
		}
		
		// Clear loop vectors
		HPGe.clear(); 
		SiERaw.clear();
		SidERaw.clear();
		Solar.clear();
		LaBr.clear();
		DICE.clear();
		SiHits.clear();

        UShort_t Nhit=tNum;
        for(UShort_t i=0;i<Nhit;i++){  // Loop over hits from event vector and load into detector vectors
			
			DetHit hit((*tTs)[i],(*tAdc)[i],(*tMod)[i],(*tCh)[i]);
					
//             int Mod_i=(*tMod)[i];
//             int Chan_i=(*tCh)[i];
//             double Time_i=(*tTs)[i];
//             double dE=(*tAdc)[i];
			

            ChanE->Fill(ModChanList[hit.Mod()]+hit.Chan(),hit.Energy());
            ChanADC->Fill(ModChanList[hit.Mod()]+hit.Chan(),hit.Charge());
			
			switch(hit.Type()) {
				case DetHit::HPGe:
					HPGe.push_back(hit);
					break;
				case DetHit::LaBr:
					LaBr.push_back(hit);
					break;
				case DetHit::Dice:
					DICE.push_back(hit);
					break;
				case DetHit::SiDeltaE:
					SidERaw.push_back(hit);
					break;
				case DetHit::Si:
					SiERaw.push_back(hit);
					break;
				case DetHit::SiDeltaE_B:
					SidERaw.push_back(hit);
					break;
				case DetHit::Si_B:
					SiERaw.push_back(hit);
					break;
				case DetHit::Solar:
					Solar.push_back(hit);
					break;
				default:
					std::cout << "Unknown type" << std::endl;
					break;
			}
		}
		
			
		#if defined(DEBUG)
			#include "sortcode/DebugLoop.h"
		#elif defined(CALIBRATE)
			#include "sortcode/CalLoop.h"
		#else
			#include "sortcode/DiceSortLoop.h"
// 			#include "sortcode/MainSortLoop.h"
		#endif
		

		if(jentry%10000==0){
		cout<<setiosflags(ios::fixed)<<std::setprecision(2)<<100.*(double)jentry/nentries<<" % complete"<<"\r"<<flush;
		}
			
		if(PartSort){  
				if(jentry==nentries/10)jentry=nentries-2;
		}
	}

	gROOT->cd();

	
	out.Write();
	out.Close();

	return 0;
}
