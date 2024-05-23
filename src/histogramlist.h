

    out.cd();

    TH2D* ChanE=new TH2D("ChanE","ChanE;Channe;E",totalchans,0,totalchans,1000,0,8000);

    TH2D* dEEtime[2]={
        new TH2D("dEEtimeA","dEEtimeA;dE Channel Number;dE-E time difference",16,0,16,200,-2E3,2E3),
        new TH2D("dEEtimeB","dEEtimeB;dE Channel Number;dE-E time difference",16,0,16,200,-2E3,2E3)
    };

    TH2D* dEE[2]={
        new TH2D("dEEA","dEEA;E;dE",1000,0,8000,1000,0,8000),
        new TH2D("dEEB","dEEB;E;dE",1000,0,8000,1000,0,8000)
    };

   TH2D* dEEtot[2]={
       new TH2D("dEEtotA","dEEtotA;Etot;dE",1000,0,8000,1000,0,8000),
       new TH2D("dEEtotB","dEEtotB;Etot;dE",1000,0,8000,1000,0,8000)
    };

    TH3I* dEE3[2]={
        new TH3I("dEEA3","dEEA3",16,0,16,100,0,4000,100,0,4000),
        new TH3I("dEEB3","dEEB3",16,0,16,100,0,4000,100,0,4000)
    };


   TH2D* dEEffEtot[2]={
       new TH2D("dEEffEtotA","dEEffEtotA;E tot;dE Effective",1000,0,8000,1000,0,8000),
       new TH2D("dEEffEtotB","dEEffEtotB;E tot;dE Effective",1000,0,8000,1000,0,8000)
    };

   TH2D* ThetaPhiSum=new TH2D("ThetaPhi","ThetaPhi;Theta #theta [deg.];Phi #Phi [deg.];",180,0,180,360,0,360);

   out.mkdir("dE_Chan");
   out.cd("dE_Chan");
    TH2D* dEEi[2][16];
    for(UShort_t i=0;i<16;i++)dEEi[0][i]=new TH2D(Form("dEEA_%d",i+1),Form("dE%d vs any E A",i+1),1000,0,8000,1000,0,8000);
    for(UShort_t i=0;i<16;i++)dEEi[1][i]=new TH2D(Form("dEEB_%d",i+1),Form("dE%d vs any E B",i+1),1000,0,8000,1000,0,8000);
   out.cd();

   out.mkdir("E_Chan");
   out.cd("E_Chan");
    TH2D* EdEi[2][16];
    for(UShort_t i=0;i<16;i++)EdEi[0][i]=new TH2D(Form("EdEA_%d",i+1),Form("E%d vs any dE A",i+1),1000,0,8000,1000,0,8000);
    for(UShort_t i=0;i<16;i++)EdEi[1][i]=new TH2D(Form("EdEB_%d",i+1),Form("E%d vs any dE B",i+1),1000,0,8000,1000,0,8000);
   out.cd();

   TH2D* ThetaPhi[2]={
       new TH2D("ThetaPhiA","ThetaPhiA;Theta;Phi",180,0,180,360,0,360),
       new TH2D("ThetaPhiB","ThetaPhiB;Theta;Phi",360,0,180,360,0,360)
    };
