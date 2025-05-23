
    out.cd();

    TH1D* dTB=new TH1D("dTB","dTB;dT",400,-2000,2000);
    TH1D* dTselect=new TH1D("dTselect","dTselect;dT",400,-2000,2000);
    TH1D* dTgate=new TH1D("dTgate","dTgate;dT",400,-2000,2000);
    
    TH2D* ChandEE=new TH2D("ChandEE","ChandEE;E;dE",16,0,16,16,0,16);
        
    TH2D* dE_E_Raw_Sum= new TH2D("dE_E_Raw_Sum","dE_E_Raw_Sum;E (MeV);dE (MeV)",1000,0,50,500,0,15);
    TH2D* dE_E_B= new TH2D("dE_E_B","dE_E_B;E (MeV);dE (MeV)",1000,0,50,500,0,15);
    TH2D* dE_E_B_Gate= new TH2D("dE_E_B_Gate","dE_E_B_Gate;E (MeV);dE (MeV)",1000,0,50,500,0,15);
    TH2D* dE_E_B_Selected= new TH2D("dE_E_B_Selected","dE_E_B_Selected;E (MeV);dE (MeV)",1000,0,50,500,0,15);

    
    
    TH2D* dE_dT[2]={
        new TH2D("dE_dT_A","dE_dT_A;dT;i_dE",400,-2000,2000,16,0,16),
        new TH2D("dE_dT_B","dE_dT_B;dT;i_dE",400,-2000,2000,16,0,16)
    };
    TH2D* E_dT[2]={
        new TH2D("E_dT_A","E_dT_A;dT;i_E",400,-2000,2000,16,0,16),
        new TH2D("E_dT_B","E_dT_B;dT;i_E",400,-2000,2000,16,0,16)
    };
    
    
    TString nm[4]={"RunFileCountsA","RunFileCountsB","RunFileABratio","RunBglitchgate"};
   TH1D* RunFileCount[4];
   out.mkdir("SiliconVSrun");
   out.cd("SiliconVSrun");
   
   
    TH1D* Run_Gate= new TH1D("Run_Gate","Run_Gate;EventGate",1000,0,nentries);
    TH1D* Run_A_arb= new TH1D("Run_A_arb","Run_A_arb;EventGate",1000,0,nentries);
    TH1D* Run_B_arb= new TH1D("Run_B_arb","Run_B_arb;EventGate",1000,0,nentries);
    TH1D* Run_AB_Ratio= new TH1D("Run_AB_Ratio","Run_AB_Ratio;EventGate",1000,0,nentries);
   
    for(int i=0;i<4;i++){
        RunFileCount[i]=new TH1D(nm[i],nm[i],FileN,0,FileN);
        TAxis *x=RunFileCount[i]->GetXaxis();
        for(unsigned int b=0;b<Inputs.InputFiles.size();b++)x->SetBinLabel(b+1,StripFileName(Inputs.InputFiles[b]));
    }
    
    gROOT->cd();

