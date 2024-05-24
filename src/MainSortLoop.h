    
    // Itterate through raw silicon hits, build telescope events and fill raw channel related histograms
    for(auto& dE : SidERaw){
        for(auto& E : SiERaw){
            
            if(TelescopeHit::AB(dE)!=TelescopeHit::AB(E))continue;// Check same detector

            double dT=dE.Time()-E.Time(); // Time difference
            
            dEEtime[TelescopeHit::AB(dE)]->Fill(dE.Index(),dT);
            EdEtime[TelescopeHit::AB(dE)]->Fill(E.Index(),dT);
            
            if(abs(dT)<60){//If dE inside true coiccidence timegate
                SiHits.push_back(TelescopeHit(dE,E));
            }
        }
    }
        

    // Itterate through constructed good telescope hits
    for(auto& Si : SiHits){
        
        TVector3 World=Si.GetPos(true);
        
        double EffThick=std::abs(cos(Si.GetPos(false,true).Theta()));
        double Theta=World.Theta()/TMath::DegToRad();
        double Phi=PhiOffset(World.Phi())/TMath::DegToRad();

        double E=Si.E().Charge();
        double dE=Si.dE().Charge();
        double Etot=E+dE;
        double dEdX=dE*EffThick;
        
        dEE[Si.AB()]->Fill(E,dE);
        dEEtot[Si.AB()]->Fill(Etot,dE);
        dEEi[Si.AB()][Si.dE().Index()]->Fill(E,dE);
        EdEi[Si.AB()][Si.E().Index()]->Fill(E,dE);
        dEEffEtot[Si.AB()]->Fill(Etot,dEdX);
        ThetaPhi[Si.AB()]->Fill(Theta,Phi);
        ThetaPhiSum->Fill(Theta,Phi);
        
        RunFile_dE[Si.AB()][Si.dE().Index()]->Fill(FileI,dE);
        RunFile_E[Si.AB()][Si.E().Index()]->Fill(FileI,E);
        
        // Itterate through the loaded TCutG and setting the IDGateTest vector if the current event is inside that event
        for(unsigned int g=0;g<Inputs.ParticleIDgates.size();g++){
            IDGateTest[g]=Inputs.ParticleIDgates[g]->IsInside(Etot,dEdX);
            if(IDGateTest[g]){
                    GatedSiliconTheta[g]->Fill(Etot,World.Theta()/TMath::DegToRad());
            }
        }
        
        // Plot silicon strips vs selected oposing strip to ensure fixed angle/energy
        if(Si.dE().Index()==14){
            ECal[Si.AB()]->Fill(Si.E().Index(),E);
        }
        if(Si.E().Index()==6||Si.E().Index()==9){
            dECal[Si.AB()]->Fill(Si.dE().Index(),dE);
        }
        
        // Silicon + Particle loops
        
        for(auto& gHit : HPGe){
            SiHPGeT[Si.AB()]->Fill(gHit.Index(),Si.Time()-gHit.Time());
            
            for(unsigned int g=0;g<IDGateTest.size();g++){
                if(IDGateTest[g]){
                    GateGamma[g]->Fill(gHit.Energy());
                    GamE=gHit.Energy();
                    Gami=gHit.Index();
                    gammatree.Fill();
                }
            }
            
        }
        for(auto& sHit : Solar){
            SiSolarT->Fill(sHit.Index(),Si.Time()-sHit.Time());
        }
        for(auto& lHit : LaBr){
            SiLaBrT->Fill(lHit.Index(),Si.Time()-lHit.Time());
            
            for(unsigned int g=0;g<IDGateTest.size();g++){
                if(IDGateTest[g]){
                    GateLaBr[g]->Fill(lHit.Energy());
                }
            }
        }
    }

    // Loops of gammas, ignoring silicon events
    
    
    // Symetrised by total loop twice rather than a j>i inner loop, slower but neat
    for(auto& gHit : HPGe){
        for(auto& GHit : HPGe){//Need t
            if (&gHit == &GHit)continue; //skip self comparison
                
            HPGeHPGe->Fill(gHit.Energy(),GHit.Energy());
            HPGeGammaT->Fill(gHit.Index(),gHit.Time()-GHit.Time());
        }
        
        for(auto& lHit : LaBr){
            HPGeGammaT->Fill(gHit.Index(),gHit.Time()-lHit.Time());
            LaBrGammaT->Fill(lHit.Index(),lHit.Time()-gHit.Time());
            HPGeLaBr->Fill(gHit.Energy(),lHit.Energy());
        }
    }
    
    for(auto& lHit : LaBr){
        for(auto& LHit : LaBr){
            if (&lHit == &LHit)continue; //skip self comparison
            
            LaBrGammaT->Fill(lHit.Index(),lHit.Time()-LHit.Time());
        }
    }

    
    if(EndOfRun){
        GammaTreeFile.Write(0,TObject::kWriteDelete);//Avoid those annoying multiple keys
    }
