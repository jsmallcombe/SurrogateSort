//     Particle Gates
    
    #include "src/RangeData.h"

    vector<TString> ParticleNames ={"Protons","3He","Alphas"};

    
    Double_t ProtonX[7] = { 2.65046, 4.12373, 6.24863, 8.82684, 11.8584, 15.1449, 17.2698 };
    Double_t ProtonY[7] = { 2.48009, 1.7842, 1.22361, 0.914328, 0.740354, 0.52772, 0.411737 };
    TGraph *Proton_dedx = new TGraph(7,ProtonX,ProtonY);

    Double_t BeamX[8] = { 11.0367, 13.105, 14.8049, 16.4482, 18.7147, 22.0862, 25.4861, 29.4526 };
    Double_t BeamY[8] = { 10.6182, 8.31787, 7.17738, 6.32684, 5.55362, 4.64509, 4.06518, 3.48527 };
    TGraph* Beam_dedx = new TGraph(8,BeamX,BeamY);

    Double_t AlphaX[12] = { 12.5472, 13.7186, 14.9486, 16.2372, 17.7015, 19.4586, 20.8644, 23.6172, 27.483, 31.1144, 36.7959, 39.7831 };
    Double_t AlphaY[12] = { 11.845, 10.3878, 9.30837, 8.36388, 7.58131, 6.82571, 6.33998, 5.61137, 4.74784, 4.2621, 3.61445, 3.29063 };
    TGraph *Alpha_dedx = new TGraph(12,AlphaX,AlphaY);
    
    vector<TGraph*> GateRoughID ={Proton_dedx,Beam_dedx,Alpha_dedx};
    vector<int> GateID ={0,1,2};

    int i_Si=0,i_Al=1,i_Am0=2;

    vector<vector<TGraph*>> Range_um ={{H1_RangeSilicon_umMeV,H1_RangeSilicon_umMeV,H1_RangeSilicon_umMeV},
                                            {He3_RangeSilicon_umMeV,He3_RangeAlBacking_umMeV,He3_RangeTarget_umMeV},
                                                {He4_RangeSilicon_umMeV,He4_RangeAlBacking_umMeV,He4_RangeTarget_umMeV}};
                                                
    vector<vector<TGraph*>> Energy_MeV ={{H1_EnergySilicon_MeVum,H1_EnergySilicon_MeVum,H1_EnergySilicon_MeVum},
                                            {He3_EnergySilicon_MeVum,He3_EnergyAlBacking_MeVum,He3_EnergyTarget_MeVum},
                                                {He4_EnergySilicon_MeVum,He4_EnergyAlBacking_MeVum,He4_EnergyTarget_MeVum}};

    vector<TCutG*> dedx_Gate={new TCutG(),new TCutG(),new TCutG()};

    for(unsigned int g=0;g<Inputs.CutGates.size();g++){
        for(unsigned int i=0;i<GateID.size();i++){
            if(GateID[i]==Inputs.GateID[g]){
                dedx_Gate[i]=Inputs.CutGates[g];
            }
        }
    }

    out.cd();

    
    TH2D* dE_E_Raw_Sum= new TH2D("dE_E_Raw_Sum","dE_E_Raw_Sum;E (MeV);dE (MeV)",1000,0,50,500,0,15);
        
    TH2D* dE_E_Raw[2]={
        new TH2D("dE_E_Raw_A","dE_E_Raw_A;E (MeV);dE (MeV)",1000,0,50,500,0,15),
        new TH2D("dE_E_Raw_B","dE_E_Raw_B;E (MeV);dE (MeV)",1000,0,50,500,0,15)
    };
    
    TH2D* dEdX_Etot_Sum= new TH2D("dEdX_Etot_Sum","dEdX_Etot_Sum;E_{tot.} (MeV);dE/dX (arb.)",1000,0,50,500,0,15);
    TH2D* dEdX_Etot_Sum_Mult2= new TH2D("dEdX_Etot_Sum_Mult2","dEdX_Etot_Sum_Mult2;E_{tot.} (MeV);dE/dX (arb.)",1000,0,50,500,0,15);
    
    TH2D* dEdX_Etot_Corrected= new TH2D("dEdX_Etot_Corrected","dEdX_Etot_Sum;E_{tot.} (MeV);dE/dX (arb.)",1000,0,50,500,0,15);
    
    
    vector<TH2D*> Gate_dE_dX;
    vector<TH2D*> Gate_E_theta;
    vector<TH2D*> Gate_E_costheta;
    vector<TH2D*> Gate_E_thetaBlur;
    vector<TH2D*> Gate_E_costhetaBlur;
    vector<TH1D*> Gate_E_strad;
    for(auto T : ParticleNames){
        out.mkdir(T);
        out.cd(T);
        
        Gate_dE_dX.push_back(new TH2D("dEdX_Etot_sum_"+T,"dEdX_Etot_sum_"+T+";E_{tot.} (MeV);dE/dX (arb.)",1000,0,50,500,0,15));
        Gate_E_theta.push_back(new TH2D("Etarg_theta_"+T,"Etarg_#theta_"+T+";E_{targ.} (MeV);#theta (deg.)",1000,0,50,180,90,180));
        Gate_E_costheta.push_back(new TH2D("Etarg_costheta_"+T,"Etarg_cos(#theta)_"+T+";E_{targ.} (MeV);cos(#theta)",1000,0,50,200,-1,0));
        Gate_E_thetaBlur.push_back(new TH2D("Etarg_thetaBlur_"+T,"Etarg_#theta_"+T+";E_{targ.} (MeV);#theta (deg.)",1000,0,50,180,90,180));
        Gate_E_costhetaBlur.push_back(new TH2D("Etarg_costhetaBlur_"+T,"Etarg_cos(#theta)_"+T+";E_{targ.} (MeV);cos(#theta)",1000,0,50,200,-1,0));
        Gate_E_strad.push_back(new TH1D("Etarg_strad_"+T,"Etarg_strad_"+T+";E_{targ.} (MeV);Counts/strad",1000,0,50));
        
        
        gROOT->cd();
    }
    
    
    
    gROOT->cd();
    
//     TString GammaTreeFN(Inputs.OutFilename);
//     GammaTreeFN.Remove(GammaTreeFN.Length() - 5, 5);
//     GammaTreeFN.Append("_GammaTree.root");
//     
//     TFile GammaTreeFile(GammaTreeFN,"RECREATE");
//     GammaTreeFile.cd();
//         TTree gammatree("GammaTree", "A simple TTree");
//         
//         Double_t GamE;
//         UShort_t Gami;
//         
//         // Create branches in the tree
//         gammatree.Branch("energy", &GamE, "energy/D");
//         gammatree.Branch("index", &Gami, "index/s");
//         
//         gammatree.AutoSave();
//     gROOT->cd();
//     

