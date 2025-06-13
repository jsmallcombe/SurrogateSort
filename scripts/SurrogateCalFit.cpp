// g++ `root-config --cflags` -o bin/SurrogateCal scripts/QuickCal.cpp -I. -I./include `root-config --glibs` -L./bin -lSurrogateSort 

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
 
 #include "detectors.h"
 #include "IO.h"


using namespace std;

/////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){

		int dummy=1;
		TApplication *app = new TApplication("app", &dummy, argv);
	
		if(argc<2)return 1;
		
		bool Elastic=false;
		bool View=false;
		bool CalSet=false;
		bool Zero=false;
		bool DoGaus=true;
		bool GausOnly=true;
		bool Prune=true;
		
		if(GausOnly)DoGaus=true;
		
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
			if(Opt=="zero"||Opt=="z"||Opt=="Zero"){
				Zero=true;
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

		TF1* pol2=new TF1("pol2","[0]+[1]*x+[2]*x*x",-1,6000);

		UShort_t Eset[4]={DetHit::SiDeltaE,DetHit::SiDeltaE_B,DetHit::Si,DetHit::Si_B};

		TF1* gaus=new TF1("gaus","gaus",0,6000);
		
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
							TGraphErrors* f=new TGraphErrors();
							
							if(Zero){
								g->SetPoint(0,0,0);
								g->SetPointError(0,5,0);
								if(DoGaus){
									f->SetPoint(0,0,0);
									f->SetPointError(0,5,0);
								}
							}
							
							for(int b=1;b<=py->GetNbinsX();b++){
								double n=py->GetBinContent(b);
								if(n>0){
									
									if(DoGaus){
										TH1*h=Hist->ProjectionX("pX",b,b);
										int binMax = h->GetMaximumBin();               // Bin number of maximum
										double xMax = h->GetBinCenter(binMax);         // X position of maximum
										double yMax = h->GetBinContent(binMax);        // Height of maximum
										double stdDev = h->GetRMS();                   // Standard deviation (RMS)
										gaus->SetParameters(yMax,xMax,stdDev);
										gaus->SetRange(xMax-stdDev,xMax+stdDev);
										h->Fit(gaus,"RQN");
										f->SetPoint(f->GetN(),gaus->GetParameter(1),py->GetBinCenter(b));
										f->SetPointError(f->GetN()-1,gaus->GetParError(1),0);
										g->SetPoint(g->GetN(),gaus->GetParameter(1),py->GetBinCenter(b));
										g->SetPointError(g->GetN()-1,gaus->GetParError(1),0);
										
// 										if(View){
// 											h->Fit(gaus,"R");
// 											gPad->Update();
// 											gPad->WaitPrimitive();
// 										}
										
										delete h;
										if(GausOnly) continue;
									}
									
									double e=py->GetBinError(b);
									if(e<1)e=50;
									if(e<10)e=10;
									g->SetPoint(g->GetN(),n,py->GetBinCenter(b));
									g->SetPointError(g->GetN()-1,e,0);
								}
							}
							pol2->SetParameters(0,0.004,0);
							pol2->SetParLimits(0,-1,1);
							pol2->FixParameter(2,0);
							
// 							pol2->SetParLimits(1,0.00001,0.015);
// 							if(k==0){
// 								pol2->FixParameter(2,0);
// 							}else{
// 								pol2->ReleaseParameter(2);
// 								pol2->SetParLimits(2,-0.001,0.001);
// 							}
							
							g->Fit(pol2,"QN");
							pol2->ReleaseParameter(2);
							g->Fit(pol2,"+R");
							
							if(Prune){
								double x, y;
								int end=0;
								if(Zero){end=1;}
								
								for (int i = g->GetN() - 1; i >= end; --i) {
									g->GetPoint(i, x, y);
									double xerr = g->GetErrorX(i);

									// Estimate expected x for given y (numerical inversion)
									double x_expected = pol2->GetX(y, pol2->GetXmin()-1, pol2->GetXmax());

									if (std::abs(x - x_expected) > 10 * xerr) {
										g->RemovePoint(i);
									}
									double std=std::abs(x - x_expected)/xerr;
								}
								
							}
							
							if(View){
								new TCanvas();
								g->SetPoint(g->GetN(),0,0);
								g->Draw("ap");
								if(DoGaus){
									f->SetLineColor(kRed);
									f->Draw("samep");
								}
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

						
						
						if(j==0&&i<13)continue;
						if(j==1&&i<12)continue;
						
					TString HStr,HHStr,HHHStr;

					if(j==0)HStr=Form("ForCal/ElasticDataInverse/InvCal_dE_A_%d",i);
					if(j==1)HStr=Form("ForCal/ElasticDataInverse/InvCal_dE_B_%d",i);
					
					if(j==0)HHStr=Form("ForCal/ElasticDataInverse/InvCal4He_dE_A_%d",i);
					if(j==1)HHStr=Form("ForCal/ElasticDataInverse/InvCal4He_dE_B_%d",i);
					
					
					if(j==0)HHHStr=Form("ForCal/ElasticDataInverse/Cal_dE_E_Proton_A%d",i);
					if(j==1)HHHStr=Form("ForCal/ElasticDataInverse/Cal_dE_E_Proton_B%d",i);
					
// 					if(j==0)HStr=Form("ForCal/ElasticDataInverse/InvCal_dEComb_A_%d",i);
// 					if(j==1)HStr=Form("ForCal/ElasticDataInverse/InvCal_dEComb_B_%d",i);
					
					TH2* Hist=(TH2*)data.Get(HStr);
					TH2* HistB=(TH2*)data.Get(HHStr);
					TH2* HistC=(TH2*)data.Get(HHHStr);
					
					
					if(!Hist||!HistB||!HistC){
						DetHit::SetCalibrationDirect(Eset[j],i,0,0,0);
						continue;
					}
					Hist->Add(HistB);
					Hist->Add(HistC);
					
					if(Hist->Integral()>100){
						TProfile* py=Hist->ProfileY();
						TGraphErrors* g=new TGraphErrors();
						
						if(Zero){
							g->SetPoint(0,0,0);
							g->SetPointError(0,100,0);
						}
							
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
