
    bool FillFilterTree=false;
    
                
    out.cd();
    
    TH2D* ChanADCGood=new TH2D("ChanADCGood","ChanADCGood;Channel;ADC Charge",64,0,64,1000,0,8000);

    TH2D* thetaphi=new TH2D("ThetaPhi","ThetaPhi;Theta;Phi",1800,0,180,3600,-180,180);
    
    TH2D* dE_dT[2]={
        new TH2D("dE_dT_A","dE_dT_A;dT;i_dE",400,-2000,2000,16,0,16),
        new TH2D("dE_dT_B","dE_dT_B;dT;i_dE",400,-2000,2000,16,0,16)
    };
    TH2D* E_dT[2]={
        new TH2D("E_dT_A","E_dT_A;dT;i_E",400,-2000,2000,16,0,16),
        new TH2D("E_dT_B","E_dT_B;dT;i_E",400,-2000,2000,16,0,16)
    };
    
    TH3I* UnTGdEE[2]={
        new TH3I("TEdEA","T;Charge_E_dE_A;Charge E;Charge dE",200,-10000,10000,1000,0,8000,500,0,4000),
        new TH3I("TEdEB","T;Charge_E_dE_B;Charge E;Charge dE",200,-10000,10000,1000,0,8000,500,0,4000)
    };
    
    out.cd();
    out.mkdir("Raw");
    out.cd("Raw");

        TH2D* UnGdEE[2]={
            new TH2D("UngatedCharge_E_dE_A","Charge_E_dE_A;Charge E;Charge dE",1000,0,8000,500,0,4000),
            new TH2D("UngatedCharge_E_dE_B","Charge_E_dE_B;Charge E;Charge dE",1000,0,8000,500,0,4000)
        };
        
        
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

    

