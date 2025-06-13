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
 #include <TSpline.h>
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
 
 
#ifdef JPHYS
	#include "jphysmaster.h"
#endif


using namespace std;

#define STR(x) #x
#define INCLUDE_FILE(x) STR(x)


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

  
  gROOT->cd();
    TFile* FilterTreeFile=nullptr;
    TTree* FilterTree=nullptr;
        if(Inputs.TestInput("OverwriteFilterFile")){
			TString FilterFilename=Inputs.OutFilename;
			FilterFilename.Remove(FilterFilename.Length() - 5);
			FilterFilename.Append("_FilterTree.root");
            FilterTreeFile=new TFile(FilterFilename,"RECREATE");
            FilterTreeFile->cd();
            FilterTree = new TTree("OutTTree01", "FilteredEventTree");
        
            FilterTree->AutoSave();
            
            FilterTree->Branch("Num", &tNum, "Num/s");
            FilterTree->Branch("Ts",  &tTs );
            FilterTree->Branch("Mod", &tMod);
            FilterTree->Branch("Ch",  &tCh);
            FilterTree->Branch("Adc", &tAdc);
        }
    gROOT->cd();
  
  
	Short_t FileI=0;
    Short_t FileN=Inputs.Entries.size();
	long NextRun=0;
	if(FileN)NextRun=Inputs.Entries[0];
	
	
// 	#include "sort_files/CalHistList.h"
	#ifdef SORTFILE1
		#include INCLUDE_FILE(SORTFILE1)
	#endif

	vector<DetHit> HPGe,SiERaw,SidERaw,Solar,LaBr;
	vector<TelescopeHit> SiHits;
	vector<bool> IDGateTest(Inputs.CutGates.size(),false);
	
	if(Inputs.TestInput("MirrorZ"))TelescopeHit::MirrorZ();
	if(Inputs.TestInput("TargetZOffset"))TelescopeHit::TargetZOffset(Inputs.GetInput("TargetZOffset"));
	
    double TelescopeBuildWindow=120;
    if(Inputs.TestInput("TelescopeBuildWindow"))TelescopeBuildWindow=Inputs.GetInput("TelescopeBuildWindow");
	
	cout<<endl;
	
	bool EndOfRun=false;
	bool PartSort=Inputs.TestInput("PartSort");
	
	// Main Loop 
	 
    for(long jentry=0;jentry<nentries;jentry++){
        DataChain->GetEntry(jentry);
		
// // 		//Quick hard coded test
// // 		if(!(((( tMod->at(0) )==2)||(( tMod->at(0) )==3)) && (tCh->at(0))<16 ))continue;
		
		if(PartSort){  
				if(jentry>=nentries/10)jentry=nentries-1;//do the last loop steps
		}
		
		EndOfRun=false;
		if(jentry+1==NextRun){
			if(FileI+1<FileN){
				FileI++;
				NextRun=Inputs.Entries[FileI];
			}
			EndOfRun=true;//Indicates the final event of a runfile			
		}
		
		// Clear loop vectors
		HPGe.clear(); 
		SiERaw.clear();
		SidERaw.clear();
		Solar.clear();
		LaBr.clear();
		SiHits.clear();
		
		double Time_i=0;
        UShort_t Nhit=tNum;
        for(UShort_t i=0;i<Nhit;i++){  // Loop over hits from event vector and load into detector vectors
			
			
			if(i==0){
				Time_i=0;
			}else{
				Time_i=(*tTs)[i];
			}
			
			DetHit hit(Time_i,(*tAdc)[i],(*tMod)[i],(*tCh)[i]);
// 			DetHit hit((*tTs)[i],(*tAdc)[i],(*tMod)[i],(*tCh)[i]);
					
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


// 		#include "sort_files/CalLoop.h"
		#ifdef SORTFILE2
			#include SORTFILE2
		#endif
		
		if(jentry%10000==0){
		cout<<setiosflags(ios::fixed)<<std::setprecision(2)<<100.*(double)jentry/nentries<<" % complete"<<"\r"<<flush;
		}

		if(EndOfRun){
			if(FilterTreeFile)FilterTreeFile->Write(0,TObject::kWriteDelete);//Avoid those annoying multiple keys
			
			// End of last run
			if(jentry+1==nentries){
				
			}
		}
			
	}

	gROOT->cd();
	
	if(FilterTreeFile){
		FilterTreeFile->Write(0,TObject::kWriteDelete);//Avoid those annoying multiple keys
		if(FilterTreeFile)FilterTreeFile->Close();
	}

	out.Write();
	out.Close();

	return 0;
}
