        std::fill(GateMult.begin(), GateMult.end(), 0);
        
        NN->Fill(tAdc->size(),tNum);

        for(UShort_t i=0;i<Nhit;i++){  // Loop over hits from event vector 
			
            int Mod_i=(*tMod)[i];
            int Chan_i=(*tCh)[i];
            double Time_i=(*tTs)[i];
            double dE_i=(*tAdc)[i];
            ChanE->Fill(ModChanList[Mod_i]+Chan_i,dE_i);
            ChanM->Fill(ModChanList[Mod_i]+Chan_i,tNum);
            
            GateMult[ModChanList[Mod_i]+Chan_i]++;
            
            for(UShort_t j=i+1;j<Nhit;j++){  // Loop over remaining hits from event vector
                
                        
                int Mod_j=(*tMod)[j];
                int Chan_j=(*tCh)[j];
                double Time_j=(*tTs)[j];
                double dE_j=(*tAdc)[j];

                
                ChanCo->Fill(ModChanList[Mod_i]+Chan_i,ModChanList[Mod_j]+Chan_j);
                ChanCo->Fill(ModChanList[Mod_j]+Chan_j,ModChanList[Mod_i]+Chan_i);
                
                if(abs(Time_j-Time_i)<100){
                    
                    
                    GateMult[ModChanList[Mod_i]+Chan_i]++;
                    GateMult[ModChanList[Mod_j]+Chan_j]++;
                    
                    ChanCoGate->Fill(ModChanList[Mod_i]+Chan_i,ModChanList[Mod_j]+Chan_j);
                    ChanCoGate->Fill(ModChanList[Mod_j]+Chan_j,ModChanList[Mod_i]+Chan_i);
                    
                    if(Mod_j==Mod_i&&Chan_i==Chan_j){
                        
                        if(Mod_j==2){
                            
                            if(Chan_i>15){
                                SelfEnergydE[0][Chan_i-16]->Fill(dE_i,dE_j);
                                SelfEnergydE[0][Chan_i-16]->Fill(dE_j,dE_i);
                                SelfTimedE[0]->Fill(Chan_i-16,Time_j-Time_i);
                            }else{
                                SelfEnergyE[0][Chan_i]->Fill(dE_i,dE_j);
                                SelfEnergyE[0][Chan_i]->Fill(dE_j,dE_i);
                                
                                SelfTimeE[0]->Fill(Chan_i,Time_j-Time_i);
                                
                            }
                        }
                        if(Mod_j==3){
                            if(Chan_i>15){
                                SelfEnergyE[1][Chan_i-16]->Fill(dE_i,dE_j);
                                SelfEnergyE[1][Chan_i-16]->Fill(dE_j,dE_i);
                                SelfTimeE[1]->Fill(Chan_i-16,Time_j-Time_i);
                            }else{
                                SelfEnergydE[1][Chan_i]->Fill(dE_i,dE_j);
                                SelfEnergydE[1][Chan_i]->Fill(dE_j,dE_i);
                                SelfTimedE[1]->Fill(Chan_i,Time_j-Time_i);
                            }
                        }
                    }
                }
                
                if(abs(Time_j-Time_i)>=100&&abs(Time_j-Time_i)<200){
                    ChanCoGateAnti->Fill(ModChanList[Mod_i]+Chan_i,ModChanList[Mod_j]+Chan_j);
                    ChanCoGateAnti->Fill(ModChanList[Mod_j]+Chan_j,ModChanList[Mod_i]+Chan_i);
                }
                
            }
		}
		
		for(int i=0;i<totalchans;i++){
            if(GateMult[i]>0)ChanMgate->Fill(i,GateMult[i]);
        }
