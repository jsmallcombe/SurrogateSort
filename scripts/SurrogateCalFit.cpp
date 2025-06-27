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
	bool ViewGaus=false;
	bool CalSet=false;
	bool Zero=false;
	bool DoGaus=false;
	bool GausOnly=false;
	bool Prune=false;
	bool Linear=false;
	bool Normalise=false;
	bool Comb=false;
	bool DeltaOnly=false;
	
	
	string CalIn="";
	string CalOut="";
	TString DataFile="";
	
	for(int i=1;i<argc;i++){
		TString Opt(argv[i]);
		if(Opt=="gaus"||Opt=="g"||Opt=="Gaus"){
			DoGaus=!DoGaus;
		}
		if(Opt=="elastic"||Opt=="e"||Opt=="Elastic"){
			Elastic=true;
		}
		if(Opt=="norm"||Opt=="n"||Opt=="Normalise"){
			Normalise=true;
		}
		if(Opt=="view"||Opt=="v"||Opt=="View"){
			View=true;
		}
		if(Opt=="viewgaus"||Opt=="vg"||Opt=="ViewGaus"){
			ViewGaus=true;
		}
		if(Opt=="zero"||Opt=="z"||Opt=="Zero"){
			Zero=true;
		}
		if(Opt=="prune"||Opt=="p"||Opt=="Prune"){
			Prune=true;
		}
		if(Opt=="lin"||Opt=="l"||Opt=="Lin"||Opt=="Linear"){
			Linear=true;
		}
		if(Opt=="comb"||Opt=="c"||Opt=="Comb"){
			Comb=true;
		}
		if(Opt=="delta"||Opt=="d"||Opt=="Delta"){
			DeltaOnly=true;
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
	
	if(DoGaus)GausOnly=true;
	if(GausOnly)DoGaus=true;
	
// 	if(!Elastic){DoGaus=false;ViewGaus=false;GausOnly=false;}
	
	TFile data(DataFile,"READ");
	
	if(CalSet)ReadCal(CalIn);

	TF1* pol2=new TF1("pol2","[0]+[1]*x+[2]*x*x",-1,6000);

	UShort_t Eset[4]={DetHit::SiDeltaE,DetHit::SiDeltaE_B,DetHit::Si,DetHit::Si_B};

	TF1* gaus=new TF1("gaus","gaus",0,6000);
	
	for(int k=0;k<2;k++){// Loop dE E
		if(DeltaOnly&&k>0)continue;
		for(int j=0;j<2;j++){ // loop A B
			for(int i=0;i<16;i++){ 

				TString HStr;
				
				if(Elastic){
					if(k==0&&j==0)HStr=Form("ForCal/ElasticData/Cal_dE_A_%d",i);
					if(k==0&&j==1)HStr=Form("ForCal/ElasticData/Cal_dE_B_%d",i);
					if(k==1&&j==0)HStr=Form("ForCal/ElasticData/Cal_E_A_%d",i);
					if(k==1&&j==1)HStr=Form("ForCal/ElasticData/Cal_E_B_%d",i);
				}else if(Comb){
					if(k==0&&j==0)HStr=Form("ForCal/ElasticDataInverse/InvCal_dEComb_A_%d",i);
					if(k==0&&j==1)HStr=Form("ForCal/ElasticDataInverse/InvCal_dEComb_B_%d",i);
					if(k==1&&j==0)HStr=Form("ForCal/ElasticDataInverse_E/InvCal_EComb_A_%d",i);
					if(k==1&&j==1)HStr=Form("ForCal/ElasticDataInverse_E/InvCal_EComb_B_%d",i);
				}else{
					if(k==0&&j==0)HStr=Form("ForCal/ElasticDataInverse/InvCal_dE_A_%d",i);
					if(k==0&&j==1)HStr=Form("ForCal/ElasticDataInverse/InvCal_dE_B_%d",i);
					if(k==1&&j==0)HStr=Form("ForCal/ElasticDataInverse_E/InvCal_E_A_%d",i);
					if(k==1&&j==1)HStr=Form("ForCal/ElasticDataInverse_E/InvCal_E_B_%d",i);
				}
				
				if(CalSet)DetHit::SetCalibrationDirect(Eset[j+k*2],i,0,0,0);
				
				TH2* Hist=(TH2*)data.Get(HStr);
				
				if(!Hist){
					continue;
				}
				
				if(Hist->Integral()<100) continue;
		
				TProfile* py=Hist->ProfileY();
				TH1* yp=Hist->ProjectionY("pY");
				TGraphErrors* g=new TGraphErrors();
				TGraphErrors* f=new TGraphErrors();
				TGraph* ng=new TGraph();
				f->SetLineColor(kGreen);
				f->SetLineWidth(3);
				g->SetLineWidth(3);
				
				if(Zero){
					g->SetPoint(0,0,0);
					g->SetPointError(0,5,0);
					if(DoGaus){
						f->SetPoint(0,0,0);
						f->SetPointError(0,5,0);
					}
					ng->SetPoint(0,0,0);
				}
				cout<<endl;
				for(int b=1;b<=py->GetNbinsX();b++){
					double n=py->GetBinContent(b);//Mean
					double N=yp->GetBinContent(b);//Integral
					
					if(n>0&&N>10){
						
						if(DoGaus){
							TH1*h=Hist->ProjectionX("pX",b,b);
							int binMax = h->GetMaximumBin();               // Bin number of maximum
							double xMax = h->GetBinCenter(binMax);         // X position of maximum
							double yMax = h->GetBinContent(binMax);        // Height of maximum
							double stdDev = h->GetRMS();                   // Standard deviation (RMS)
							gaus->SetParameters(yMax,xMax,stdDev);
							gaus->SetRange(xMax-stdDev,xMax+stdDev);
							h->Fit(gaus,"RQN");
							
// 							f->SetPoint(f->GetN(),gaus->GetParameter(1),py->GetBinCenter(b));
// 							f->SetPointError(f->GetN()-1,gaus->GetParError(1),0);
							
							ng->SetPoint(g->GetN(),gaus->GetParameter(1),py->GetBinCenter(b));
							g->SetPoint(g->GetN(),gaus->GetParameter(1),py->GetBinCenter(b));
							g->SetPointError(g->GetN()-1,gaus->GetParError(1),0);
							
							if(ViewGaus){
								h->Fit(gaus,"R");
								gPad->Update();
								gPad->WaitPrimitive();
							}
							
							delete h;
							if(GausOnly) continue; // b loop continue
						}
						
						double e=py->GetBinError(b);
						if(e<1)e=50;
						if(e<10)e=10;
						ng->SetPoint(g->GetN(),n,py->GetBinCenter(b));
						g->SetPoint(g->GetN(),n,py->GetBinCenter(b));
						g->SetPointError(g->GetN()-1,e,0);
					}
				}
				
				pol2->SetParameters(0,0.004,0);
				pol2->SetParLimits(0,-4,4);
				pol2->SetParLimits(1,0.002,0.008);
				pol2->FixParameter(2,0);
						
// 							pol2->SetParLimits(1,0.00001,0.015);
// 							if(k==0){
// 								pol2->FixParameter(2,0);
// 							}else{
// 								pol2->ReleaseParameter(2);
// 								pol2->SetParLimits(2,-0.001,0.001);
// 							}
				if(Normalise){
					ng->Fit(pol2,"QN");
				}else{
					g->Fit(pol2,"QN");
				}
				if(!Linear){
// 					pol2->ReleaseParameter(0);
					pol2->ReleaseParameter(2);
				}
				pol2->ReleaseParameter(0);
				pol2->ReleaseParameter(1);
						
				if(Prune&&Elastic){
					g->Fit(pol2,"QN");
					
					double x, y;
					int end=0;
					if(Zero){end=1;}
					
					for (int i = g->GetN() - 1; i >= end; --i) {
						g->GetPoint(i, x, y);
						double xerr = g->GetErrorX(i);

						// Estimate expected x for given y (numerical inversion)
						double x_expected = pol2->GetX(y, pol2->GetXmin()-1, pol2->GetXmax());

// 						if (std::abs(x - x_expected) > 10 * xerr) {
						if (std::abs(x - x_expected) > 200) {
							g->RemovePoint(i);
							f->SetPoint(f->GetN(),x,y);
							f->SetPointError(f->GetN()-1,xerr,0);
							ng->RemovePoint(i);
					
							cout<<endl<<"Pruning "<<x<<"+-"<<xerr<<" , "<<y;
						}
						double std=std::abs(x - x_expected)/xerr;
					}
				}
				
				if(Normalise){
					ng->Fit(pol2,"+R");
				}else{
					g->Fit(pol2,"+R");
				}
						
				if(View){
					TCanvas* C1=new TCanvas("","",1200,900);
					C1->Divide(2,1);
					C1->cd(1);
					gPad->Update();
					Hist->DrawCopy("col");
					C1->cd(2);
					gPad->Update();
					g->SetPoint(g->GetN(),0,0);
					g->Draw("ap");
					if(Normalise){
						ng->Draw("same");
					}
					if(Prune){
						f->Draw("samep");
					}
					gPad->Update();
					gPad->WaitPrimitive();
				}
				
				if(CalSet)DetHit::SetCalibrationDirect(Eset[j+k*2],i,pol2->GetParameter(0),pol2->GetParameter(1),pol2->GetParameter(2));
				
				delete py;
				delete g;
				delete yp;
				
			} // loop A B
		} // Loop dE E
	} // i index
	
	if(CalSet)WriteCal(CalOut);
		
	return 0;
}
