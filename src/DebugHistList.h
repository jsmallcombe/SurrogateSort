

    out.cd();

    TH2D* ChanE=new TH2D("ChanE","ChanE;Channel;E",totalchans,0,totalchans,1000,0,8000);
    TH2D* ChanM=new TH2D("ChanM","ChanM;Channel;M",totalchans,0,totalchans,10,0,10);
    TH2D* ChanCo=new TH2D("ChanCo","ChanCo;Channel;Channel",totalchans,0,totalchans,totalchans,0,totalchans);
    TH2D* ChanCoGate=new TH2D("ChanCoGate","ChanCoGate;Channel;Channel",totalchans,0,totalchans,totalchans,0,totalchans);
    TH2D* ChanCoGateAnti=new TH2D("ChanCoGateAnti","ChanCoGateAnti;Channel;Channel",totalchans,0,totalchans,totalchans,0,totalchans);
    
    TH2D* ChanMgate=new TH2D("ChanMgate","ChanMgate;Channel;M",totalchans,0,totalchans,10,0,10);
    
    TH2D* NN=new TH2D("NN","NN;N;NN",20,0,20,20,0,20);

//    out.mkdir("TimeHist");
//    out.cd("TimeHist");
//     
//         TH2D* dEEtime[2]={
//             new TH2D("dEvEdT_A","dEvE#DeltaT_A;dE Channel Index;dE-E #DeltaT",16,0,16,300,-3E3,3E3),
//             new TH2D("dEvEdT_B","dEvE#DeltaT_B;dE Channel Index;dE-E #DeltaT",16,0,16,300,-3E3,3E3)
//         };
//         TH2D* EdEtime[2]={
//             new TH2D("EvdEdT_A","EvdE#DeltaT_A;E Channel Index;dE-E #DeltaT",16,0,16,300,-3E3,3E3),
//             new TH2D("EvdEdT_B","EvdE#DeltaT_B;E Channel Index;dE-E #DeltaT",16,0,16,300,-3E3,3E3)
//         };
//         
//    out.cd();
    
   TH2F* SelfEnergyE[2][16];
   TH2F* SelfEnergydE[2][16];
   out.mkdir("SelfEnergy");
   out.cd("SelfEnergy");
    for(int i=0;i<16;i++){
        SelfEnergyE[0][i]=new TH2F(Form("SelfEnergyEA%d",i),Form("SelfEnergyEA%d;E;E",i),1000,0,8000,1000,0,8000);
        SelfEnergyE[1][i]=new TH2F(Form("SelfEnergyEB%d",i),Form("SelfEnergyEB%d;E;E",i),1000,0,8000,1000,0,8000);
        SelfEnergydE[0][i]=new TH2F(Form("SelfEnergydEA%d",i),Form("SelfEnergydEA%d;dE;dE",i),1000,0,8000,1000,0,8000);
        SelfEnergydE[1][i]=new TH2F(Form("SelfEnergydEB%d",i),Form("SelfEnergydEB%d;dE;dE",i),1000,0,8000,1000,0,8000);
    }

    
   out.mkdir("SelfTime");
   out.cd("SelfTime");
    TH2D* SelfTimeE[2]={
        new TH2D("SelfTimeA","SelfTimeADeltaT_A;dE Channel Index;dE-E #DeltaT",16,0,16,2000,-3E2,3E2),
        new TH2D("SelfTimeB","SelfTimeADeltaT_B;dE Channel Index;dE-E #DeltaT",16,0,16,2000,-3E2,3E2)
    };
    TH2D* SelfTimedE[2]={
        new TH2D("SelfTimedA","SelfTimedA;dE Channel Index;dE-E #DeltaT",16,0,16,2000,-3E2,3E2),
        new TH2D("SelfTimedB","SelfTimedB;dE Channel Index;dE-E #DeltaT",16,0,16,2000,-3E2,3E2)
    };

    vector<int> GateMult(totalchans,0);
