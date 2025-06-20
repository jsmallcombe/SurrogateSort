{
	// source bin/thisSSlib.sh
	// root 
	// gSystem->Load("libSurrogateSort.so"); 
	// .x Examples/CalEditExample.C
	
	ReadCal("InitialCalFile.cal");

	DetHit::SetCalibrationDirect(DetHit::SiDeltaE,0,0,0,0);
	DetHit::SetCalibrationDirect(DetHit::SiDeltaE,7,0,0,0);
	DetHit::SetCalibrationDirect(DetHit::SiDeltaE,13,0,0,0);
	DetHit::SetCalibrationDirect(DetHit::Si,0,0,0,0);
	DetHit::SetCalibrationDirect(DetHit::Si,15,0,0,0);
	DetHit::SetCalibrationDirect(DetHit::SiDeltaE_B,14,0,0,0);
	DetHit::SetCalibrationDirect(DetHit::SiDeltaE_B,15,0,0,0);

	WriteCal("EditedCalFile.cal");
}
