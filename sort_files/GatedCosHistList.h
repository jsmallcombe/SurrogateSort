//     Particle Gates
    
    #include "src/RangeData.h"
    
    
    //Order set in RangeData.h
    vector<TString> ParticleNames ={"He3","Alphas"};
    
    vector<TCutG*> dedx_Gate={new TCutG(),new TCutG()};
    
    for(unsigned int g=0;g<Inputs.CutGates.size();g++){
        if(Inputs.GateID[g]==1){
            dedx_Gate[0]=Inputs.CutGates[g];
        }
        if(Inputs.GateID[g]==2){
            dedx_Gate[1]=Inputs.CutGates[g];
        }
    }
    
    out.cd();
        TH2D* dEdX_Etot_Corrected= new TH2D("dEdX_Etot_Corrected","dEdX_Etot_Sum;E_{tot.} (MeV);dE/dX (arb.)",1000,0,50,500,0,15);
    gROOT->cd();
    
    vector<TH2D*> Gate_dE_dX;
    vector<TH2D*> Gate_E_theta;
    vector<TH2D*> Gate_E_costheta;
    vector<TH2D*> Gate_E_thetaBlur;
    vector<TH2D*> Gate_E_thetaBlurBack;
    vector<TH2D*> Gate_E_costhetaBlur;
    vector<TH1D*> Gate_E_strad;
    for(auto T : ParticleNames){
        out.mkdir(T);
        out.cd(T);
        Gate_dE_dX.push_back(new TH2D("Charge_dEdX_Etot_sum_"+T,"dEdX_Etot_sum_"+T+";Charge E+dE;Charge dE/dX",1000,0,8000,500,0,4000));
        Gate_E_theta.push_back(new TH2D("Etarg_theta_"+T,"Etarg_#theta_"+T+";E_{targ.} (MeV);#theta (deg.)",1000,0,50,180,90,180));
        Gate_E_costheta.push_back(new TH2D("Etarg_costheta_"+T,"Etarg_cos(#theta)_"+T+";E_{targ.} (MeV);cos(#theta)",1000,0,50,200,-1,0));
        Gate_E_thetaBlur.push_back(new TH2D("Etarg_thetaBlur_"+T,"Etarg_#theta_"+T+";E_{targ.} (MeV);#theta (deg.)",1000,0,50,180,90,180));
        Gate_E_thetaBlurBack.push_back(new TH2D("EBack_thetaBlur_"+T,"EBack_#theta_"+T+";E_{Back.} (MeV);#theta (deg.)",1000,0,50,180,90,180));
        Gate_E_costhetaBlur.push_back(new TH2D("Etarg_costhetaBlur_"+T,"Etarg_cos(#theta)_"+T+";E_{targ.} (MeV);cos(#theta)",1000,0,50,200,-1,0));
        Gate_E_strad.push_back(new TH1D("Etarg_strad_"+T,"Etarg_strad_"+T+";E_{targ.} (MeV);Counts/strad",1000,0,50));
        
        gROOT->cd();
    }
    
    gROOT->cd();
    
    
    double BackRange0=RangeAl_umMeV->Eval(beam_energy_MeV);
    double MeV_MidBack=EnergyAl_MeVum->Eval(BackRange0-AlBacking_half_um);
    double MeV_PostBack=EnergyAl_MeVum->Eval(BackRange0-AlBacking_half_um*2);
    double TargRange0=RangeAm_umMeV->Eval(MeV_PostBack);
    double MeV_MidTarg=EnergyAm_MeVum->Eval(TargRange0);
    
    cout<<endl<<"ENERGY AT CENTER OF BACKING "<< MeV_MidBack<<" MeV";
    cout<<endl<<"ENERGY AT CENTER OF TARGET "<< MeV_MidTarg<<" MeV";
    
    TGraph* Am=new TGraph();
    TGraph* Al=new TGraph();
    TGraph* O=new TGraph();
    TGraph* C=new TGraph();
    
    for(int i=1;i<360;i++){
        double theta=i*TMath::Pi()/360.;
        double Elab=kinetic_lab_calcs_elastic_E(MeV_MidTarg,3.016029,243.0613727,theta)[8]; 
        Am->SetPoint(i-1,Elab,i/2.);
        Elab=kinetic_lab_calcs_elastic_E(MeV_MidTarg,3.016029,15.999,theta)[8];
        O->SetPoint(i-1,Elab,i/2.); 
         Elab=kinetic_lab_calcs_elastic_E(MeV_MidBack,3.016029,26.981539,theta)[8]; 
        Al->SetPoint(i-1,Elab,i/2.);
        Elab=kinetic_lab_calcs_elastic_E(MeV_MidTarg,3.016029,12,theta)[8]; 
        C->SetPoint(i-1,Elab,i/2.);
    }
    
    out.mkdir("KinCalc");
    out.cd("KinCalc");
        Am->Write("Kinematics243Am3He");
        Al->Write("Kinematics27Al3He");
        O->Write("Kinematics16O3He");
        C->Write("Kinematics12C3He");
    
    gROOT->cd();
