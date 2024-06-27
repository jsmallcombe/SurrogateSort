    
    
    // Itterate through raw silicon hits, build telescope events and fill raw channel related histograms
    for(auto& dE : SidERaw){
        for(auto& E : SiERaw){
            
            if(TelescopeHit::AB(dE)!=TelescopeHit::AB(E))continue;// Check same detector

            double dT=dE.Time()-E.Time(); // Time difference
            
            if(abs(dT)<100){//If dE inside true coiccidence timegate (relaxed from 60)
                SiHits.push_back(TelescopeHit(dE,E));
            }
            
            if(FilterTree)FillFilterTree=true;
        }
    }
        
    // Itterate through constructed good telescope hits
    for(auto& Si : SiHits){
        
        TVector3 World=Si.GetPos(false);
        TVector3 Intrinsic=Si.GetPos(false,true);
        
        World+=TVector3(0,0,TargetZoffset);
        Intrinsic=World;
        Intrinsic.RotateY(TMath::DegToRad()*(50-100*Si.AB()));
        
        double EffThick=std::abs(cos(Intrinsic.Theta())); // Angle though
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
        
        dE_E_Calibrated[Si.AB()]->Fill(Cal_E,Cal_dE);
        dE_Etot_Calibrated[Si.AB()]->Fill(Cal_E+Cal_dE,Cal_dE);
        
        dEEi_cal[Si.AB()][Si.dE().Index()]->Fill(Cal_E,Cal_dE);
        EdEi_cal[Si.AB()][Si.E().Index()]->Fill(Cal_E,Cal_dE);

        
        //// Do convoluted correction Assuming a He3  /////

        // Range before E detector active region
        double PreRange=He3Silicon_Range_umMeV->Eval(Cal_E);  
        
        // Energy at 3 points (start,middle,end) of the active region of the dE, calulcated exclusively from the E detector energy
        double CalcEnergyPostDeltaActive=He3Silicon_Energy_MeVum->Eval(PreRange+(E_Dead_um+dE_BackDead_um)/EffThick); 
        double CalcEnergyMidDeltaActive=He3Silicon_Energy_MeVum->Eval(PreRange+(E_Dead_um+dE_Thickness_um*0.5)/EffThick); 
        double CalcEnergyPreDeltaActive=He3Silicon_Energy_MeVum->Eval(PreRange+(E_Dead_um+dE_Thickness_um-dE_FrontDead_um)/EffThick);
        
        // dE/dX "measurement" from the above range calculations
        double dECalc=CalcEnergyPreDeltaActive-CalcEnergyPostDeltaActive;
        double dEdXCalc=dECalc*EffThick;
        
        // dE/dX from the actual dE detector energy
        double dEdXDirect=Cal_dE*EffThick;
        
        // Sum energy at midpoint using actual dE+E measurement (and deadlayer projection from E)
        double EnergyMidDeltaActive=CalcEnergyPostDeltaActive+Cal_dE*0.5;
        
        // Taking and average because the dE calibration is a little wonky 
        // Should compare without this average once the calibration is better
        double EnergyMidDeltaActive_Average=(CalcEnergyMidDeltaActive+EnergyMidDeltaActive)*0.5;
        double dEdX_Average=(dEdXDirect+dEdXCalc)*0.5;

        dEdX_Etot_SumCalibrated[Si.AB()]->Fill(EnergyMidDeltaActive,dEdXDirect);
        
        dEdX_Etot_SumCalibrated3He[Si.AB()]->Fill(EnergyMidDeltaActive_Average,dEdX_Average);
        
        
        //// Do convoluted correction Assuming a Proton  /////
        
        // Range before E detector active region
        double ProtonPreRange=ProtonsSilicon_Range_umMeV->Eval(Cal_E);  
        
        // Energy at 3 points (start,middle,end) of the active region of the dE, calulcated exclusively from the E detector energy
        double ProtonCalcEnergyPostDeltaActive=ProtonsSilicon_Energy_MeVum->Eval(ProtonPreRange+(E_Dead_um+dE_BackDead_um)/EffThick); 
        double ProtonCalcEnergyMidDeltaActive=ProtonsSilicon_Energy_MeVum->Eval(ProtonPreRange+(E_Dead_um+dE_Thickness_um*0.5)/EffThick); 
        double ProtonCalcEnergyPreDeltaActive=ProtonsSilicon_Energy_MeVum->Eval(ProtonPreRange+(E_Dead_um+dE_Thickness_um-dE_FrontDead_um)/EffThick);
        
        // dE/dX "measurement" from the above range calculations
        double ProtondECalc=ProtonCalcEnergyPreDeltaActive-ProtonCalcEnergyPostDeltaActive;
        double ProtondEdXCalc=ProtondECalc*EffThick;
        
        // Sum energy at midpoint using actual dE+E measurement (and deadlayer projection from E)
        double ProtonEnergyMidDeltaActive=ProtonCalcEnergyPostDeltaActive+Cal_dE*0.5;
        
        // Taking and average because the dE calibration is a little wonky 
        // Should compare without this average once the calibration is better
        double ProtonEnergyMidDeltaActive_Average=(ProtonCalcEnergyMidDeltaActive+ProtonEnergyMidDeltaActive)*0.5;
        double ProtondEdX_Average=(dEdXDirect+ProtondEdXCalc)*0.5;
        
        dEdX_Etot_SumCalibratedProton[Si.AB()]->Fill(ProtonEnergyMidDeltaActive_Average,ProtondEdX_Average);
        


                
            //GatedSilicon_dEE[g][Si.AB()]->Fill(E,dE);
//             ;
        
        bool IsBeam=Raw_dE_E_Charge_BeamGate[Si.AB()]->IsInside(Etot,dEdX);
        
        double KE_PostTarget=0;
        
        if(Raw_dE_E_Charge_ElasticBeamGate[Si.AB()]->IsInside(Etot,dEdX)){
            // Gate elastic on americium
            KE_PostTarget=KinematicElastic->Eval(ThetaRad);//Get energy of He3 from pre-solved kinematics
        }
        
        // Gate on all beam and then energy range gate strip by strip to focus on oxygen elastics
        if(IsBeam&&ElasticAlHigh[Si.dE().Index()]>E&&ElasticAlLow[Si.dE().Index()]<E){  
            dEEi_Al[Si.AB()][Si.dE().Index()]->Fill(E,dE);
            KE_PostTarget=KinematicElasticAl->Eval(ThetaRad);//Get energy of He3 from pre-solved kinematics
            
        }
                                    
        // If we have determined an absolute incident particle energy  from elastics
        if(KE_PostTarget>0){
            double SiRange=He3Silicon_Range_umMeV->Eval(KE_PostTarget);
            double Energy_Active_E=He3Silicon_Energy_MeVum->Eval(SiRange-(dE_Thickness_um+E_Dead_um)/EffThick);
            
//              double Energy_Active_dE=(KE_PostTarget-Energy_Active_E)*TotalDeadTodERatio;
            
            double Energy_Active_dE=He3Silicon_Energy_MeVum->Eval(SiRange-(dE_FrontDead_um/EffThick))-He3Silicon_Energy_MeVum->Eval(SiRange-(dE_Thickness_um-dE_BackDead_um)/EffThick);

            CalElast_E[Si.AB()][Si.E().Index()]->Fill(E,Energy_Active_E);
            CalElast_dE[Si.AB()][Si.dE().Index()]->Fill(dE,Energy_Active_dE);
        }
                   
             
                   
                   
                   
        if(IsBeam){
            InvCal_dESelf[Si.AB()][Si.dE().Index()]->Fill(Cal_dE,dECalc);
            
            InvCal_dEComb[Si.AB()][Si.dE().Index()]->Fill(dE,dECalc);
            InvCal_dEComb[Si.AB()][Si.dE().Index()]->Fill(dE,Cal_dE);
        }    
        
        if(dEdX_Etot_Charge_ProtonGate[Si.AB()]->IsInside(E,dE)){
            InvCal_dE_E_Proton[Si.AB()][Si.dE().Index()]->Fill(dE,ProtondECalc);
        }
        
        
        
        if(Raw_dE_E_Charge_3HeGate[Si.AB()]->IsInside(Etot,dEdX)){// special gate that gets those which dont see elastic
            InvCal_dE[Si.AB()][Si.dE().Index()]->Fill(dE,dECalc);
        }
        
        if(Raw_dE_E_Charge_4HeGate[Si.AB()]->IsInside(Etot,dEdX)){// special gate that gets those which dont see elastic
            
            PreRange=ASTAR_4He_Silicon_Range_um_MeV->Eval(Cal_E);  
            CalcEnergyPostDeltaActive=ASTAR_4He_Silicon_Energy_MeV_um->Eval(PreRange+(E_Dead_um+dE_BackDead_um)/EffThick); 
            CalcEnergyPreDeltaActive=ASTAR_4He_Silicon_Energy_MeV_um->Eval(PreRange+(E_Dead_um+dE_Thickness_um-dE_FrontDead_um)/EffThick);
            dECalc=CalcEnergyPreDeltaActive-CalcEnergyPostDeltaActive;
            
            InvCal_dE4He[Si.AB()][Si.dE().Index()]->Fill(dE,dECalc);
        }
        
        
    }
    
    
    
    if(FillFilterTree&&FilterTree){
        FilterTree->Fill();
        FillFilterTree=false;
    }
    
    if(EndOfRun){
        if(FilterTreeFile)FilterTreeFile->Write(0,TObject::kWriteDelete);//Avoid those annoying multiple keys
        
        // End of last run
        if(jentry+1==nentries){
            if(FilterTreeFile)FilterTreeFile->Close();
        }
    }
    

