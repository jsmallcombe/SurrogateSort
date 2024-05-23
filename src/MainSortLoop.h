    
    for(auto dE : SidERaw){
        for(auto E : SiERaw){
            
            if(TelescopeHit::AB(dE)!=TelescopeHit::AB(E))continue;// Check same module

            double dT=dE.Time()-E.Time(); // Time difference
            dEEtime[TelescopeHit::AB(dE)]->Fill(dE.Index(),dT);
            
            if(abs(dT)<60){//If dE inside true coiccidence timegate
                SiHits.push_back(TelescopeHit(dE,E));
            }
        }
    }
        

    for(auto Si : SiHits){
        
        TVector3 World=Si.GetPos(true);
        
        double EffThick=std::abs(cos(Si.GetPos(false).Theta()));
        double Theta=World.Theta()/TMath::DegToRad();
        double Phi=PhiOffset(World.Phi())/TMath::DegToRad();

        double E=Si.E().Charge();
        double dE=Si.dE().Charge();
        
        dEE[Si.AB()]->Fill(E,dE);
        dEEtot[Si.AB()]->Fill(E+dE,dE);
        dEEi[Si.AB()][Si.dE().Index()]->Fill(E,dE);
        EdEi[Si.AB()][Si.E().Index()]->Fill(E,dE);
        dEEffEtot[Si.AB()]->Fill(E+dE,dE*EffThick);
        ThetaPhi[Si.AB()]->Fill(Theta,Phi);
        ThetaPhiSum->Fill(Theta,Phi);

    }
