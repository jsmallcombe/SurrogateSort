

    out.cd();

    TH2D* ChanE=new TH2D("ChanE","ChanE;Channe;E",totalchans,0,totalchans,1000,0,8000);

   out.mkdir("TimeHist");
   out.cd("TimeHist");
    
        TH2D* dEEtime[2]={
            new TH2D("dEvEdT_A","dEvE#DeltaT_A;dE Channel Index;dE-E #DeltaT",16,0,16,300,-3E3,3E3),
            new TH2D("dEvEdT_B","dEvE#DeltaT_B;dE Channel Index;dE-E #DeltaT",16,0,16,300,-3E3,3E3)
        };
        TH2D* EdEtime[2]={
            new TH2D("EvdEdT_A","EvdE#DeltaT_A;E Channel Index;dE-E #DeltaT",16,0,16,300,-3E3,3E3),
            new TH2D("EvdEdT_B","EvdE#DeltaT_B;E Channel Index;dE-E #DeltaT",16,0,16,300,-3E3,3E3)
        };
        TH2D* SiHPGeT[2]={
            new TH2D("SiHPGeT_A","SiHPGe#DeltaT_A;HPGe Index;dE-HPGe #DeltaT",3,0,3,300,-3E3,3E3),
            new TH2D("SiHPGeT_B","SiHPGe#DeltaT_B;HPGe Index;dE-HPGe #DeltaT",3,0,3,300,-3E3,3E3)
        };
        
        TH2D* SiSolarT = new TH2D("SiSolarT","SiSolar#DeltaT;Solar Index;dE-Solar #DeltaT",32,0,32,300,-3E3,3E3);
        TH2D* SiLaBrT = new TH2D("SiLaBrT","SiLaBr#DeltaT;LaBr Index;dE-LaBr #DeltaT",4,0,4,300,-3E3,3E3);
        
        TH2D* LaBrGammaT = new TH2D("LaBrGammaT","LaBrAll#gamma#DeltaT;LaBr Index;LaBr-#gamma #DeltaT",4,0,4,300,-3E3,3E3);
        TH2D* HPGeGammaT = new TH2D("HPGeGammaT","HPGeAll#gamma#DeltaT;HPGe Index;HPGe-#gamma #DeltaT",4,0,4,300,-3E3,3E3);
        
   out.cd();
   
    TH2D* HPGeHPGe = new TH2D("HPGeHPGe","HPGeHPGe;HPGe E_{#gamma};HPGe E_{#gamma}",1000,0,8000,1000,0,8000);
    TH2D* HPGeLaBr = new TH2D("HPGeLaBr","HPGeLaBr;HPGe E_{#gamma};LaBr E_{#gamma}",1000,0,8000,1000,0,8000);
   

    TH2D* dEE[2]={
        new TH2D("dEEA","dEEA;E;dE",1000,0,8000,500,0,4000),
        new TH2D("dEEB","dEEB;E;dE",1000,0,8000,500,0,4000)
    };

   TH2D* dEEtot[2]={
       new TH2D("dEEtotA","dEEtotA;Etot;dE",1000,0,8000,500,0,4000),
       new TH2D("dEEtotB","dEEtotB;Etot;dE",1000,0,8000,500,0,4000)
    };


   TH2D* dEEffEtot[2]={
       new TH2D("dEdX_Etot_A","dEdX_Etot_A;E tot;dE/dX",1000,0,8000,500,0,4000),
       new TH2D("dEdX_Etot_B","dEdX_Etot_B;E tot;dE/dX",1000,0,8000,500,0,4000)
    };
    
    
   TH2F* RunFile_dE[2][16];
   TH2F* RunFile_E[2][16];
   out.mkdir("SiliconVSrun");
   out.cd("SiliconVSrun");
    for(int i=0;i<16;i++){
        RunFile_dE[0][i]=new TH2F(Form("RunFile_dE_A_%d",i),Form("RunFile_dE_A_%d;Run;dE",i),FileN,0,FileN,500,0,4000);
        TAxis *x=RunFile_dE[0][i]->GetXaxis();
        for(unsigned int b=0;b<Inputs.InputFiles.size();b++)x->SetBinLabel(b+1,StripFileName(Inputs.InputFiles[b]));
    }
    for(int i=0;i<16;i++)RunFile_E[0][i]=new TH2F(Form("RunFile_E_A_%d",i),Form("RunFile_E_A_%d;Run;E",i),FileN,0,FileN,1000,0,8000);
    for(int i=0;i<16;i++)RunFile_dE[1][i]=new TH2F(Form("RunFile_dE_B_%d",i),Form("RunFile_dE_B_%d;Run;dE",i),FileN,0,FileN,500,0,4000);
    for(int i=0;i<16;i++)RunFile_E[1][i]=new TH2F(Form("RunFile_E_B_%d",i),Form("RunFile_E_B_%d;Run;E",i),FileN,0,FileN,1000,0,8000);
    //Multiple loops so order in file is nicer
   out.cd();
 
   
   out.mkdir("SiliconCal");
   out.cd("SiliconCal");
   TH2D* dECal[2]={
       new TH2D("dECalA","dECalA Single E strip conicince;Index;dE",16,0,16,500,0,4000),
       new TH2D("dECalB","dECalB Single E strip conicince;Index;dE",16,0,16,500,0,4000)
    };
   TH2D* dEdXCal[2]={
       new TH2D("dEdXCalA","dEdXCalA Single E strip conicince;Index;dEdX",16,0,16,500,0,4000),
       new TH2D("dEdXCalB","dEdXCalB Single E strip conicince;Index;dEdX",16,0,16,500,0,4000)
    };
   TH2D* ECal[2]={
       new TH2D("ECalA","ECalA Single dE strip conicince;Index;E",16,0,16,100,0,8000),
       new TH2D("ECalB","ECalB Single dE strip conicince;Index;E",16,0,16,100,0,8000)
    };
   out.cd();
   int CalGateI=0;
   
    for(unsigned int g=0;g<Inputs.CutGates.size();g++){
        TString gatename=Inputs.CutGates[g]->GetName();
        if(gatename.Contains("eam"))CalGateI=g;
    }
   
   

   out.mkdir("dE_Chan");
   out.cd("dE_Chan");
    TH2D* dEEi[2][16];
    for(UShort_t i=0;i<16;i++)dEEi[0][i]=new TH2D(Form("dEEA_%d",i),Form("dE%d vs any E A;E;dE%d",i,i),1000,0,8000,500,0,4000);
    for(UShort_t i=0;i<16;i++)dEEi[1][i]=new TH2D(Form("dEEB_%d",i),Form("dE%d vs any E B;E;dE%d",i,i),1000,0,8000,500,0,4000);
   out.cd();

   out.mkdir("E_Chan");
   out.cd("E_Chan");
    TH2D* EdEi[2][16];
    for(UShort_t i=0;i<16;i++)EdEi[0][i]=new TH2D(Form("EdEA_%d",i),Form("E%d vs any dE A;E%d;dE",i,i),1000,0,8000,500,0,4000);
    for(UShort_t i=0;i<16;i++)EdEi[1][i]=new TH2D(Form("EdEB_%d",i),Form("E%d vs any dE B;E%d;dE",i,i),1000,0,8000,500,0,4000);
   out.cd();
   
   TH2D* ThetaPhiSum=new TH2D("ThetaPhi","ThetaPhi;Theta #theta [deg.];Phi #Phi [deg.];",180,0,180,360,0,360);
   TH2D* ThetaPhi[2]={
       new TH2D("ThetaPhiA","ThetaPhiA;Theta;Phi",180,0,180,360,0,360),
       new TH2D("ThetaPhiB","ThetaPhiB;Theta;Phi",360,0,180,360,0,360)
    };
    
    
    vector<TH1D*> GateGamma,GateLaBr;
    vector<TH2F*> GatedSiliconTheta;
    for(auto gate : Inputs.CutGates){
        TString gatename=gate->GetName();
        GateGamma.push_back(new TH1D(gatename+"GatedGamma",gatename+";E gamma",2000,0,8000));
        GateLaBr.push_back(new TH1D(gatename+"GatedLaBr",gatename+";E LaBr",2000,0,8000));
        GatedSiliconTheta.push_back(new TH2F(gatename+"GatedSiliconTheta",gatename+";Tot E;Theta",2000,0,8000,360,0,360));
    }
    
    
    
    
    
    gROOT->cd();
    
    TString GammaTreeFN(Inputs.OutFilename);
    GammaTreeFN.Remove(GammaTreeFN.Length() - 5, 5);
    GammaTreeFN.Append("_GammaTree.root");
    
    TFile GammaTreeFile(GammaTreeFN,"RECREATE");
    GammaTreeFile.cd();
        TTree gammatree("GammaTree", "A simple TTree");
        
        Double_t GamE;
        UShort_t Gami;
        
        // Create branches in the tree
        gammatree.Branch("energy", &GamE, "energy/D");
        gammatree.Branch("index", &Gami, "index/s");
        
        gammatree.AutoSave();
    gROOT->cd();
    
