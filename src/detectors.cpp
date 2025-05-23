#include <detectors.h>

vector<vector<ChannelCalibration>> DetHit::ChanCal;

void DetHit::ExpandCal(UShort_t Mod,UShort_t Chan){
    // Ensure the outer vector has enough elements
    while (ChanCal.size() <= Mod) {
        ChanCal.push_back(std::vector<ChannelCalibration>());
    }
    // Ensure the inner vector has enough elements
    while (ChanCal[Mod].size() <= Chan) {
        ChanCal[Mod].push_back(ChannelCalibration());
    }
}

void DetHit::SetCalibrationParam(UShort_t Mod,UShort_t Chan,double Pol0, double Pol1,double Pol2){
    ExpandCal(Mod,Chan); 
    ChanCal[Mod][Chan].p0=Pol0;
    ChanCal[Mod][Chan].p1=Pol1;
    ChanCal[Mod][Chan].p2=Pol2;
}

std::array<double, 3> DetHit::GetCal(UShort_t Mod, UShort_t Chan) {
    ExpandCal(Mod,Chan); 
    return {ChanCal[Mod][Chan].p0,ChanCal[Mod][Chan].p1,ChanCal[Mod][Chan].p2};
}

void DetHit::SetCalibrationDirect(UShort_t DetT,UShort_t Index,double Pol0, double Pol1,double Pol2){
    
    for(auto& V : ChanCal){
        for(auto& CC : V){
            if(CC.DetectorType==DetT&&CC.Index==Index){
                CC.p0=Pol0;
                CC.p1=Pol1;
                CC.p2=Pol2;
                return;
            }
        }
    }
    
}


UShort_t DetHit::GetDetType(UShort_t Mod,UShort_t Chan){
    ExpandCal(Mod,Chan); 
    return ChanCal[Mod][Chan].DetectorType;
}

void DetHit::SetDetType(UShort_t Mod,UShort_t Chan,UShort_t DetT){
    ExpandCal(Mod,Chan); 
    ChanCal[Mod][Chan].DetectorType=DetT; 
}

UShort_t DetHit::GetIndex(UShort_t Mod,UShort_t Chan){
    ExpandCal(Mod,Chan); 
    return ChanCal[Mod][Chan].Index;
}

void DetHit::SetIndex(UShort_t Mod,UShort_t Chan,UShort_t Index){
    ExpandCal(Mod,Chan); 
    ChanCal[Mod][Chan].Index=Index; 
}

Double_t DetHit::GetTOff(UShort_t Mod,UShort_t Chan){
    ExpandCal(Mod,Chan); 
    return ChanCal[Mod][Chan].TOff;
}

void DetHit::SetTOff(UShort_t Mod,UShort_t Chan,Double_t toff){
    ExpandCal(Mod,Chan); 
    ChanCal[Mod][Chan].TOff=toff; 
}


////////////////////////////



TRandom3 DetHit::randGen;
double DetHit::Energy(){
    
    if(fEnergySet)return fEnergy;
    
    array<double, 3> Cal=GetCal(fModule,fChannel);
    
    fEnergy=(fCharge+randGen.Uniform());
    fEnergy=Cal[0]+fEnergy*Cal[1]+fEnergy*fEnergy*Cal[2];
    fEnergySet=true;
    return fEnergy;
}


////////////////////////////

double TelescopeHit::ActiveLength = 49.50;
int TelescopeHit::segN = 16;

// Distance in the XZ plane between the face of detector and origin
double TelescopeHit::MagXZnorm_dE = 39.776;
double TelescopeHit::MagXZnorm_E = 46.006;

// Distance between the origin facing PCB face and center/front of the silicon wafer
double TelescopeHit::FaceOffset_dE = 2.2 - 0.05;
double TelescopeHit::FaceOffset_E = 2.2 - 0.5;

// Distance along the detector face between the face center and origin normal vector, along the XZ plane
// As the face normal vector origin is NOT in the center
double TelescopeHit::FaceOriginOffsetXZ = -10.227;

// Calculate the basic parameters
double TelescopeHit::dStrip = TelescopeHit::ActiveLength / TelescopeHit::segN;
double TelescopeHit::XZ_dE = TelescopeHit::MagXZnorm_dE + TelescopeHit::FaceOffset_dE;
double TelescopeHit::XZ_E = TelescopeHit::MagXZnorm_E + TelescopeHit::FaceOffset_E;
double TelescopeHit::OffsetY = (TelescopeHit::ActiveLength * 0.5) - 0.5 * TelescopeHit::dStrip;
double TelescopeHit::OffsetXZ = TelescopeHit::OffsetY - TelescopeHit::FaceOriginOffsetXZ;

// The default assumption is the XZ strip 0 is nearest the beam axis and Y strip 0 is nearest floor
bool TelescopeHit::Invert_dEA = false;
bool TelescopeHit::Invert_EA = false;
bool TelescopeHit::Invert_dEB = false;
bool TelescopeHit::Invert_EB = false;
// Can be set with correct channel numbers

bool TelescopeHit::Z_Mirror = false;

int TelescopeHit::AB(const DetHit& hit){
    if(hit.Type()==DetHit::SiDeltaE_B||hit.Type()==DetHit::Si_B)return 1;
    else return 0;
}


TVector3 TelescopeHit::SiliconDetectorPos(UShort_t N_dE,UShort_t N_E,bool Inv_dE,bool Inv_E,bool blur,bool intrinsic){

    // Flip the segment axis if needed
    if(Inv_dE){
        N_dE=segN-1-N_dE;
    }
    if(Inv_E){
        N_E=segN-1-N_E;
    }

    // i is ZX plane positioning, j is Y positioning
    // dE detectors are divided along the XZ plane
    // E detectors are divided along the Y axis plane
    int i=N_dE;     int j=N_E;    double ZPrime=XZ_dE; double YScale=XZ_dE/XZ_E;
//     // Opposite config :
// //     int j=N_dE;int i=N_E;double ZPrime=XZ_E;double YScale=XZ_E/XZ_dE;

    if(i>=segN||j>=segN){return TVector3(0,0,0);}

    // Calcualte the position on the detector in the the frame where the detector normal is aligned with the Z-axis
    double Xprime=(i*dStrip)-OffsetXZ;
    double Yprime=(j*dStrip)-OffsetY;

    // A function I always include to make visulisation easier when testing.
    if(blur){
        Xprime+=DetHit::randGen.Uniform(-dStrip*0.45,dStrip*0.45);
        Yprime+=DetHit::randGen.Uniform(-dStrip*0.45,dStrip*0.45);
    }

    // Create the vector in the prime frame
    TVector3 RetVec(Xprime,Yprime*YScale,-ZPrime);

    if(!intrinsic){
        //Rotate from the prime frame into the lab-frame
        RetVec.RotateY(TMath::DegToRad()*-55);
    }

    if(Z_Mirror) RetVec=TVector3(RetVec.X(),RetVec.Y(),-RetVec.Z());
    return RetVec;
}


TVector3 TelescopeHit::SiliconPosDet(UShort_t N_dE,UShort_t N_E,int AB,bool blur,bool intrinsic){
    if(AB==0){
        return SiliconDetectorPos(N_dE,N_E,Invert_dEA,Invert_EA,blur,intrinsic);
    }else{
        TVector3 RetVec=SiliconDetectorPos(N_dE,N_E,Invert_dEB,Invert_EB,blur,intrinsic);
        RetVec.SetX(-RetVec.X());
        return RetVec;
    }
}
