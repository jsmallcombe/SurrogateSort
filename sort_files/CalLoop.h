    
    
    // Itterate through raw silicon hits, build telescope events and fill raw channel related histograms
    for(auto& dE : SidERaw){
        for(auto& E : SiERaw){
            
            if(TelescopeHit::AB(dE)!=TelescopeHit::AB(E))continue;// Check same detector

            double dT=dE.Time()-E.Time(); // Time difference
            
            if(abs(dT)<TelescopeBuildWindow){//If dE inside true coiccidence timegate (relaxed from 60)
                SiHits.push_back(TelescopeHit(dE,E));
            }
            
        }
    }
        
    // Itterate through constructed good telescope hits
    for(auto& Si : SiHits){
        
        //Initial vector calcs
        TVector3 World=Si.GetPos(false);
        TVector3 Intrinsic=Si.GetPos(false,true);
        
        double EffThick=std::abs(cos(Intrinsic.Theta())); // Angle though
        double ThetaRad=World.Theta(); // Angle target
    
        double E=Si.E().Charge();
        double dE=Si.dE().Charge();
        double Etot=E+dE;
        double dEdX=dE*EffThick;
        
        double Cal_E=Si.E().Energy();
        double Cal_dE=Si.dE().Energy();
        double Cal_dEdX=Cal_dE*EffThick;
        double Cal_Etot=Cal_E+Cal_dE;
        
        // dE/dX from the actual dE detector energy
        double dEdXDirect=Cal_dE*EffThick;
                
        // Fill basic histograms
            
        bool IsBeam=Raw_dE_E_Charge_BeamGate[Si.AB()]->IsInside(Etot,dEdX); 
        
        dEE[Si.AB()]->Fill(E,dE);
        dEEtot[Si.AB()]->Fill(Etot,dE);
        dEEi[Si.AB()][Si.dE().Index()]->Fill(E,dE);
        EdEi[Si.AB()][Si.E().Index()]->Fill(E,dE);
        
        dEdXEi[Si.AB()][Si.dE().Index()]->Fill(Etot,dEdX);
        
        if(IsBeam){dEEffEtot[Si.AB()]->Fill(Etot,dEdX);}
        
        if(PreCalibrated){
            dE_E_Calibrated[Si.AB()]->Fill(Cal_E,Cal_dE);
            dE_Etot_Calibrated[Si.AB()]->Fill(Cal_E+Cal_dE,Cal_dE);
            
            dEEi_cal[Si.AB()][Si.dE().Index()]->Fill(Cal_E,Cal_dE);
            EdEi_cal[Si.AB()][Si.E().Index()]->Fill(Cal_E,Cal_dE);
        }

    //////////////////// End of basic histograms
        
        
//////////////////// Do Corrections ASSUMING it is beam particle (without gating)
//////////////////// Energy correted ID gate can then be used BEFORE filling histograms

        double EnergyMidDeltaActive_Average=0;
        double dEdX_Average=0;
        double CalcEnergyEdet=0;   
        double CalcEnergyPostDeltaActive=0;
        double CalcEnergyMidDeltaActive=0;
        double CalcEnergyPreDeltaActive=0;
        double dECalc=0;
        double dEdXCalc=0;
        double EnergyMidDeltaActive=0;
        double dEmidcalc=0;     
        
        bool EventCal=((Cal_E>0.01)&&(Cal_dE>0.01));
    
        // Only do this section after initial calibration
        if(PreCalibrated){
            
    //////////////////// Determine  convoluted correction for He3 beam particle for deltaE detector /////

            // Range before E detector active region
            double PreRange=RangeSi_umMeV->Eval(Cal_E);  
            
            // Energy at 3 points (start,middle,end) of the active region of the dE, calulcated exclusively from the E detector energy
            CalcEnergyPostDeltaActive=EnergySi_MeVum->Eval(PreRange+(E_Dead_um+dE_BackDead_um)/EffThick); 
            CalcEnergyMidDeltaActive=EnergySi_MeVum->Eval(PreRange+(E_Dead_um+dE_Thickness_um*0.5)/EffThick); 
            CalcEnergyPreDeltaActive=EnergySi_MeVum->Eval(PreRange+(E_Dead_um+dE_Thickness_um-dE_FrontDead_um)/EffThick);
            
            // dE/dX "measurement" from the above range calculations
            dECalc=CalcEnergyPreDeltaActive-CalcEnergyPostDeltaActive;
            dEdXCalc=dECalc*EffThick;
            
            // Sum energy at midpoint using actual dE+E measurement (and deadlayer projection from E)
            EnergyMidDeltaActive=CalcEnergyPostDeltaActive+Cal_dE*0.5;
            
            // Taking and average because the dE calibration is a little wonky 
            // Should compare without this average once the calibration is better
            EnergyMidDeltaActive_Average=(CalcEnergyMidDeltaActive+EnergyMidDeltaActive)*0.5;
            dEdX_Average=(dEdXDirect+dEdXCalc)*0.5;
            
            if(EventCal){
                dEdX_Etot_SumCalibrated[Si.AB()]->Fill(EnergyMidDeltaActive,dEdXDirect);
                
                dEdX_Etot_SumCalibrated3He[Si.AB()]->Fill(EnergyMidDeltaActive_Average,dEdX_Average);
                
                dedxmid_dEi_cal[Si.AB()][Si.dE().Index()]->Fill(EnergyMidDeltaActive_Average,dEdX_Average);
                dedxmid_Ei_cal[Si.AB()][Si.E().Index()]->Fill(EnergyMidDeltaActive_Average,dEdX_Average);
            }
            
//////////////////// Do convoluted correction Assuming a He3 beam particle for E detector /////
    
            double dEActive_umEff= dEActive_um/EffThick;
                 
            double rstep=100; // Start with very large step size as quite computationally intensive
            // We should create a simple lookup table for the 256 pixels, it would be much faster
            for(int r=PreRange*0.7;r<E_Thickness_um;r+=rstep){
            
                double R=r;  // Guessed Range at the end of dE Active
                double R0=R+dEActive_umEff; //Range at the start of dE active
                
                double E1=EnergySi_MeVum->Eval(R); 
                double E2=EnergySi_MeVum->Eval(R0); 
                
                double Ecomp=E2-E1; // Energy that should have been lost if guess R is correct
            
                if(Ecomp<Cal_dE){ // Have surpassed the exact range that agrees with calibration Cal_dE
                
                    if(rstep>1){ //Circle back more accurately
                        r-=rstep;
                        rstep/=10;
                        r-=rstep;
                        continue;
                    }else{ // Break with the determined energy at precision 1um+-0.5
                        double RE=R-((E_Dead_um+dE_BackDead_um)/EffThick);
                        if(R>10){
                            CalcEnergyEdet=EnergySi_MeVum->Eval(RE-rr.Uniform()); //changed from -0.5 to -random due to fenceposting
                            dEmidcalc=EnergySi_MeVum->Eval(R+dEActive_umEff*0.5-rr.Uniform());
                        }
                        break;
                    }
                }
            }
        }//end PreCalibrated
            
        double KE_PostTarget=0;
        
        if(UseCalibratedGate)IsBeam=Calibrated_3HeGate[Si.AB()]->IsInside(EnergyMidDeltaActive_Average,dEdX_Average);
                    
        if(IsBeam){
            
            // First determine the calculated post-target energy, is it elastic on Uranium or Aluminium or Oxygen
            
            //if(Raw_dE_E_Charge_ElasticBeamGate[Si.AB()]->IsInside(Etot,dEdX)){
            if(PreCalibrated){
                if(EnergyMidDeltaActive_Average>ElasticCut){
                    // Gate elastic on americium
                    KE_PostTarget=KinematicElastic->Eval(ThetaRad);//Get energy of He3 from pre-solved kinematics
                }
            }else{
                if(Etot>ElasticCut){
                    // Gate elastic on americium
                    KE_PostTarget=KinematicElastic->Eval(ThetaRad);//Get energy of He3 from pre-solved kinematics
                }
            }
            
            // Gate on all beam and then energy range gate strip by strip to focus on oxygen elastics
    //         if(IsBeam&&ElasticAlHigh[Si.dE().Index()]>E&&ElasticAlLow[Si.dE().Index()]<E){  
            if(PreCalibrated){
//                 if(abs(ElasticAlCent[Si.AB()][Si.dE().Index()]-Cal_Etot)<0.35){  
                if(abs(ElasticAlCent[Si.AB()][Si.dE().Index()]-EnergyMidDeltaActive_Average)<0.35){  
                    KE_PostTarget=KinematicElasticAl->Eval(ThetaRad);//Get energy of He3 from pre-solved kinematics
                }
            }else{
                if(abs(ElasticAlCent[Si.AB()][Si.dE().Index()]-Etot)<50){  
                    KE_PostTarget=KinematicElasticAl->Eval(ThetaRad);//Get energy of He3 from pre-solved kinematics
                }
            }
                                         
            // IF we have determined an absolute incident particle energy from elastics
            // fill the calibration histograms
            if(KE_PostTarget>0){
                double SiRange=RangeSi_umMeV->Eval(KE_PostTarget);
                double Energy_Active_E=EnergySi_MeVum->Eval(SiRange-(dE_Thickness_um+E_Dead_um)/EffThick);
                
                double Energy_Active_dE=EnergySi_MeVum->Eval(SiRange-(dE_FrontDead_um/EffThick))-EnergySi_MeVum->Eval(SiRange-(dE_Thickness_um-dE_BackDead_um)/EffThick);

                CalElast_E[Si.AB()][Si.E().Index()]->Fill(E,Energy_Active_E);
                CalElast_dE[Si.AB()][Si.dE().Index()]->Fill(dE,Energy_Active_dE);
            }
            
//////////////////// Fill Histograms which depend on the convoluted corrections /////
    
            if(PreCalibrated){
                if(CalcEnergyEdet&&Etot<ElasticCut){
                    E_Check[Si.AB()]->Fill(Cal_E,CalcEnergyEdet); 
                    E_Check[Si.AB()]->Fill(CalcEnergyEdet,CalcEnergyEdet); 
                    
                    InvCal_E[Si.AB()][Si.dE().Index()]->Fill(E,CalcEnergyEdet);
    //                     InvCal_EComb[Si.AB()][Si.dE().Index()]->Fill(E,CalcEnergyEdet);
    //                     InvCal_EComb[Si.AB()][Si.dE().Index()]->Fill(E,Cal_E);
                    if(abs(CalcEnergyEdet-Cal_E)<0.2){
                        if(EventCal)InvCal_EComb[Si.AB()][Si.dE().Index()]->Fill(E,0.3*CalcEnergyEdet+0.7*Cal_E);
//                         InvCal_EComb[Si.AB()][Si.dE().Index()]->Fill(E,Cal_E);
                    }
                    
                    InvCal_ECompare[Si.AB()][Si.dE().Index()]->Fill(Cal_E,CalcEnergyEdet);
                }
                
                dEdX_Etot_SumCalibrated3HeAv[Si.AB()]->Fill((dEmidcalc+CalcEnergyMidDeltaActive+EnergyMidDeltaActive)/3.0,dEdX_Average);
                
                
                dE_Check[Si.AB()]->Fill(Cal_dE,dECalc); 
                dE_Check[Si.AB()]->Fill(dECalc,dECalc); 
                        
    //         if(Raw_dE_E_Charge_3HeGate[Si.AB()]->IsInside(Etot,dEdX)){// special gate that gets those which dont see elastic
                if(Etot<ElasticCut){
                    
                    InvCal_dE[Si.AB()][Si.dE().Index()]->Fill(dE,dECalc);
    //                     InvCal_dEComb[Si.AB()][Si.dE().Index()]->Fill(dE,dECalc);
    //                     InvCal_dEComb[Si.AB()][Si.dE().Index()]->Fill(dE,Cal_dE);
                    if(abs(dECalc-Cal_dE)<0.2){
                        if(EventCal)InvCal_dEComb[Si.AB()][Si.dE().Index()]->Fill(dE,0.5*(dECalc+Cal_dE));
//                         InvCal_dEComb[Si.AB()][Si.dE().Index()]->Fill(dE,0.3*dECalc+0.7*Cal_dE);
                    }
                    
                    InvCal_dECompare[Si.AB()][Si.dE().Index()]->Fill(Cal_dE,dECalc);
                }
                
            }//end PreCalibrated
                
            
        }//end IsBeam
        

        if(DoProtons&&PreCalibrated){
        
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
        
            if(dEdX_Etot_Charge_ProtonGate[Si.AB()]->IsInside(E,dE)){
                InvCal_dE_E_Proton[Si.AB()][Si.dE().Index()]->Fill(dE,ProtondECalc);
            }
            
        }//end DoProtons
        
          
        if(DoAlphas&&PreCalibrated){
        
            if(Raw_dE_E_Charge_4HeGate[Si.AB()]->IsInside(Etot,dEdX)){// special gate that gets those which dont see elastic
                
                double AlphaPreRange=AlphaSilicon_Range_umMeV->Eval(Cal_E);  
                double AlphaCalcEnergyPostDeltaActive=AlphaSilicon_Energy_MeVum->Eval(AlphaPreRange+(E_Dead_um+dE_BackDead_um)/EffThick); 
                double AlphaCalcEnergyPreDeltaActive=AlphaSilicon_Energy_MeVum->Eval(AlphaPreRange+(E_Dead_um+dE_Thickness_um-dE_FrontDead_um)/EffThick);
                double AlphadECalc=AlphaCalcEnergyPreDeltaActive-AlphaCalcEnergyPostDeltaActive;
                
                InvCal_dE4He[Si.AB()][Si.dE().Index()]->Fill(dE,AlphadECalc);
            }
        }//end DoAlphas
        
    }//End or TelescopeHit
    
    
    if(EndOfRun){    
        // End of last run
        if(jentry+1==nentries){

        }
    }
    

