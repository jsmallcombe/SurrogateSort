#include "include/detectors.h"
#include "include/IO.h"

void ListAngle(){
    
    gSystem->Load("bin/libSurrogateSort.so");
    
    ReadCal("../CalFiles/InitialCalJan.cal");
    gROOT->cd();
    
 
    std::cout << "Mod, Channel_dE, Channel_E, theta, phi " << std::endl;
    for(int m=0;m<4;m++){
        
        
        for(int i=0;i<32;i++){
            DetHit hit(1,1,m,i);
            if(!((hit.Type()==DetHit::SiDeltaE)||(hit.Type()==DetHit::SiDeltaE_B)))continue;
            
            for(int j=0;j<32;j++){
                DetHit hitB(1,1,m,j);
                if(!((hitB.Type()==DetHit::Si)||(hitB.Type()==DetHit::Si_B)))continue;
                
                TVector3 World=TelescopeHit(hit,hitB).GetPos(false);
					
                std::cout << m << ", "<< i << ", "<< j << ", "<< World.Theta() << ", " <<World.Phi()<< std::endl;
                
            }  
        }
    }
    

}
