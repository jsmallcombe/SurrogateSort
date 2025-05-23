 
 for(auto& dE : SidERaw){
    for(auto& E : SiERaw){
        
        bool Select=((dE.Index()==6||dE.Index()==8)&&(E.Index()<6)&&(E.Index()!=2)&&(E.Index()!=3));

        if(TelescopeHit::AB(dE)!=TelescopeHit::AB(E))continue;// Check same detector
        double dT=dE.Time()-E.Time(); // Time difference
        
        dE_dT[TelescopeHit::AB(dE)]->Fill(dT,dE.Index());
        E_dT[TelescopeHit::AB(dE)]->Fill(dT,E.Index());
        
        for(unsigned int g=0;g<Inputs.CutGates.size();g++){
            if(Inputs.GateID[g]==0){
                
                if(abs(dT)<100){
                    if(Inputs.CutGates[g]->IsInside(E.Energy(),dE.Energy())){
                        if(TelescopeHit::AB(dE)==0){
                            Run_A_arb->Fill(jentry);
                            RunFileCount[0]->Fill(FileI);
                        }else{
                            Run_B_arb->Fill(jentry);
                            RunFileCount[1]->Fill(FileI);
                        }
                        
                    }
                }
            }
        }
        
        if(TelescopeHit::AB(dE)!=1)continue;// Only B detector

            if(Select)dTselect->Fill(dT);
            dTB->Fill(dT);
   
           for(unsigned int g=0;g<Inputs.CutGates.size();g++){
                if(Inputs.GateID[g]==3){
                    if(Inputs.CutGates[g]->IsInside(E.Energy(),dE.Energy())){
                        dTgate->Fill(dT);
                    }
                }
            }
        
        if(abs(dT)<100){
            
            dE_E_Raw_Sum->Fill(E.Energy(),dE.Energy());
            
            
            dE_E_B->Fill(E.Energy(),dE.Energy());
            if(!Select)dE_E_B_Selected->Fill(E.Energy(),dE.Energy());
            
            for(unsigned int g=0;g<Inputs.CutGates.size();g++){
                if(Inputs.GateID[g]==3){
                    if(Inputs.CutGates[g]->IsInside(E.Energy(),dE.Energy())){
                        dE_E_B_Gate->Fill(E.Energy(),dE.Energy());
                        ChandEE->Fill(E.Index(),dE.Index());
                        Run_Gate->Fill(jentry);
                        RunFileCount[3]->Fill(FileI);
                    }
                }
            }
            
            
        }
    }
 }

if(EndOfRun){

    // End of last run
    if(jentry+1==nentries){
        
        for(int b=1;b<=Run_A_arb->GetNbinsX();b++){
            double A=Run_A_arb->GetBinContent(b);
            double B=Run_B_arb->GetBinContent(b);
            if(B>0){
                Run_AB_Ratio->SetBinContent(b,A/B);
            }
        }
        
        
        for(int b=1;b<=RunFileCount[0]->GetNbinsX();b++){
            double A=RunFileCount[0]->GetBinContent(b);
            double B=RunFileCount[1]->GetBinContent(b);
            if(B>0){
                RunFileCount[2]->SetBinContent(b,A/B);
            }
        }
        
        
    }
}
