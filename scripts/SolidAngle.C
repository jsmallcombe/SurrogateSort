#include "include/detectors.h"
#include "include/IO.h"

void SolidAngle(){
    
    gSystem->Load("SS.so");
    
    ReadCal("../CalFiles/Fin.cal");
    gROOT->cd();
    
    TH2D* ThetaPhi=new TH2D("ThetaPhi","ThetaPhi",720,TMath::Pi()/2,TMath::Pi(),720,-TMath::Pi(),TMath::Pi());
    TH2D* ThetaCos=new TH2D("ThetaCos","ThetaPhi",720,-1,0,720,-TMath::Pi(),TMath::Pi());
 
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            cout<<j<<flush;
            for(int ab=0;ab<2;ab++){
        
            if(ab==0&&i==5)continue;
        
                for(int n=0;n<10000;n++){
                    TVector3 vec=TelescopeHit::SiliconPosDet(i,j,ab,true);
                    double theta=vec.Theta();
                    double cs=cos(theta);
                    double ph=vec.Phi();
                    ThetaPhi->Fill(theta,ph);
                    ThetaCos->Fill(cs,ph);
                }
            }
        }
    }
    ThetaPhi->Draw("col");
    new TCanvas();
    ThetaCos->Draw("col");

    TGraph* Gtheta=new TGraph();
    TGraph* Gcos=new TGraph();
    for(int x=1;x<=ThetaPhi->GetNbinsX();x++){
        int count=0;
        int countcos=0;
        for(int y=1;y<=ThetaPhi->GetNbinsY();y++){
            if(ThetaPhi->GetBinContent(x,y)>0)count++;
            if(ThetaCos->GetBinContent(x,y)>0)countcos++;
        }
        
        Gtheta->SetPoint(x-1,ThetaPhi->GetXaxis()->GetBinCenter(x),count*1.0/ThetaPhi->GetNbinsY());
        Gcos->SetPoint(x-1,ThetaCos->GetXaxis()->GetBinCenter(x),countcos*1.0/ThetaCos->GetNbinsY());
        
    }
    
    double area = Gcos->Integral();
    std::cout << "Area under the curve: " << area << std::endl;
    std::cout << "Total strad coverage of both detectors: " << area*TMath::Pi()*2 <<" str."<<std::endl;
    std::cout << "Total strad coverage of both detectors: " << area*2 <<"*pi str."<<std::endl;

    new TCanvas();
    gPad->Update();
    Gtheta->Draw("al");
    new TCanvas();
    gPad->Update();
    Gcos->SetTitle("Cos_theta;cos(#theta);#phi Fraction [1/2#pi]");
    Gcos->SetName("Cos_theta");
    Gcos->SaveAs("SolidAngle.root");
}
