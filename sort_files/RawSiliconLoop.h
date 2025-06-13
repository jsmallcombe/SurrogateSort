 
 
 
     // Itterate through raw silicon hits, build telescope events and fill raw channel related histograms
    for(auto& dE : SidERaw){
            if(dE.Charge()<10)continue;
        for(auto& E : SiERaw){
            
            if(E.Charge()<10)continue;
            
            
            
            if(TelescopeHit::AB(dE)!=TelescopeHit::AB(E))continue;// Check same detector
            double dT=dE.Time()-E.Time(); // Time difference
                        
            dE_dT[TelescopeHit::AB(dE)]->Fill(dT,dE.Index());
            E_dT[TelescopeHit::AB(dE)]->Fill(dT,E.Index());
            
            UnGdEE[TelescopeHit::AB(dE)]->Fill(E.Charge(),dE.Charge());
            
            UnTGdEE[TelescopeHit::AB(dE)]->Fill(dT,E.Charge(),dE.Charge());
            
            if(abs(dT)<TelescopeBuildWindow){//If dE inside true coiccidence timegate (relaxed from 60)
                SiHits.push_back(TelescopeHit(dE,E));
                
//                 if(FilterTree)FillFilterTree=true;
            }
            
//             cout<<endl<<dT<<endl;
        }
    }
    
    if(SiHits.size()==1)if(SidERaw.size()==1)if(SiERaw.size()==1)if(FilterTree){
        
        FillFilterTree=true;
    }
    
    
    // Itterate through constructed good telescope hits
    for(auto& Si : SiHits){
        
        if(SiHits.size()==1){
            ChanADCGood->Fill(Si.dE().Index()+Si.AB()*32,Si.dE().Charge());
            ChanADCGood->Fill(Si.E().Index()+16+Si.AB()*32,Si.E().Charge());
        }
        TVector3 World=Si.GetPos(false);
        TVector3 WorldBlur=Si.GetPos(true);
        TVector3 Intrinsic=Si.GetPos(false,true);
        
        thetaphi->Fill(WorldBlur.Theta()/TMath::DegToRad(),WorldBlur.Phi()/TMath::DegToRad());
        
        double EffThick=std::abs(cos(Intrinsic.Theta())); // Angle through
        double ThetaRad=World.Theta(); // Angle target
    
        double E=Si.E().Charge();
        double dE=Si.dE().Charge();
        double Etot=E+dE;
        double dEdX=dE*EffThick;
        
        double Cal_E=Si.E().Energy();
        double Cal_dE=Si.dE().Energy();
        double Cal_dEdX=Cal_dE*EffThick;
        
        dEE[Si.AB()]->Fill(E,dE);
        dEEtot[Si.AB()]->Fill(Etot,dE);
        dEEi[Si.AB()][Si.dE().Index()]->Fill(E,dE);
        EdEi[Si.AB()][Si.E().Index()]->Fill(E,dE);
        
        dEdXEi[Si.AB()][Si.dE().Index()]->Fill(Etot,dEdX);
        
        dEEffEtot[Si.AB()]->Fill(Etot,dEdX);
        
        if(FillFilterTree)if(Inputs.CutGates.size()){
            if(!Inputs.CutGates[0]->IsInside(Etot,dEdX)){
                FillFilterTree=false;
            }
        }
        
    }
    
    if(FillFilterTree&&FilterTree){
        FilterTree->Fill();
        FillFilterTree=false;
    }
    
    if(EndOfRun){
        // End of last run
        if(jentry+1==nentries){

        }
    }

