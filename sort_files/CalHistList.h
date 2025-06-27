    TRandom3 rr;
    rr.SetSeed();
    
    bool PreCalibrated=Inputs.TestInput("PreCalibrated");
    bool DoProtons=false;
    bool DoAlphas=false;
    bool UseCalibratedGate=false;
    
    TCutG* Raw_dE_E_Charge_ElasticBeamGate[2]={new TCutG(),nullptr};
    Raw_dE_E_Charge_ElasticBeamGate[1]=Raw_dE_E_Charge_ElasticBeamGate[0];
    TCutG* Raw_dE_E_Charge_BeamGate[2]={new TCutG(),nullptr};
    Raw_dE_E_Charge_BeamGate[1]=Raw_dE_E_Charge_BeamGate[0];
    
    TCutG* Calibrated_3HeGate[2]={nullptr,nullptr};
    
    TCutG* dEdX_Etot_Charge_ProtonGate[2]={nullptr,nullptr};
    TCutG* Raw_dE_E_Charge_4HeGate[2]={nullptr,nullptr};
    
    for(unsigned int g=0;g<Inputs.CutGates.size();g++){

        if(Inputs.GateID[g]==0){
            Raw_dE_E_Charge_ElasticBeamGate[0]=Inputs.CutGates[g];
            Raw_dE_E_Charge_ElasticBeamGate[1]=Inputs.CutGates[g];
        }
        
        if(Inputs.GateID[g]==1){
            //If gate B HASNT been set, set it to be the same (if using one A+B gate only A is assigned)
            if(Raw_dE_E_Charge_BeamGate[1]==Raw_dE_E_Charge_BeamGate[0]){
                Raw_dE_E_Charge_BeamGate[1]=Inputs.CutGates[g];
            }
            Raw_dE_E_Charge_BeamGate[0]=Inputs.CutGates[g];
        }
        if(Inputs.GateID[g]==2){
            Raw_dE_E_Charge_BeamGate[1]=Inputs.CutGates[g];
        }
        
        if(Inputs.GateID[g]==3){
            dEdX_Etot_Charge_ProtonGate[0]=Inputs.CutGates[g];
            dEdX_Etot_Charge_ProtonGate[1]=Inputs.CutGates[g];
        }
        
        if(Inputs.GateID[g]==4){
            Raw_dE_E_Charge_4HeGate[0]=Inputs.CutGates[g];
            Raw_dE_E_Charge_4HeGate[1]=Inputs.CutGates[g];
        }
        if(Inputs.GateID[g]==5){
            Raw_dE_E_Charge_4HeGate[1]=Inputs.CutGates[g];
        }
        
        if(Inputs.GateID[g]==6){
            Calibrated_3HeGate[0]=Inputs.CutGates[g];
            if(Calibrated_3HeGate[1]==nullptr){Calibrated_3HeGate[1]=Inputs.CutGates[g];}
        }
        if(Inputs.GateID[g]==7){
            Calibrated_3HeGate[1]=Inputs.CutGates[g];
        }
    }
    
    if(Raw_dE_E_Charge_4HeGate[0])DoProtons=true;
    if(dEdX_Etot_Charge_ProtonGate[0])DoAlphas=true;
    if(PreCalibrated&&Calibrated_3HeGate[0])UseCalibratedGate=true;
     
    out.cd();
    out.mkdir("Raw");
    out.cd("Raw");

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
    
    
    out.cd();
    
    TH2D* dE_Check[2];
    TH2D* E_Check[2];
    TH2D* dE_E_Calibrated[2];
    TH2D* dE_Etot_Calibrated[2];
    TH2D* dEdX_Etot_SumCalibrated[2];
    TH2D* dEdX_Etot_SumCalibrated3He[2];
    TH2D* dEdX_Etot_SumCalibrated3HeAv[2];
    TH2D* dEdX_Etot_SumCalibratedProton[2];
    TH2D* dEEi_cal[2][16];
    TH2D* dedxmid_dEi_cal[2][16];
    TH2D* EdEi_cal[2][16];
    TH2D* dedxmid_Ei_cal[2][16];
    
    if(PreCalibrated){
    out.mkdir("Calibrated");
    out.cd("Calibrated");

        dE_Check[0]=new TH2D("dE_E_CalCheck_A","dE_E_CalCheck_A;dE (MeV);dE (MeV Calc.)",500,0,15,500,0,15);
        dE_Check[1]=new TH2D("dE_E_CalCheck_B","dE_E_CalCheck_B;dE (MeV);dE (MeV Calc.)",500,0,15,500,0,15);
    
        E_Check[0]=new TH2D("E_dE_CalCheck_A","E_dE_CalCheck_A;E (MeV);E (MeV Calc.)",500,0,30,250,0,30);
        E_Check[1]=new TH2D("E_dE_CalCheck_B","E_dE_CalCheck_B;E (MeV);E (MeV Calc.)",500,0,30,250,0,30);

        dE_E_Calibrated[0]=new TH2D("dE_E_Calibrated_A","dE_E_Calibrated_A;E (MeV);dE (MeV)",1000,0,40,500,0,15);
        dE_E_Calibrated[1]=new TH2D("dE_E_Calibrated_B","dE_E_Calibrated_B;E (MeV);dE (MeV)",1000,0,40,500,0,15);
    
        dE_Etot_Calibrated[0]=new TH2D("dE_Etot_Calibrated_A","dE_Etot_Calibrated_A;E+dE (MeV);dE (MeV)",1000,0,40,500,0,15);
        dE_Etot_Calibrated[1]=new TH2D("dE_Etot_Calibrated_B","dE_Etot_Calibrated_B;E+dE (MeV);dE (MeV)",1000,0,40,500,0,15);
        
        dEdX_Etot_SumCalibrated[0]=new TH2D("dEdX_E_at_dE_SumCalibrated_A","dEdX_EAtdE_Calibrated_A;E+dE0.5+dead (MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15);
        dEdX_Etot_SumCalibrated[1]=new TH2D("dEdX_E_at_dE_SumCalibrated_B","dEdX_EAtdE_Calibrated_B;E+dE0.5+dead (MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15);
        
        dEdX_Etot_SumCalibrated3He[0]=new TH2D("dEdX_E_at_dE_SumCalibrated3He_A","3He_dEdX_EatdE_Calibrated_A;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15);
        dEdX_Etot_SumCalibrated3He[1]=new TH2D("dEdX_E_at_dE_SumCalibrated3He_B","3He_dEdX_EatdE_Calibrated_B;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15);
        
        dEdX_Etot_SumCalibrated3HeAv[0]=new TH2D("dEdX_E_at_dE_SumCalibrated3HeAv_A","3He_dEdX_EatdE_Calibrated_A;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15);
        dEdX_Etot_SumCalibrated3HeAv[1]=new TH2D("dEdX_E_at_dE_SumCalibrated3HeAv_B","3He_dEdX_EatdE_Calibrated_B;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15);
        
        if(DoProtons){
            dEdX_Etot_SumCalibratedProton[0]=new TH2D("dEdX_E_at_dE_SumCalibratedProton_A","Proton_dEdX_EatdE_Calibrated_A;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15);
            dEdX_Etot_SumCalibratedProton[1]=new TH2D("dEdX_E_at_dE_SumCalibratedProton_B","Proton_dEdX_EatdE_Calibrated_B;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",1000,0,40,500,0,15);
        };
    
   out.mkdir("Calibrated/dE_Chan_cal");
   out.cd("Calibrated/dE_Chan_cal");
    for(UShort_t i=0;i<16;i++)dEEi_cal[0][i]=new TH2D(Form("dEE_calA_%d",i),Form("dE%d vs any E A;E (MeV);dE%d (MeV)",i,i),1000,0,40,500,0,15);
    for(UShort_t i=0;i<16;i++)dEEi_cal[1][i]=new TH2D(Form("dEE_calB_%d",i),Form("dE%d vs any E B;E (MeV);dE%d (MeV)",i,i),1000,0,40,500,0,15);
    for(UShort_t i=0;i<16;i++)dedxmid_dEi_cal[0][i]=new TH2D(Form("dedxmid_dE_calA_%d",i),Form("dE%d vs any E A;Energy At dE Midpoint (Calc. MeV);dE%d/dX (MeV/arb.)",i,i),1000,0,40,500,0,15);
    for(UShort_t i=0;i<16;i++)dedxmid_dEi_cal[1][i]=new TH2D(Form("dedxmid_dE_calB_%d",i),Form("dE%d vs any E B;Energy At dE Midpoint (Calc. MeV);dE%d/dX (MeV/arb.)",i,i),1000,0,40,500,0,15);
   out.cd();
   

   out.mkdir("Calibrated/E_Chan_cal");
   out.cd("Calibrated/E_Chan_cal");
    for(UShort_t i=0;i<16;i++)EdEi_cal[0][i]=new TH2D(Form("EdE_calA_%d",i),Form("E%d vs any dE A;E%d (MeV);dE (MeV)",i,i),1000,0,40,500,0,15);
    for(UShort_t i=0;i<16;i++)EdEi_cal[1][i]=new TH2D(Form("EdE_calB_%d",i),Form("E%d vs any dE B;E%d (MeV);dE (MeV)",i,i),1000,0,40,500,0,15);
    
    for(UShort_t i=0;i<16;i++)dedxmid_Ei_cal[0][i]=new TH2D(Form("dedxmid_E_calA_%d",i),Form("E%d vs any E A;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",i),1000,0,40,500,0,15);
    for(UShort_t i=0;i<16;i++)dedxmid_Ei_cal[1][i]=new TH2D(Form("dedxmid_E_calB_%d",i),Form("E%d vs any E B;Energy At dE Midpoint (Calc. MeV);dE/dX (MeV/arb.)",i),1000,0,40,500,0,15);
   }
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
    
    TH2F* InvCal_dECompare[2][16];
    TH2F* InvCal_dE[2][16];
    TH2F* InvCal_dEComb[2][16];
    TH2F* InvCal_dE4He[2][16];
    TH2F* InvCal_dE_E_Proton[2][16];
    
    TH2F* InvCal_E[2][16];
    TH2F* InvCal_ECompare[2][16];
    TH2F* InvCal_EComb[2][16];
    
    if(PreCalibrated){
        out.cd();
        out.mkdir("ForCal/ElasticDataInverse");
        out.cd("ForCal/ElasticDataInverse");
                for(int i=0;i<16;i++)InvCal_dECompare[0][i]=new TH2F(Form("InvCalCompare_dE_A_%d",i),Form("InverseCalibrationCompare_3He_dE_A_%d;dE (MeV);dE (E Calc. MeV)",i),500,0,15,500,0,15);
                for(int i=0;i<16;i++)InvCal_dECompare[1][i]=new TH2F(Form("InvCalCompare_dE_B_%d",i),Form("InverseCalibrationCompare_3He_dE_B_%d;dE (MeV);dE (E Calc. MeV))",i),500,0,15,500,0,15);
                for(int i=0;i<16;i++)InvCal_dE[0][i]=new TH2F(Form("InvCal_dE_A_%d",i),Form("InverseCalibration_3He_dE_A_%d_FromE;Charge dE%d;dE (E Calc. MeV)",i,i),500,0,3000,500,0,15);
                for(int i=0;i<16;i++)InvCal_dE[1][i]=new TH2F(Form("InvCal_dE_B_%d",i),Form("InverseCalibration_3He_dE_B_%d_FromE;Charge dE%d;dE (E Calc. MeV)",i,i),500,0,3000,500,0,15);
                for(int i=0;i<16;i++)InvCal_dEComb[0][i]=new TH2F(Form("InvCal_dEComb_A_%d",i),Form("Combine_InverseCalibrations_3He_A_%d;Charge dE; dE (MeV)",i),500,0,3000,500,0,15);
                for(int i=0;i<16;i++)InvCal_dEComb[1][i]=new TH2F(Form("InvCal_dEComb_B_%d",i),Form("Combine_InverseCalibrations_3He_B_%d;Charge dE; dE (MeV)",i),500,0,3000,500,0,15);
                if(DoAlphas)for(int i=0;i<16;i++)InvCal_dE4He[0][i]=new TH2F(Form("InvCal4He_dE_A_%d",i),Form("InverseCalibration_4He_dE_A_%d_FromE;Charge dE%d;dE (Calc. MeV)",i,i),500,0,3000,500,0,15);
                 if(DoAlphas)for(int i=0;i<16;i++)InvCal_dE4He[1][i]=new TH2F(Form("InvCal4He_dE_B_%d",i),Form("InverseCalibration_4He_dE_B_%d_FromE;Charge dE%d;dE (Calc. MeV)",i,i),500,0,3000,500,0,15);
                 if(DoProtons)for(int i=0;i<16;i++)InvCal_dE_E_Proton[0][i]=new TH2F(Form("Cal_dE_E_Proton_A%d",i),Form("Proton_dE%d_CalcFromE_A;dE%d Charge;dE Proton (Calc. MeV)",i,i),500,0,3000,500,0,15);
                 if(DoProtons)for(int i=0;i<16;i++)InvCal_dE_E_Proton[1][i]=new TH2F(Form("Cal_dE_E_Proton_B%d",i),Form("Proton_dE%d_CalcFromE_B;dE%d Charge;dE Proton (Calc. MeV)",i,i),500,0,3000,500,0,15);
        out.cd();
        
        
            
        out.mkdir("ForCal/ElasticDataInverse_E");
        out.cd("ForCal/ElasticDataInverse_E");
                for(int i=0;i<16;i++)InvCal_ECompare[0][i]=new TH2F(Form("InvCalCompare_E_A_%d",i),Form("InverseCalibrationCompare_3He_E_A_%d;E (MeV);E (dE Calc. MeV)",i),500,0,30,500,0,30);
                for(int i=0;i<16;i++)InvCal_ECompare[1][i]=new TH2F(Form("InvCalCompare_E_B_%d",i),Form("InverseCalibrationCompare_3He_E_B_%d;E (MeV);E (dE Calc. MeV))",i),500,0,30,500,0,30);
                for(int i=0;i<16;i++)InvCal_E[0][i]=new TH2F(Form("InvCal_E_A_%d",i),Form("InverseCalibration_3He_E_A_%d_FromE;Charge E%d;E (E Calc. MeV)",i,i),500,0,6000,500,0,30);
                for(int i=0;i<16;i++)InvCal_E[1][i]=new TH2F(Form("InvCal_E_B_%d",i),Form("InverseCalibration_3He_E_B_%d_FromE;Charge E%d;E (E Calc. MeV)",i,i),500,0,6000,500,0,30);
                for(int i=0;i<16;i++)InvCal_EComb[0][i]=new TH2F(Form("InvCal_EComb_A_%d",i),Form("Combine_InverseCalibrations_3He_A_%d;Charge E; E (MeV)",i),500,0,6000,500,0,30);
                for(int i=0;i<16;i++)InvCal_EComb[1][i]=new TH2F(Form("InvCal_EComb_B_%d",i),Form("Combine_InverseCalibrations_3He_B_%d;Charge E; E (MeV)",i),500,0,6000,500,0,30);
        out.cd();
   
    }
   

   
    // Per dE channel gate for "cleanly" selecting elastic on light mass target element
//     double ElasticAlLow[16]={1250,1300,1400,1500,1600,1670,1770,1850,1920,2000,2050,2100,2150,0,0,0}; 
//     double ElasticAlHigh[16]={1420,1500,1600,1700,1760,1830,1920,2010,2120,2200,2250,2300,2350,0,0,0}; 

//     double ElasticAlCent[2][16]={{2620,2550,2575,2590,2605,2675,2705,2750,2790,2840,2910,3000,3040,3110,0,0},        {2620,2550,2575,2590,2605,2675,2705,2750,2790,2840,2910,3000,3040,3110,0,0}}; //Jan2025

    double ElasticAlCent[2][16]={{0,4100,4070,4020,3985,3910,3850,3780,3700,3620,3510,3400,3310,000,000,000},{0,4270,4250,4110,4060,4000,3850,3760,3690,3590,3480,3380,3280,3170,000,000}}; //Feb2025
    
    
    if(Inputs.TestInput("FebElasticRun")){
        double FebElasticRun[2][16]={{000,5190,5170,5145,5105,5055,5010,4975,4915,4820,4770,4700,4620,4550,000,000},{5170,5190,5180,5140,5105,5050,5000,4960,4900,4800,4740,4680,4610,4510,4450,000}}; //Feb2025AlRun
        for(int i=0;i<2;i++){for(int j=0;j<16;j++){ElasticAlCent[i][j]=FebElasticRun[i][j];}}
    }
    
    for(int a=0;a<2;a++){
        if(Inputs.ArrayInputs.size()>a){
            for(int i=0;i<Inputs.ArrayInputs[a].size();i++){
                if(i>15)break;
                ElasticAlCent[a][i]=Inputs.ArrayInputs[a][i];
            }
        }
    }
    
   
    double ElasticCut=4900;//Jan2025
    if(Inputs.TestInput("ElasticCut")){
        ElasticCut=Inputs.GetInput("ElasticCut");
    }
    
    double InelasticExcitationMeV=0;
    if(Inputs.TestInput("InelasticEx")){
        InelasticExcitationMeV=Inputs.GetInput("InelasticEx");
    }
    
    bool ElasticInTarget=false;
    bool Oxygen=false;
    bool Carbon=false;
    double ElasticMassAmu=26.981539;
    if(Inputs.TestInput("Carbon")){ 
        ElasticMassAmu=12;
        Carbon=true;
    }
    if(Inputs.TestInput("Oxygen")){
        ElasticMassAmu=15.999;
        ElasticInTarget=true;
    }
    if(Inputs.TestInput("ElasticMassAmu")){
        ElasticMassAmu=Inputs.GetInput("ElasticMassAmu");
    }
    if(Inputs.TestInput("ElasticInTarget")){
        ElasticInTarget=true;
    }
    
    
    #include "src/RangeData.h"
   
    double BackRange0=RangeAl_umMeV->Eval(beam_energy_MeV);
    double MeV_MidBack=EnergyAl_MeVum->Eval(BackRange0-AlBacking_half_um);
    double MeV_PostBack=EnergyAl_MeVum->Eval(BackRange0-AlBacking_half_um*2);
    double TargRange0=RangeAm_umMeV->Eval(MeV_PostBack);
    double MeV_MidTarg=EnergyAm_MeVum->Eval(TargRange0-AmHalf_um);
    
    // If the geometry is in the mirrored configuration, the beam enters the target first and the backing second.
    if(MirrorZ){
        TargRange0=RangeAm_umMeV->Eval(beam_energy_MeV);
        MeV_MidTarg=EnergyAm_MeVum->Eval(TargRange0-AmHalf_um);
        double MeV_PosTarg=EnergyAm_MeVum->Eval(TargRange0-target_um);
        BackRange0=RangeAl_umMeV->Eval(MeV_PosTarg);
        MeV_MidBack=EnergyAl_MeVum->Eval(BackRange0-AlBacking_half_um);
        MeV_PostBack=EnergyAl_MeVum->Eval(BackRange0-AlBacking_half_um*2);
    }
    
    cout<<endl<<"ENERGY AT CENTER OF BACKING "<< MeV_MidBack<<" MeV";
    cout<<endl<<"ENERGY AT CENTER OF TARGET "<< MeV_MidTarg<<" MeV";
    
    TGraph* KinematicElasticSet=new TGraph();
    TGraph* KinematicElasticSetAl=new TGraph();
    
    for(int i=1;i<360;i++){
        
//         4.002603
//          243.0613727
//          3.016029
        // As the target is reversed when MirrorZ, this section does not need to change.
        
        double theta=i*TMath::Pi()/360;
        
        double Elab=kinetic_lab_calcs_elastic_E(MeV_MidTarg,3.016029,243.0613727,theta)[8]; 
        
        double RangeTargElastic=RangeAm_umMeV->Eval(Elab);
    
        double EffectiveTargetThickness_um=AmHalf_um/std::abs(cos(theta));
        
        double EnergyPostAmElastic=EnergyAm_MeVum->Eval(RangeTargElastic-EffectiveTargetThickness_um); 
        if(RangeTargElastic<EffectiveTargetThickness_um)EnergyPostAmElastic=0;

        // The energy is calcualted assuming the particle is also penetrating the backing.
        // This should depend on theta, but, as we ONLY detect particles in this direction, there is no need to complicate.
        
        double RangePostAmElastic=RangeAl_umMeV->Eval(EnergyPostAmElastic);
        if(EnergyPostAmElastic<=0)RangePostAmElastic=0;///////////////
                
        double EffectiveBackingThickness_um=2*AlBacking_half_um/std::abs(cos(theta));

        double KE_PostTarget=EnergyAl_MeVum->Eval(RangePostAmElastic-EffectiveBackingThickness_um); 
        if(RangePostAmElastic<EffectiveBackingThickness_um)KE_PostTarget=0;//////////////////////
        
        KinematicElasticSet->SetPoint(KinematicElasticSet->GetN(),theta,KE_PostTarget); 
        
      
        ///// The next part considered [in]elastic scattering on something other than the target element, i.e. backing and oxidisation 
        
        if(InelasticExcitationMeV>0){
            Elab=kinetic_lab_calcs_E(MeV_MidTarg,3.016029,ElasticMassAmu,theta,3.016029,ElasticMassAmu+(InelasticExcitationMeV/jam_phys_amu))[8];
        }else{
            Elab=kinetic_lab_calcs_elastic_E(MeV_MidTarg,3.016029,ElasticMassAmu,theta)[8]; 
        }
    
        if(ElasticInTarget){  
            //// Elastic is in the Target i.e. Oxygen In Target Oxide

            RangeTargElastic=RangeAm_umMeV->Eval(Elab);
            EffectiveTargetThickness_um=AmHalf_um/std::abs(cos(theta));
            EnergyPostAmElastic=EnergyAm_MeVum->Eval(RangeTargElastic-EffectiveTargetThickness_um); 
            if(RangeTargElastic<EffectiveTargetThickness_um)EnergyPostAmElastic=0;
            RangePostAmElastic=RangeAl_umMeV->Eval(EnergyPostAmElastic);
            if(EnergyPostAmElastic<=0)RangePostAmElastic=0;///////////////
            EffectiveBackingThickness_um=2*AlBacking_half_um/std::abs(cos(theta));//Duplicating above setting
            KE_PostTarget=EnergyAl_MeVum->Eval(RangePostAmElastic-EffectiveBackingThickness_um); 
            if(RangePostAmElastic<EffectiveBackingThickness_um)KE_PostTarget=0;//////////////////////
            KinematicElasticSetAl->SetPoint(KinematicElasticSetAl->GetN(),theta,KE_PostTarget);     
        }else{
            //// Elastic on Aluminium backing (or another contaminant) 

            double RangeMidAlElastic=RangeAl_umMeV->Eval(Elab);
            EffectiveBackingThickness_um*=0.5;

            KE_PostTarget=EnergyAl_MeVum->Eval(RangeMidAlElastic-EffectiveBackingThickness_um); 
            if(RangeMidAlElastic<EffectiveBackingThickness_um)KE_PostTarget=0;//////////////////////

            KinematicElasticSetAl->SetPoint(KinematicElasticSetAl->GetN(),theta,KE_PostTarget); 

        }
    }
    
   out.cd();
   KinematicElasticSet->Write("KinElastic");
   stringstream kk;
   kk<<"KinElastic"<<(int)std::round(ElasticMassAmu);
   if(Oxygen)kk<<"O";
   if(Carbon)kk<<"C";
   if(std::round(ElasticMassAmu)==27)kk<<"O";
   KinematicElasticSetAl->Write(kk.str().c_str());
   
   gROOT->cd();
   
    #ifdef NOSPLINES
        TGraph *KinematicElastic = KinematicElasticSet;
        TGraph *KinematicElasticAl = KinematicElasticSetAl;  
    #else
        TSpline3 *KinematicElastic = new TSpline3("KinematicElastic",KinematicElasticSet);  
        TSpline3 *KinematicElasticAl = new TSpline3("KinematicElasticAl",KinematicElasticSetAl);  
    #endif  
    
//     TGraph* KinematicAlphaDecay = new TGraph();
//     
//     for(int i=1;i<360;i++){
//         double theta=i*TMath::Pi()/360;
//         double EAlpha=5.2753;
//         double RangeTargAlphaDecay=AlphaTarget_Range_umMeV->Eval(EAlpha);
//         double EffectiveTargetThickness_um=AmHalf_um/std::abs(cos(theta));
//         double EnergyPostAmAlpha=AlphaTarget_Energy_MeVum->Eval(RangeTargAlphaDecay-EffectiveTargetThickness_um); 
//         if(RangeTargAlphaDecay<EffectiveTargetThickness_um)EnergyPostAmAlpha=0;
//         double RangePostAmAlpha=AlphaAluminium_Range_umMeV->Eval(EnergyPostAmAlpha);
//         if(EnergyPostAmAlpha<=0)RangePostAmAlpha=0;///////////////
//         double EffectiveBackingThickness_um=2*AlBacking_half_um/std::abs(cos(theta));
//         double KE_Alpha_PostTarget=AlphaSilicon_Energy_MeVum->Eval(RangePostAmAlpha-EffectiveBackingThickness_um); 
//         if(RangePostAmAlpha<EffectiveBackingThickness_um)KE_Alpha_PostTarget=0;//////////////////////
//         KinematicAlphaDecay->SetPoint(KinematicAlphaDecay->GetN(),theta,KE_Alpha_PostTarget); 
//     }
//     
    
    
//    out.cd();
//     KinematicAlphaDecay->Write("KinematicAlphaDecay");
//    gROOT->cd();
    
//    TGraphErrors* AlphaCalGraph[2]={new TGraphErrors(),new TGraphErrors()};
//    
//    out.mkdir("ForCal/AlphaDecay");
//    out.cd("ForCal/AlphaDecay");
//         TH1D* dEAlphaCalc[2]={
//             new TH1D("dEAlphaCalc_A","dEAlphaCalc_A;Index;E Calc. (MeV)",16,0,16),
//             new TH1D("dEAlphaCalc_B","dEAlphaCalc_B;Index;E Calc. (MeV)",16,0,16)
//             };
//         TH1D* dEAlphaFitMax[2]={
//             new TH1D("dEAlphaFitMax_A","dEAlphaFitMax_A;Index;Fit Charge",16,0,16),
//             new TH1D("dEAlphaFitMax_B","dEAlphaFitMax_B;Index;Fit Charge",16,0,16)
//             };
//         TH1D* dEAlphaFitCentroid[2]={
//             new TH1D("dEAlphaFitCentroid_A","dEAlphaFitCentroid_A;Index;Fit Charge",16,0,16),
//             new TH1D("dEAlphaFitCentroid_B","dEAlphaFitCentroid_B;Index;Fit Charge",16,0,16)
//             };
//             
//    gROOT->cd();
//    
//     double alphafits[2][2][16]={{{889.91,887.48,883.4,880.61,876.6,872.23,862.76,855.81,845.47,833.72,816.01,795.36,756.55,0,0,0},{895.6,895.89,896.85,892.59,900.66,898.53,891.56,888.1,870.51,876.75,867.38,858.61,788.55,0,0,0}},
//     {{0,0,0,0,780.58,806.58,820.49,835.83,0,854.15,0,867.17,881.53,881.71,882.58,885.18},
//     {0,0,0,0,811.73,846.91,857.77,854.53,0,869.46,0,880.36,889.62,891.12,890.29,893.03}}};
// 
//     for(int mod=2;mod<4;mod++){
//         int AB=0;
//         for(int chan=0;chan<16;chan++){
//             DetHit dEhit(0,0, mod, chan);
//             double energysum=0;
//             double positionsum=0;
//             for(int Edex=0;Edex<16;Edex++){
//                 AB=TelescopeHit::AB(dEhit);
// 
//                 TVector3 PixelVec=TelescopeHit::SiliconPosDet(dEhit.Index(),Edex,AB);
//                 PixelVec+=TVector3(0,0,TargetZoffset);
//                 TVector3 Intrinsic=PixelVec;
//                 Intrinsic.RotateY(TMath::DegToRad()*(50-100*AB));
//                 
//                 double EffThick=std::abs(cos(Intrinsic.Theta())); // Angle though
//             
//                 double theta=PixelVec.Theta();
//                 double Emit=KinematicAlphaDecay->Eval(PixelVec.Theta());
//                 double SiRange=AlphaSilicon_Range_umMeV->Eval(Emit);
//             
//                 double EffDead=dE_FrontDead_um/EffThick;
//             
//                 double Energy_Active_dE=AlphaSilicon_Energy_MeVum->Eval(SiRange-EffDead);
//             
//                 if(Energy_Active_dE>0){
//                     double RSq=pow(PixelVec.Mag(),2);
//                     energysum+=Energy_Active_dE/RSq;
//                     positionsum+=1/RSq;
//                 }
//             
//             }
//             if(positionsum>0){
//                 energysum/=positionsum;
//                 dEAlphaCalc[AB]->SetBinContent(dEhit.Index(),energysum);
//             }else{
//                 energysum=0;
//             }
//             
//             double Mx=alphafits[mod-2][0][chan];
//             double Cnt=alphafits[mod-2][0][chan];
//             dEAlphaFitMax[AB]->SetBinContent(dEhit.Index(),Mx);
//             dEAlphaFitCentroid[AB]->SetBinContent(dEhit.Index(),Cnt);
//             
//             AlphaCalGraph[AB]->SetPoint(AlphaCalGraph[AB]->GetN(),0.5*(Mx+Cnt),energysum);
//             AlphaCalGraph[AB]->SetPointError(AlphaCalGraph[AB]->GetN()-1,0.5*std::abs(Mx-Cnt),0);
//             
//         }
//     }
//    out.cd("ForCal/AlphaDecay");
//         AlphaCalGraph[0]->Write("AlphaDecayFitA");
//         AlphaCalGraph[1]->Write("AlphaDecayFitB");
//    gROOT->cd();
   
