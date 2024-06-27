    TFile* FilterTreeFile=nullptr;
    TTree* FilterTree=nullptr;
    bool FillFilterTree=false;
   
    TCutG* Raw_dE_E_Charge_ElasticBeamGate[2]={new TCutG(),new TCutG()};
    TCutG* Raw_dE_E_Charge_BeamGate[2]={new TCutG(),new TCutG()};
    TCutG* dEdX_Etot_Charge_ProtonGate[2]={new TCutG(),new TCutG()};
    TCutG* Raw_dE_E_Charge_4HeGate[2]={new TCutG(),new TCutG()};
    TCutG* Raw_dE_E_Charge_3HeGate[2]={new TCutG(),new TCutG()};
    
    for(unsigned int g=0;g<Inputs.CutGates.size();g++){

        if(Inputs.GateID[g]==0){
            Raw_dE_E_Charge_ElasticBeamGate[0]=Inputs.CutGates[g];
            Raw_dE_E_Charge_ElasticBeamGate[1]=Inputs.CutGates[g];
        }
        if(Inputs.GateID[g]==1){
            Raw_dE_E_Charge_BeamGate[0]=Inputs.CutGates[g];
            Raw_dE_E_Charge_BeamGate[1]=Inputs.CutGates[g];
        }
        
        if(Inputs.GateID[g]==3){
            dEdX_Etot_Charge_ProtonGate[0]=Inputs.CutGates[g];
            dEdX_Etot_Charge_ProtonGate[1]=Inputs.CutGates[g];
        }
        
        if(Inputs.GateID[g]==4){
            Raw_dE_E_Charge_4HeGate[0]=Inputs.CutGates[g];
        }
        if(Inputs.GateID[g]==5){
            Raw_dE_E_Charge_4HeGate[1]=Inputs.CutGates[g];
        }
        if(Inputs.GateID[g]==6){
            Raw_dE_E_Charge_3HeGate[0]=Inputs.CutGates[g];
        }
        if(Inputs.GateID[g]==7){
            Raw_dE_E_Charge_3HeGate[1]=Inputs.CutGates[g];
        }
    }
    
    
    
    gROOT->cd();
        if(Inputs.TestInput("OverwriteFilterFile")){
            FilterTreeFile=new TFile("FilterSiliconTree.root","RECREATE");
            FilterTreeFile->cd();
            FilterTree = new TTree("OutTTree01", "FilteredEventTree");
        
            FilterTree->AutoSave();
            
            FilterTree->Branch("Num", &tNum, "Num/s");
            FilterTree->Branch("Ts",  &tTs );
            FilterTree->Branch("Mod", &tMod);
            FilterTree->Branch("Ch",  &tCh);
            FilterTree->Branch("Adc", &tAdc);
        }
    gROOT->cd();
    
    
    out.cd();
    out.mkdir("Raw");
    out.cd("Raw");

        TH2D* ChanE=new TH2D("ChanVsCharge","ChanVsCharge;Channel;Charge",totalchans,0,totalchans,1000,0,8000);

        TH2D* dEE[2]={
            new TH2D("Charge_E_dE_A","Charge_E_dE_A;Charge E;Charge dE",1000,0,8000,500,0,4000),
            new TH2D("Charge_E_dE_B","Charge_E_dE_B;Charge E;Charge dE",1000,0,8000,500,0,4000)
        };

        TH2D* dEEtot[2]={
            new TH2D("Charge_Tot_dE_A","Charge_Tot_dE_A;Charge E+dE;Charge dE",1000,0,8000,500,0,4000),
            new TH2D("Charge_Tot_dE_B","Charge_Tot_dE_B;Charge E+dE;Charge dE",1000,0,8000,500,0,4000)
            };

        TH2D* dEEffEtot[2]={
            new TH2D("Charge_dEdX_Etot_A","Charge_dEdX_Etot_A;Charge E+dE;Charge dE/dX",1000,0,8000,500,0,4000),
            new TH2D("Charge_dEdX_Etot_B","Charge_dEdX_Etot_B;Charge E+dE;Charge dE/dX",1000,0,8000,500,0,4000)
            };
            
            
//     vector<vector<TH2F*>> GatedSilicon_dEE;
//     for(auto gate : Inputs.CutGates){
//         TString gatename=gate->GetName();
//         vector<TH2F*> gv;
//         gv.push_back(new TH2F(gatename+"_Gated_Charge_E_dE_A",gatename+"_Gated_Charge_E_dE_A;Charge E;Charge dE",1000,0,8000,500,0,4000));
//         gv.push_back(new TH2F(gatename+"_Gated_Charge_E_dE_B",gatename+"_Gated_Charge_E_dE_B;Charge E;Charge dE",1000,0,8000,500,0,4000));
//         GatedSilicon_dEE.push_back(gv);
//     }
        
    
    out.mkdir("Raw/dE_Chan");
    out.cd("Raw/dE_Chan");
        TH2D* dEEi[2][16];
        for(UShort_t i=0;i<16;i++)dEEi[0][i]=new TH2D(Form("dEEA_%d",i),Form("Charge dE%d vs any E A;Charge E All;Charge dE%d",i,i),1000,0,8000,500,0,4000);
        for(UShort_t i=0;i<16;i++)dEEi[1][i]=new TH2D(Form("dEEB_%d",i),Form("Charge dE%d vs any E B;Charge E All;Charge dE%d",i,i),1000,0,8000,500,0,4000);
        TH2D* dEdXEi[2][16];
        for(UShort_t i=0;i<16;i++)dEdXEi[0][i]=new TH2D(Form("dEdXTotA_%d",i),Form("Charge dE%ddX vs any E A;Charge Tot;Charge dE%ddX",i,i),1000,0,8000,500,0,4000);
        for(UShort_t i=0;i<16;i++)dEdXEi[1][i]=new TH2D(Form("dEdXTotB_%d",i),Form("Charge dE%ddX vs any E B;Charge Tot;Charge dE%ddX",i,i),1000,0,8000,500,0,4000);
    out.cd();

    out.mkdir("Raw/E_Chan");
    out.cd("Raw/E_Chan");
        TH2D* EdEi[2][16];
        for(UShort_t i=0;i<16;i++)EdEi[0][i]=new TH2D(Form("EdEA_%d",i),Form("Charge E%d vs any dE A;Charge E%d;Charge dE All",i,i),1000,0,8000,500,0,4000);
        for(UShort_t i=0;i<16;i++)EdEi[1][i]=new TH2D(Form("EdEB_%d",i),Form("Charge E%d vs any dE B;Charge E%d;Charge dE All",i,i),1000,0,8000,500,0,4000);
    out.cd();
            
    
   out.mkdir("Raw/dE_LowMassElasticGateCheck");
   out.cd("Raw/dE_LowMassElasticGateCheck");
    TH2D* dEEi_Al[2][16];
    for(UShort_t i=0;i<16;i++)dEEi_Al[0][i]=new TH2D(Form("dEEA_LME_%d",i),Form("LowMassElasticGate dE%d vs any E A;Charge E All;Charge dE%d",i,i),500,0,8000,250,0,4000);
    for(UShort_t i=0;i<16;i++)dEEi_Al[1][i]=new TH2D(Form("dEEB_LME_%d",i),Form("LowMassElasticGate dE%d vs any E A;Charge E All;Charge dE%d",i,i),500,0,8000,250,0,4000);
   out.cd();
    
   
    out.cd();
    out.mkdir("Calibrated");
    out.cd("Calibrated");
        
        TH2D* dE_E_Calibrated[2]={
            new TH2D("dE_E_Calibrated_A","dE_E_Calibrated_A;E (MeV);dE (MeV)",1000,0,40,500,0,15),
            new TH2D("dE_E_Calibrated_B","dE_E_Calibrated_B;E (MeV);dE (MeV)",1000,0,40,500,0,15)
        };
        
        TH2D* dE_Etot_Calibrated[2]={
            new TH2D("dE_Etot_Calibrated_A","dE_Etot_Calibrated_A;E+dE (MeV);dE (MeV)",1000,0,40,500,0,15),
            new TH2D("dE_Etot_Calibrated_B","dE_Etot_Calibrated_B;E+dE (MeV);dE (MeV)",1000,0,40,500,0,15)
        };
        
        TH2D* dEdX_Etot_SumCalibrated[2]={
            new TH2D("dEdX_E_at_dE_SumCalibrated_A","dEdX_EAtdE_Calibrated_A;E+dE0.5+dead (MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15),
            new TH2D("dEdX_E_at_dE_SumCalibrated_B","dEdX_EAtdE_Calibrated_B;E+dE0.5+dead (MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15)
        };
        
        TH2D* dEdX_Etot_SumCalibrated3He[2]={
            new TH2D("dEdX_E_at_dE_SumCalibrated3He_A","3He_dEdX_EatdE_Calibrated_A;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15),
            new TH2D("dEdX_E_at_dE_SumCalibrated3He_B","3He_dEdX_EatdE_Calibrated_B;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15)
        };
        
        TH2D* dEdX_Etot_SumCalibratedProton[2]={
            new TH2D("dEdX_E_at_dE_SumCalibratedProton_A","Proton_dEdX_EatdE_Calibrated_A;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15),
            new TH2D("dEdX_E_at_dE_SumCalibratedProton_B","Proton_dEdX_EatdE_Calibrated_B;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15)
        };
    
   out.mkdir("Calibrated/dE_Chan_cal");
   out.cd("Calibrated/dE_Chan_cal");
    TH2D* dEEi_cal[2][16];
    for(UShort_t i=0;i<16;i++)dEEi_cal[0][i]=new TH2D(Form("dEE_calA_%d",i),Form("dE%d vs any E A;E (MeV);dE%d (MeV)",i,i),1000,0,40,500,0,15);
    for(UShort_t i=0;i<16;i++)dEEi_cal[1][i]=new TH2D(Form("dEE_calB_%d",i),Form("dE%d vs any E B;E (MeV);dE%d (MeV)",i,i),1000,0,40,500,0,15);
   out.cd();

   out.mkdir("Calibrated/E_Chan_cal");
   out.cd("Calibrated/E_Chan_cal");
    TH2D* EdEi_cal[2][16];
    for(UShort_t i=0;i<16;i++)EdEi_cal[0][i]=new TH2D(Form("EdE_calA_%d",i),Form("E%d vs any dE A;E%d (MeV);dE (MeV)",i,i),1000,0,40,500,0,15);
    for(UShort_t i=0;i<16;i++)EdEi_cal[1][i]=new TH2D(Form("EdE_calB_%d",i),Form("E%d vs any dE B;E%d (MeV);dE (MeV)",i,i),1000,0,40,500,0,15);
   out.cd();
   
    
    
    out.cd();
    out.mkdir("ForCal");
    out.cd("ForCal");

    out.cd();
    out.mkdir("ForCal/ElasticData");
    out.cd("ForCal/ElasticData");
    
    TH2F* CalElast_dE[2][16];
    TH2F* CalElast_E[2][16];
        for(int i=0;i<16;i++)CalElast_dE[0][i]=new TH2F(Form("Cal_dE_A_%d",i),Form("Cal_dE_A_%d;dE Charge;Energy (Kinematic Calc. MeV)",i),500,0,3000,500,0,15);
        for(int i=0;i<16;i++)CalElast_E[0][i]=new TH2F(Form("Cal_E_A_%d",i),Form("Cal_E_A_%d;E Charge;Energy (Kinematic Calc. MeV)",i),1000,0,6000,1000,0,30);
        for(int i=0;i<16;i++)CalElast_dE[1][i]=new TH2F(Form("Cal_dE_B_%d",i),Form("Cal_dE_B_%d;dE Charge;Energy (Kinematic Calc. MeV)",i),500,0,3000,500,0,15);
        for(int i=0;i<16;i++)CalElast_E[1][i]=new TH2F(Form("Cal_E_B_%d",i),Form("Cal_E_B_%d;E Charge;Energy (Kinematic Calc. MeV)",i),1000,0,6000,1000,0,30);
        //Multiple loops so order in file is nicer
    
   out.cd();
   out.mkdir("ForCal/ElasticDataInverse");
   out.cd("ForCal/ElasticDataInverse");
   
    TH2F* InvCal_dE[2][16];
    TH2F* InvCal_dE4He[2][16];
    TH2F* InvCal_dESelf[2][16];
    TH2F* InvCal_dEComb[2][16];
    TH2F* InvCal_dE_E_Proton[2][16];
        for(int i=0;i<16;i++)InvCal_dE[0][i]=new TH2F(Form("InvCal_dE_A_%d",i),Form("InverseCalibration_3He_dE_A_%d_FromE;Charge dE%d;dE (Calc. MeV)",i,i),500,0,3000,500,0,15);
        for(int i=0;i<16;i++)InvCal_dE[1][i]=new TH2F(Form("InvCal_dE_B_%d",i),Form("InverseCalibration_3He_dE_B_%d_FromE;Charge dE%d;dE (Calc. MeV)",i,i),500,0,3000,500,0,15);
        for(int i=0;i<16;i++)InvCal_dE4He[0][i]=new TH2F(Form("InvCal4He_dE_A_%d",i),Form("InverseCalibration_4He_dE_A_%d_FromE;Charge dE%d;dE (Calc. MeV)",i,i),500,0,3000,500,0,15);
        for(int i=0;i<16;i++)InvCal_dE4He[1][i]=new TH2F(Form("InvCal4He_dE_B_%d",i),Form("InverseCalibration_4He_dE_B_%d_FromE;Charge dE%d;dE (Calc. MeV)",i,i),500,0,3000,500,0,15);
        for(int i=0;i<16;i++)InvCal_dESelf[0][i]=new TH2F(Form("InvCal_dESelf_A_%d",i),Form("InverseSelfCalibration_3He_dE_A_%d;Charge dE ;dE (MeV)",i),500,0,15,500,0,15);
        for(int i=0;i<16;i++)InvCal_dESelf[1][i]=new TH2F(Form("InvCal_dESelf_B_%d",i),Form("InverseSelfCalibration_3He_dE_B_%d;Charge dE ;dE (MeV))",i),500,0,15,500,0,15);
        for(int i=0;i<16;i++)InvCal_dEComb[0][i]=new TH2F(Form("InvCal_dEComb_A_%d",i),Form("Combine_InverseCalibrations_3He_A_%d;Charge dE; dE",i),500,0,3000,500,0,15);
        for(int i=0;i<16;i++)InvCal_dEComb[1][i]=new TH2F(Form("InvCal_dEComb_B_%d",i),Form("Combine_InverseCalibrations_3He_B_%d;Charge dE; dE",i),500,0,3000,500,0,15);
        for(int i=0;i<16;i++)InvCal_dE_E_Proton[0][i]=new TH2F(Form("Cal_dE_E_Proton_A%d",i),Form("Proton_dE%d_CalcFromE_A;dE%d Charge;dE Proton (Calc. MeV)",i,i),500,0,3000,500,0,15);
        for(int i=0;i<16;i++)InvCal_dE_E_Proton[1][i]=new TH2F(Form("Cal_dE_E_Proton_B%d",i),Form("Proton_dE%d_CalcFromE_B;dE%d Charge;dE Proton (Calc. MeV)",i,i),500,0,3000,500,0,15);
   out.cd();

   
    // Per dE channel gate for "cleanly" selecting elastic on light mass target element
    double ElasticAlLow[16]={1250,1300,1400,1500,1600,1670,1770,1850,1920,2000,2050,2100,2150,0,0,0}; 
    double ElasticAlHigh[16]={1420,1500,1600,1700,1760,1830,1920,2010,2120,2200,2250,2300,2350,0,0,0}; 
    
    double target_thickness_ug_cm2=145.5;
    double target_density_g_cm3=11.68;
    double target_um=1E-2*target_thickness_ug_cm2/target_density_g_cm3;
    double beam_energy_MeV=30; 
    double TargetZoffset=0; 
    
    if(Inputs.TestInput("BeamEnergy")){
        beam_energy_MeV=Inputs.GetInput("BeamEnergy");
    }
    if(Inputs.TestInput("ZOffset")){
        TargetZoffset=Inputs.GetInput("ZOffset");
    }
    
    double AmHalf_um=target_um/2;
    double AlHalf_um=0.5;
    
    if(Inputs.TestInput("Backing_Thickness_um")){
        AlHalf_um=Inputs.GetInput("Backing_Thickness_um")*0.5;
    }

    double dE_Thickness_um=111;
    double dE_FrontDead_um=0.5065;
    double dE_BackDead_um=dE_FrontDead_um;
    double E_Dead_um=0.545;
    
    if(Inputs.TestInput("dE_Thickness_um")){
        dE_Thickness_um=Inputs.GetInput("dE_Thickness_um");
    }
    if(Inputs.TestInput("dE_BackDead_um")){
        dE_BackDead_um=Inputs.GetInput("dE_BackDead_um");
    }
    
    double TotalDead=dE_BackDead_um+dE_FrontDead_um+E_Dead_um;
    double TotalDeadTodERatio=1-(TotalDead/(dE_Thickness_um+E_Dead_um));
    
    
   Double_t RangeAl_fx2[50] = { 0.55, 0.6, 0.65, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 2, 2.25,  2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9, 10,  11, 12, 13, 14, 15, 16, 17, 18, 20, 22.5, 25, 27.5, 30, 32.5, 35, 37.5,  40 };
   Double_t RangeAl_fy2[50] = { 1.8, 1.94, 2.08, 2.22, 2.52, 2.82, 3.14, 3.46, 3.8, 4.14, 4.5, 4.87, 5.25, 5.64, 6.04, 6.88, 7.99,  9.16, 10.41, 11.71, 13.09, 14.52, 16.02, 17.58, 20.88, 24.43, 28.2, 32.2, 36.43, 40.89, 50.53, 61.09, 72.53,  84.84, 97.99, 111.98, 126.79, 142.41, 158.83, 176.04, 194.02, 232.29, 284.35, 341.06, 402.3, 468.02, 538.12, 612.56, 691.26,  774.18 };
   TGraph *RangeAl_umMeV = new TGraph(50,RangeAl_fx2,RangeAl_fy2);
   TGraph *EnergyAl_MeVum = new TGraph(50,RangeAl_fy2,RangeAl_fx2);
   
   Double_t Graph_fx6[50] = { 0.55, 0.6, 0.65, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 2, 2.25,  2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9, 10,  11, 12, 13, 14, 15, 16, 17, 18, 20, 22.5, 25, 27.5, 30, 32.5, 35, 37.5,  40 };
   Double_t Graph_fy6[50] = { 0.984, 1.05, 1.13, 1.2, 1.34, 1.5, 1.65, 1.82, 1.99, 2.16, 2.34, 2.53, 2.72, 2.92, 3.12, 3.55, 4.11,  4.71, 5.34, 5.99, 6.68, 7.4, 8.14, 8.92, 10.54, 12.27, 14.1, 16.03, 18.05, 20.15, 24.63, 29.47, 34.65,   40.16, 45.99, 52.14, 58.61, 65.38, 72.44, 79.81, 87.46, 103.61, 125.36, 148.81, 173.9, 200.62, 228.91, 258.75, 290.12,  322.99 };
   TGraph *RangeAm_umMeV = new TGraph(50,Graph_fx6,Graph_fy6);
   TGraph *EnergyAm_MeVum = new TGraph(50,Graph_fy6,Graph_fx6);
   
   Double_t Graph_fx4[50] = { 0.55, 0.6, 0.65, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 2, 2.25,  2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9, 10,  11, 12, 13, 14, 15, 16, 17, 18, 20, 22.5, 25, 27.5, 30, 32.5, 35, 37.5,
   40 };
   Double_t Graph_fy4[50] = { 1.98, 2.14, 2.29, 2.45, 2.78, 3.12, 3.47, 3.83, 4.21, 4.6, 5.01, 5.43, 5.86, 6.31, 6.77, 7.73, 9,  10.35, 11.78, 13.29, 14.87, 16.53, 18.25, 20.05, 23.86, 27.93, 32.27, 36.87, 41.72, 46.83, 57.86, 69.94, 83.01,  97.07, 112.08, 128.04, 144.93, 162.73, 181.44, 201.04, 221.52, 265.08, 324.32, 388.8, 458.42, 533.09, 612.73, 697.26, 786.62,  880.74 };
   TGraph *RangeSi_umMeV = new TGraph(50,Graph_fx4,Graph_fy4);
   TGraph *EnergySi_MeVum = new TGraph(50,Graph_fy4,Graph_fx4);   

Double_t Graph_fx2as[51] = { 1.82676, 1.9766, 2.12685, 2.2978, 2.45095, 2.78151, 3.12922, 3.48483, 3.84823, 4.23073, 4.62179, 5.0227, 5.43776, 5.86571, 6.30214, 6.75705, 7.69672,  8.94899, 10.2555, 11.6469, 13.0932, 14.6291, 16.2262, 17.892, 19.6387, 23.3384, 27.3212, 31.5838, 36.1217, 40.9316, 46.0187, 57.048, 69.2351,  82.3711, 96.4975, 111.601, 127.611, 144.591, 162.507, 181.313, 200.993, 221.603, 265.448, 325.031, 389.909, 459.965, 535.219, 615.23, 700.109,  789.857, 884.464 };
   Double_t Graph_fy2as[51] = { 0.5, 0.55, 0.6, 0.65, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 2,  2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 22.5, 25, 27.5, 30, 32.5, 35,  37.5, 40 };
   TGraph *ASTAR_Si_He3_Energy = new TGraph(51,Graph_fx2as,Graph_fy2as);  
   TGraph *ASTAR_Si_He3_Range = new TGraph(51,Graph_fy2as,Graph_fx2as);
   //NIST ASTAR DATA, Mass adjusted based on SRIM
   
   TGraph *He3Silicon_Energy_MeVum = ASTAR_Si_He3_Energy;  
   TGraph *He3Silicon_Range_umMeV = ASTAR_Si_He3_Range;
   
    if(Inputs.TestInput("UseSRIM")){
        He3Silicon_Energy_MeVum = EnergySi_MeVum;  
        He3Silicon_Range_umMeV = RangeSi_umMeV;
    }
   
   Double_t Graph_fx1p[61] = { 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.1, 1.2, 1.25, 1.3, 1.4, 1.5,  1.6, 1.7, 1.75, 1.8, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5,   6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 11, 12, 12.5, 13, 14, 15, 16,  17, 17.5, 18, 20, 22.5, 25, 27.5, 30, 32.5, 35, 37.5, 40 };
   Double_t Graph_fy1p[61] = { 5.96393, 6.80979, 7.69429, 8.61743, 9.57922, 10.5754, 11.6144, 12.6921, 13.8128, 14.9721, 16.1657, 18.669, 21.3225, 22.705, 24.1219, 27.0588, 30.1417,  33.3577, 36.711, 38.4414, 40.2018, 47.5741, 57.5354, 68.2696, 79.7767, 92.0567, 105.109, 118.892, 133.362, 148.605, 181.151, 216.445, 254.444,  295.105, 338.386, 384.242, 432.804, 483.469, 536.711, 592.529, 650.923, 711.464, 839.845, 977.243, 1049.81, 1124.09, 1279.95, 1444.83, 1618.29,  1800.34, 1894.8, 1990.98, 2398.02, 2953.2, 3559.04, 4214.26, 4916.27, 5671.96, 6470.59, 7312.15, 8205.24 };
   TGraph *ProtonsSilicon_Range_umMeV = new TGraph(61,Graph_fx1p,Graph_fy1p);
   TGraph *ProtonsSilicon_Energy_MeVum = new TGraph(61,Graph_fy1p,Graph_fx1p);
   //NIST PSTAR Projected Range DATA 
   
   
    double BackRange0=RangeAl_umMeV->Eval(beam_energy_MeV);
    double MeV_MidBack=EnergyAl_MeVum->Eval(BackRange0-AlHalf_um);
    double MeV_PostBack=EnergyAl_MeVum->Eval(BackRange0-AlHalf_um*2);
    double TargRange0=RangeAm_umMeV->Eval(MeV_PostBack);
    double MeV_MidTarg=EnergyAm_MeVum->Eval(TargRange0);
    
    cout<<endl<<"ENERGY AT CENTER OF BACKING "<< MeV_MidBack<<" MeV";
    cout<<endl<<"ENERGY AT CENTER OF TARGET "<< MeV_MidTarg<<" MeV";
    
    TGraph* KinematicElastic=new TGraph();
    TGraph* KinematicElasticAl=new TGraph();
    
    for(int i=1;i<360;i++){
        
//         4.002603
//          243.0613727
//          3.016029
        
        double theta=i*TMath::Pi()/360;
        
        double Elab=kinetic_lab_calcs_elastic_E(MeV_MidTarg,3.016029,243.0613727,theta)[8]; 
        
        double RangeTargElastic=RangeAm_umMeV->Eval(Elab);
    
        double EffectiveTargetThickness_um=AmHalf_um/std::abs(cos(theta));
        
        double EnergyPostAmElastic=EnergyAm_MeVum->Eval(RangeTargElastic-EffectiveTargetThickness_um); 
        if(RangeTargElastic<EffectiveTargetThickness_um)EnergyPostAmElastic=0;

        double RangePostAmElastic=RangeAl_umMeV->Eval(EnergyPostAmElastic);
        if(EnergyPostAmElastic<=0)RangePostAmElastic=0;///////////////
                
        double EffectiveBackingThickness_um=2*AlHalf_um/std::abs(cos(theta));

        double KE_PostTarget=EnergyAl_MeVum->Eval(RangePostAmElastic-EffectiveBackingThickness_um); 
        if(RangePostAmElastic<EffectiveBackingThickness_um)KE_PostTarget=0;//////////////////////
        
        KinematicElastic->SetPoint(KinematicElastic->GetN(),theta,KE_PostTarget); 
        
        
        if(Inputs.TestInput("Oxygen")){  
            //// Elastic on Oxygen In Target
            Elab=kinetic_lab_calcs_elastic_E(MeV_MidTarg,3.016029,15.999,theta)[8]; 
            RangeTargElastic=RangeAm_umMeV->Eval(Elab);
            EffectiveTargetThickness_um=AmHalf_um/std::abs(cos(theta));
            EnergyPostAmElastic=EnergyAm_MeVum->Eval(RangeTargElastic-EffectiveTargetThickness_um); 
            if(RangeTargElastic<EffectiveTargetThickness_um)EnergyPostAmElastic=0;
            RangePostAmElastic=RangeAl_umMeV->Eval(EnergyPostAmElastic);
            if(EnergyPostAmElastic<=0)RangePostAmElastic=0;///////////////
            EffectiveBackingThickness_um=2*AlHalf_um/std::abs(cos(theta));
            KE_PostTarget=EnergyAl_MeVum->Eval(RangePostAmElastic-EffectiveBackingThickness_um); 
            if(RangePostAmElastic<EffectiveBackingThickness_um)KE_PostTarget=0;//////////////////////
            KinematicElasticAl->SetPoint(KinematicElasticAl->GetN(),theta,KE_PostTarget);     
        }else{
        //// Elastic on Aluminium backing 
            Elab=kinetic_lab_calcs_elastic_E(MeV_MidBack,3.016029,26.981539,theta)[8]; 
 
            if(Inputs.TestInput("Carbon")){ 
                Elab=kinetic_lab_calcs_elastic_E(MeV_MidBack,3.016029,12,theta)[8]; 
            }

            double RangeMidAlElastic=RangeAl_umMeV->Eval(Elab);
            EffectiveBackingThickness_um*=0.5;

            KE_PostTarget=EnergyAl_MeVum->Eval(RangeMidAlElastic-EffectiveBackingThickness_um); 
            if(RangeMidAlElastic<EffectiveBackingThickness_um)KE_PostTarget=0;//////////////////////

            KinematicElasticAl->SetPoint(KinematicElasticAl->GetN(),theta,KE_PostTarget); 

        }
    }
    
   out.cd();
   KinematicElastic->Write("KinElastic");
    if(Inputs.TestInput("Oxygen")){ 
        KinematicElasticAl->Write("KinElasticO");
    }else{
        if(Inputs.TestInput("Carbon")){ 
            KinematicElasticAl->Write("KinElasticC");
        }else{
            KinematicElasticAl->Write("KinElasticAl");
        }
    }
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
    
    
    TGraph* KinematicAlphaDecay = new TGraph();
    
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
        KinematicAlphaDecay->SetPoint(KinematicAlphaDecay->GetN(),theta,KE_Alpha_PostTarget); 
    }
    
   out.cd();
    KinematicAlphaDecay->Write("KinematicAlphaDecay");
   gROOT->cd();
    
   
   TGraphErrors* AlphaCalGraph[2]={new TGraphErrors(),new TGraphErrors()};
   
   out.mkdir("ForCal/AlphaDecay");
   out.cd("ForCal/AlphaDecay");
        TH1D* dEAlphaCalc[2]={
            new TH1D("dEAlphaCalc_A","dEAlphaCalc_A;Index;E Calc. (MeV)",16,0,16),
            new TH1D("dEAlphaCalc_B","dEAlphaCalc_B;Index;E Calc. (MeV)",16,0,16)
            };
        TH1D* dEAlphaFitMax[2]={
            new TH1D("dEAlphaFitMax_A","dEAlphaFitMax_A;Index;Fit Charge",16,0,16),
            new TH1D("dEAlphaFitMax_B","dEAlphaFitMax_B;Index;Fit Charge",16,0,16)
            };
        TH1D* dEAlphaFitCentroid[2]={
            new TH1D("dEAlphaFitCentroid_A","dEAlphaFitCentroid_A;Index;Fit Charge",16,0,16),
            new TH1D("dEAlphaFitCentroid_B","dEAlphaFitCentroid_B;Index;Fit Charge",16,0,16)
            };
            
   gROOT->cd();
   
    double alphafits[2][2][16]={{{889.91,887.48,883.4,880.61,876.6,872.23,862.76,855.81,845.47,833.72,816.01,795.36,756.55,0,0,0},{895.6,895.89,896.85,892.59,900.66,898.53,891.56,888.1,870.51,876.75,867.38,858.61,788.55,0,0,0}},
    {{0,0,0,0,780.58,806.58,820.49,835.83,0,854.15,0,867.17,881.53,881.71,882.58,885.18},
    {0,0,0,0,811.73,846.91,857.77,854.53,0,869.46,0,880.36,889.62,891.12,890.29,893.03}}};

    for(int mod=2;mod<4;mod++){
        int AB=0;
        for(int chan=0;chan<16;chan++){
            DetHit dEhit(0,0, mod, chan);
            double energysum=0;
            double positionsum=0;
            for(int Edex=0;Edex<16;Edex++){
                AB=TelescopeHit::AB(dEhit);

                TVector3 PixelVec=TelescopeHit::SiliconPosDet(dEhit.Index(),Edex,AB);
                PixelVec+=TVector3(0,0,TargetZoffset);
                TVector3 Intrinsic=PixelVec;
                Intrinsic.RotateY(TMath::DegToRad()*(50-100*AB));
                
                double EffThick=std::abs(cos(Intrinsic.Theta())); // Angle though
            
                double theta=PixelVec.Theta();
                double Emit=KinematicAlphaDecay->Eval(PixelVec.Theta());
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
                dEAlphaCalc[AB]->SetBinContent(dEhit.Index(),energysum);
            }else{
                energysum=0;
            }
            
            double Mx=alphafits[mod-2][0][chan];
            double Cnt=alphafits[mod-2][0][chan];
            dEAlphaFitMax[AB]->SetBinContent(dEhit.Index(),Mx);
            dEAlphaFitCentroid[AB]->SetBinContent(dEhit.Index(),Cnt);
            
            AlphaCalGraph[AB]->SetPoint(AlphaCalGraph[AB]->GetN(),0.5*(Mx+Cnt),energysum);
            AlphaCalGraph[AB]->SetPointError(AlphaCalGraph[AB]->GetN()-1,0.5*std::abs(Mx-Cnt),0);
            
        }
    }
   out.cd("ForCal/AlphaDecay");
        AlphaCalGraph[0]->Write("AlphaDecayFitA");
        AlphaCalGraph[1]->Write("AlphaDecayFitB");
   gROOT->cd();
