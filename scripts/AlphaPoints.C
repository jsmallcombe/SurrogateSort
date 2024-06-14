#include "include/detectors.h"
#include "include/IO.h"

void AlphaPoints(){
    gSystem->Load("SS.so");
    
    ReadCal("../CalFiles/InitialCalFile.cal");
    gROOT->cd();
    
    Double_t SiAlphaX[61] = { 1.96651, 2.11636, 2.26664, 2.41821, 2.57149, 2.72563, 2.88192, 3.03993, 3.19966, 3.36153, 3.52512, 3.85831, 4.20052, 4.37527, 4.55131, 4.91198, 5.27694,
   5.65479, 6.04122, 6.23873, 6.43624, 7.25633, 8.32976, 9.459, 10.6441, 11.8849, 13.1859, 14.5384, 15.9511, 17.4238, 20.5453, 23.8987, 27.4796,
   31.2838, 35.307, 39.5449, 44.0103, 48.6475, 53.5423, 58.6088, 63.8901, 69.3431, 80.8931, 93.216, 99.6565, 106.269, 120.094, 134.65, 149.936,
   165.908, 174.195, 182.611, 218.119, 266.295, 318.678, 375.182, 435.809, 500.215, 568.484, 640.618, 716.617 };
   Double_t SiAlphaY[61] = { 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.1, 1.2, 1.25, 1.3, 1.4, 1.5,
   1.6, 1.7, 1.75, 1.8, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5,
   6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 11, 12, 12.5, 13, 14, 15, 16,
   17, 17.5, 18, 20, 22.5, 25, 27.5, 30, 32.5, 35, 37.5, 40 };
   TGraph *ASTAR_4He_Silicon_Energy_MeV_um = new TGraph(61,SiAlphaX,SiAlphaY);
   TGraph *ASTAR_4He_Silicon_Range_um_MeV = new TGraph(61,SiAlphaY,SiAlphaX);
   
   Double_t AlAlphaX[52] = { 0.5606958, 0.6528497, 0.7398224, 0.8223538, 0.9022946, 0.9800148, 1.056255, 1.131014, 1.205033, 1.350111, 1.492968, 1.634715, 1.775722, 1.916728, 2.057735, 2.199112, 2.341229,
   2.484086, 2.628053, 2.773131, 2.919689, 3.067728, 3.216876, 3.985936, 4.803849, 5.666173, 6.57661, 7.535159, 8.541821, 9.592894, 10.69578, 13.03849, 15.57735,
   18.32346, 21.27313, 24.42635, 27.77202, 31.31754, 35.04811, 38.97113, 43.0792, 47.37232, 51.85048, 56.47668, 61.28793, 88.00888, 118.8749, 153.775, 192.5241,
   281.3101, 331.1621, 384.53 };
   Double_t AlAlphaY[52] = { 0.1, 0.125, 0.15, 0.175, 0.2, 0.225, 0.25, 0.275, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7,
   0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.5, 4,
   4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 12.5, 15, 17.5, 20,
   25, 27.5, 30 };
   TGraph *ASTAR_4He_Aluminium_Energy_MeV_um = new TGraph(52,AlAlphaX,AlAlphaY);
   TGraph *ASTAR_4He_Aluminium_Range_um_MeV = new TGraph(52,AlAlphaY,AlAlphaX);
   
   
   Double_t AlphaAmOX[47] = { 1.01, 1.08, 1.15, 1.23, 1.3, 1.44, 1.58, 1.73, 1.88, 2.03, 2.19, 2.35, 2.52, 2.69, 2.87, 3.05, 3.42,
   3.91, 4.43, 4.97, 5.54, 6.14, 6.75, 7.4, 8.06, 9.46, 10.94, 12.5, 14.15, 15.87, 17.67, 21.49, 25.58,
   29.94, 34.57, 39.46, 44.61, 50.02, 55.67, 61.56, 67.69, 74.05, 87.47, 105.49, 124.89, 145.61, 167.64 };
   Double_t AlphaAmOY[47] = { 0.5, 0.55, 0.6, 0.65, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 2,
   2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9,
   10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 22.5, 25, 27.5, 30 };
   TGraph *SRIM_4He_Am02_Energy_MeV_um = new TGraph(47,AlphaAmOX,AlphaAmOY);
   TGraph *SRIM_4He_Am02_Range_um_MeV = new TGraph(47,AlphaAmOY,AlphaAmOX);
    
    double target_thickness_ug_cm2=145.5;
    double target_density_g_cm3=11.68;
    double target_um=1E-2*target_thickness_ug_cm2/target_density_g_cm3;
    double beam_energy_MeV=30; 
    double TargetZoffset=0; 
    
    double AmHalf_um=target_um/2;
    double AlHalf_um=0.5;

    double dE_Thickness_um=111;
    double dE_FrontDead_um=0.5065;
    double dE_BackDead_um=dE_FrontDead_um;
    double E_Dead_um=0.545;
    
    double TotalDead=dE_BackDead_um+dE_FrontDead_um+E_Dead_um;
    double TotalDeadTodERatio=1-(TotalDead/(dE_Thickness_um+E_Dead_um));
    
    TGraph* KinematicAlpha = new TGraph();
    
    for(int i=1;i<360;i++){
        
        double theta=i*TMath::Pi()/360;
        
        double EAlpha=5.2753;
        
        double RangeTargAlphaDecay=SRIM_4He_Am02_Range_um_MeV->Eval(EAlpha);
    
        double EffectiveTargetThickness_um=AmHalf_um/std::abs(cos(theta));
        
        double EnergyPostAmAlpha=SRIM_4He_Am02_Energy_MeV_um->Eval(RangeTargAlphaDecay-EffectiveTargetThickness_um); 
        if(RangeTargAlphaDecay<EffectiveTargetThickness_um)EnergyPostAmAlpha=0;

        double RangePostAmAlpha=ASTAR_4He_Aluminium_Range_um_MeV->Eval(EnergyPostAmAlpha);
        if(EnergyPostAmAlpha<=0)RangePostAmAlpha=0;///////////////
                
        double EffectiveBackingThickness_um=2*AlHalf_um/std::abs(cos(theta));

        double KE_Alpha_PostTarget=ASTAR_4He_Aluminium_Energy_MeV_um->Eval(RangePostAmAlpha-EffectiveBackingThickness_um); 
        if(RangePostAmAlpha<EffectiveBackingThickness_um)KE_Alpha_PostTarget=0;//////////////////////
        
        KinematicAlpha->SetPoint(KinematicAlpha->GetN(),theta,KE_Alpha_PostTarget); 
        
    }
    
    
    KinematicAlpha->Draw("AL");


    for(int mod=2;mod<4;mod++){
        
        for(int chan=0;chan<16;chan++){
            
            DetHit dEhit(0,0, mod, chan);
            
            double energysum=0;
            double positionsum=0;
            
            for(int Edex=0;Edex<16;Edex++){
    
                TVector3 PixelVec=TelescopeHit::SiliconPosDet(dEhit.Index(),Edex,TelescopeHit::AB(dEhit));
                TVector3 Intrinsic=TelescopeHit::SiliconPosDet(dEhit.Index(),Edex,TelescopeHit::AB(dEhit),false,true);
                
                double EffThick=std::abs(cos(Intrinsic.Theta())); // Angle though
                
                double theta=PixelVec.Theta();
                double Emit=KinematicAlpha->Eval(PixelVec.Theta());
                
                double SiRange=ASTAR_4He_Silicon_Range_um_MeV->Eval(Emit);
                
                double EffDead=dE_FrontDead_um/EffThick;
                
                double Energy_Active_dE=ASTAR_4He_Silicon_Energy_MeV_um->Eval(SiRange-EffDead);
                
                if(Energy_Active_dE>0){
                    double RSq=pow(PixelVec.Mag(),2);
                    energysum+=Energy_Active_dE/RSq;
                    positionsum+=1/RSq;
                }
                
            }
            if(positionsum>0){
                energysum/=positionsum;
            }
//             cout<<endl<<mod<<" "<<chan<<" "<<energysum;
        }
    }

}
