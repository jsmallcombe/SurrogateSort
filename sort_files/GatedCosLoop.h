    
    
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
        
        TVector3 World=Si.GetPos(false);
        TVector3 Intrinsic=Si.GetPos(false,true);
        
        double EffThick=std::abs(cos(Intrinsic.Theta())); // Angle though
        double ThetaRad=World.Theta(); // Angle target
        
        double ThetaBlur=Si.GetPos(true).Theta();
        double Theta=World.Theta();
        double ThetaDeg=Theta/TMath::DegToRad();
        double CosTheta=cos(Theta);
        double AbsCosTheta=std::abs(CosTheta);
        double Phi=PhiOffset(World.Phi())/TMath::DegToRad();

        double E=Si.E().Energy();
        double dE=Si.dE().Energy();
        double Etot=E+dE;
        double dEdX=dE*EffThick;
        
        double ChargeTot=Si.E().Charge()+Si.dE().Charge();
        double ChargedX=Si.dE().Charge()*EffThick;
        
        
        
        for(unsigned int g=0;g<ParticleNames.size();g++){
//             if(!dedx_Gate[g]->IsInside(Etot,dEdX))continue;
            if(!dedx_Gate[g]->IsInside(ChargeTot,ChargedX))continue;
            
//             Gate_dE_dX[g]->Fill(Etot,dEdX);
            Gate_dE_dX[g]->Fill(ChargeTot,ChargedX);
            
            // Range before E detector active region
            double PreRange=Range_um[g][i_Si]->Eval(E);  
            
            // Energy at 3 points (start,middle,end) of the active region of the dE, calulcated exclusively from the E detector energy
            double CalcEnergyPostDeltaActive=Energy_MeV[g][i_Si]->Eval(PreRange+((E_Dead_um+dE_BackDead_um)/EffThick)); 
            double CalcEnergyMidDeltaActive=Energy_MeV[g][i_Si]->Eval(PreRange+((E_Dead_um+dE_Thickness_um*0.5)/EffThick)); 
            double CalcEnergyPreDeltaActive=Energy_MeV[g][i_Si]->Eval(PreRange+((E_Dead_um+dE_Thickness_um-dE_FrontDead_um)/EffThick));
        
            // dE/dX "measurement" from the above range calculations
            double dECalc=CalcEnergyPreDeltaActive-CalcEnergyPostDeltaActive;
            double dEdXCalc=dECalc*EffThick;
        
            // dE/dX from the actual dE detector energy
            double dEdXDirect=dE*EffThick;
        
            // Sum energy at midpoint using actual dE+E measurement (and deadlayer projection from E)
            double EnergyMidDeltaActive=CalcEnergyPostDeltaActive+dE*0.5;
        
            // Taking and average because the dE calibration is a little wonky 
            // Should compare without this average once the calibration is better
            double EnergyMidDeltaActive_Average=(CalcEnergyMidDeltaActive+EnergyMidDeltaActive)*0.5;
            double dEdX_Average=(dEdXDirect+dEdXCalc)*0.5;

            // //->Fill(EnergyMidDeltaActive_Average,dEdXDirect);
            
            dEdX_Etot_Corrected->Fill(EnergyMidDeltaActive_Average,dEdX_Average);
            
            // Uses an average of the measured dE and the dE value calculated from E measurement (as E calibration is better).
            double EnergyAtStartOfActiveDE=(CalcEnergyPreDeltaActive+(CalcEnergyPostDeltaActive+dE))*0.5;
            
            double RangeAtStartOfActiveDE=Range_um[g][i_Si]->Eval(EnergyAtStartOfActiveDE);  
            double EnergyAtStartOfDE=Energy_MeV[g][i_Si]->Eval(RangeAtStartOfActiveDE+(dE_FrontDead_um/EffThick));
            double RangeInBacking=Range_um[g][i_Al]->Eval(EnergyAtStartOfDE);
            
            double EnergyAtBackingCenter=Energy_MeV[g][i_Al]->Eval(RangeInBacking+((AlBacking_um*0.5)/AbsCosTheta));
            double EnergyBeforeBacking=Energy_MeV[g][i_Al]->Eval(RangeInBacking+(AlBacking_um/AbsCosTheta));
            double RangeInTarget=Range_um[g][i_Am0]->Eval(EnergyBeforeBacking);
            double EnergyAtCenter=Energy_MeV[g][i_Am0]->Eval(RangeInTarget+(AmHalf_um/AbsCosTheta));
        
            Gate_E_theta[g]->Fill(EnergyAtCenter,ThetaDeg);
            Gate_E_costheta[g]->Fill(EnergyAtCenter,CosTheta);
            Gate_E_thetaBlur[g]->Fill(EnergyAtCenter,ThetaBlur/TMath::DegToRad());
            Gate_E_thetaBlurBack[g]->Fill(EnergyAtBackingCenter,ThetaBlur/TMath::DegToRad());
            Gate_E_costhetaBlur[g]->Fill(EnergyAtCenter,cos(ThetaBlur));
            Gate_E_strad[g]->Fill(EnergyAtCenter);
            

        }
        
    }

    
    if(EndOfRun){
//         GammaTreeFile.Write(0,TObject::kWriteDelete);//Avoid those annoying multiple keys
        // End of last run
        if(jentry+1==nentries){
            TCanvas* KinCheck=new TCanvas("","",1920,1080);
            KinCheck->Divide(2,1);
            KinCheck->cd(1);
            gPad->Update();
            gPad->SetLogz();
            Gate_E_thetaBlur[0]->DrawCopy("col")->GetXaxis()->SetRangeUser(5,35);
            Am->SetLineWidth(2);
            Am->Draw("samel");
            O->SetLineWidth(2);
            O->Draw("samel");
            KinCheck->cd(2);
            gPad->Update();
            gPad->SetLogz();
            Gate_E_thetaBlurBack[0]->DrawCopy("col")->GetXaxis()->SetRangeUser(5,35);
            Al->SetLineWidth(2);
            Al->Draw("samel");
            
            // 	gStyle->SetLineScalePS(1.5); // default is 3, which looks crap
            gStyle->SetLineScalePS(2); // default is 3, which looks crap
            // 	Can->Write("Efficiency.pdf");
            // 	KinCheck->SaveAs(FileBaseName+"KinCheck.eps");
            // 	KinCheck->SaveAs(FileBaseName+"KinCheck.pdf");
            KinCheck->SaveAs(FileBaseName+"KinCheck.png"); 
            
            out.cd();
                KinCheck->Write("KinCheckCan");
            gROOT->cd();
    
    
//             cout<<endl<<"Scaling by hardcoded solid angle"<<endl;
//             for(auto g : Gate_E_strad){
//                 g->Scale(1/1.52333);
//             }
        }
    }

