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


using namespace std;

/////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){

		int dummy=1;
		TApplication *app = new TApplication("app", &dummy, argv);
	
		if(argc<2)return 1;
		
		bool Elastic=false;
		bool View=false;
		bool CalSet=false;
		
		string CalIn="";
		string CalOut="";
		TString DataFile="";
		
		for(int i=1;i<argc;i++){
			TString Opt(argv[i]);
			if(Opt=="elastic"||Opt=="e"||Opt=="Elastic"){
				Elastic=true;
			}
			if(Opt=="view"||Opt=="v"||Opt=="View"){
				View=true;
			}
			if(Opt.EndsWith(".cal")||Opt.EndsWith(".Cal")){
				if(!CalIn.size()){
					CalSet=true;
					CalIn=Opt;
					CalOut=Opt;
				}else{
					CalOut=Opt;
				}
			}
			if(Opt.EndsWith(".root")){
				DataFile=Opt;
			}
		}
		
		TFile data(DataFile,"READ");
		
		if(CalSet)ReadCal(CalIn);

		TF1* pol2=new TF1("pol2","[0]+[1]*x+[2]*x*x",0,6000);

		UShort_t Eset[4]={DetHit::SiDeltaE,DetHit::SiDeltaE_B,DetHit::Si,DetHit::Si_B};


		
		if(Elastic){
				
			for(int i=0;i<16;i++){
				for(int k=0;k<2;k++){// Loop dE E
					for(int j=0;j<2;j++){ // loop A B

						TString HStr;

						if(k==0&&j==0)HStr=Form("ForCal/ElasticData/Cal_dE_A_%d",i);
						if(k==0&&j==1)HStr=Form("ForCal/ElasticData/Cal_dE_B_%d",i);
						if(k==1&&j==0)HStr=Form("ForCal/ElasticData/Cal_E_A_%d",i);
						if(k==1&&j==1)HStr=Form("ForCal/ElasticData/Cal_E_B_%d",i);
						TH2* Hist=(TH2*)data.Get(HStr);
						if(!Hist){
							DetHit::SetCalibrationDirect(Eset[j+k*2],i,0,0,0);
							continue;
						}
						
						if(Hist->Integral()>100){
							TProfile* py=Hist->ProfileY();
							TGraphErrors* g=new TGraphErrors();
	// 						g->SetPoint(0,0,0);
							for(int b=1;b<=py->GetNbinsX();b++){
								double n=py->GetBinContent(b);
								if(n>0){
									double e=py->GetBinError(b);
									if(e<1)e=50;
									if(e<10)e=10;
									g->SetPoint(g->GetN(),n,py->GetBinCenter(b));
									g->SetPointError(g->GetN()-1,e,0);
								}
							}
							pol2->SetParameters(0,0.004,-1E-8);
							pol2->SetParLimits(0,-12,12);
							pol2->SetParLimits(1,0.00001,0.015);
							if(k==0){
								pol2->FixParameter(2,0);
							}else{
								pol2->ReleaseParameter(2);
								pol2->SetParLimits(2,-0.0001,0.0001);
							}
							g->Fit(pol2,"+R");
							
							if(View){
								new TCanvas();
								g->SetPoint(g->GetN(),0,0);
								g->Draw("ap");
								gPad->Update();
								gPad->WaitPrimitive();
							}
							if(CalSet)DetHit::SetCalibrationDirect(Eset[j+k*2],i,pol2->GetParameter(0),pol2->GetParameter(1),pol2->GetParameter(2));
							delete py;
							delete g;
						}else{
							if(CalSet)DetHit::SetCalibrationDirect(Eset[j+k*2],i,0,0,0);
						}
					}
				}
			}
		}else{
				
			for(int i=0;i<16;i++){
				for(int j=0;j<2;j++){ // loop A B

					TString HStr;

// 					if(j==0)HStr=Form("SiliconInverseCal/InvCal_dE_A_%d",i);
// 					if(j==1)HStr=Form("SiliconInverseCal/InvCal_dE_B_%d",i);
					if(j==0)HStr=Form("SiliconInverseCal/InvCal_dEComb_A_%d",i);
					if(j==1)HStr=Form("SiliconInverseCal/InvCal_dEComb_B_%d",i);
					
					TH2* Hist=(TH2*)data.Get(HStr);
					if(!Hist){
						DetHit::SetCalibrationDirect(Eset[j],i,0,0,0);
						continue;
					}
					
					if(Hist->Integral()>100){
						TProfile* py=Hist->ProfileY();
						TGraphErrors* g=new TGraphErrors();
// 						g->SetPoint(0,0,0);
						for(int b=10;b<=py->GetNbinsX();b++){
							double n=py->GetBinContent(b);
							if(n>0){
								double e=py->GetBinError(b);
								if(e<1)e=50;
								if(e<10)e=10;
								g->SetPoint(g->GetN(),n,py->GetBinCenter(b));
								g->SetPointError(g->GetN()-1,e,0);
							}
						}
						pol2->SetParameters(0,0.004,-1E-8);
						pol2->SetParLimits(0,-12,12);
						pol2->SetParLimits(1,0.00001,0.015);
						pol2->SetParLimits(2,-0.00001,0.00001);
						g->Fit(pol2,"+R");
						
						
						if(View){
							new TCanvas();
							g->SetPoint(g->GetN(),0,0);
							g->Draw("ap");
							gPad->Update();
							gPad->WaitPrimitive();
						}
							
						if(CalSet)DetHit::SetCalibrationDirect(Eset[j],i,pol2->GetParameter(0),pol2->GetParameter(1),pol2->GetParameter(2));
						delete py;
						delete g;
					}else{
						if(CalSet)DetHit::SetCalibrationDirect(Eset[j],i,0,0,0);
					}
				}
			}
		}
		
		if(CalSet)WriteCal(CalOut);
	return 0;
}
