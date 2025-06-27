cd SurrogateSort
make -j SPLINE=0
cd ..

source SurrogateSort/bin/thisSSlib.sh

#####  Set the inital cal file with correct channel assignments 
CalFile=CalFiles/RawCal.cal

#####  Sort a little data to look at timing
SurrogateSort/bin/RawSiliconSort $CalFile /Data/**.root -PartSort 0 -o Outputs/FirstSort.root -TelescopeBuildWindow 1000

#####  From histograms   dE_dT_A   dE_dT_B   E_dT_A   E_dT_B  
#####  Manually align detector times in CalFile #####  

#####  From histogram  Raw/Charge_dEdX_EtotA Raw/Charge_dEdX_EtotB
#####  Save the charge Etot - dedx particle gates for beamlike Gates/Gate3He.root  (also define tigher A/B gates)

#####  Filter To Only The Silicon Events with 1 good event
SurrogateSort/bin/RawSiliconSort $CalFile /Data/**.root -ID Gates/Gate3He.root 0 -OverwriteFilterFile 0 -o out.root -TelescopeBuildWindow 150
out_FilterTree.root Outputs/BeamFiltered.root

echo $CalFile > infofile.info
echo "-ID Gates/Gate3HeA.root 1" >> infofile.info
echo "-ID Gates/Gate3HeB.root 2" >> infofile.info
echo "Outputs/BeamFiltered.root" >> infofile.info

#####  From histogram  Raw/Charge_dEdX_EtotA Raw/Charge_dEdX_EtotB
#####  Determine the charge cut for the beam/target elastic peak
echo "-ElasticCut 4900" >> infofile.info

#####  From histograms Raw/dE_Chan/dEdXTotA_i 
#####  Determine the charge centroid per dE detector for elastic oxygen/aluminium points, 0 if none
echo "-Array 0 16 000 5190 5170 5145 5105 5055 5010 4975 4915 4820 4770 4700 4620 4550 000 000" >> infofile.info
echo "-Array 1 16 5170 5190 5180 5140 5105 5050 5000 4960 4900 4800 4740 4680 4610 4510 4450 000" >> infofile.info
echo "-Oxygen 0" >> infofile.info

#####  Cal Step 1
#####  Do the sort which produces calibration histograms solving for gated elastics
SurrogateSort/bin/CalSort infofile.info -o Outputs/InitialCalSort.root

#####  Copy cal file as the next step overwrites
cp CalFiles/RawCal.cal CalFiles/FirstCal.cal

#####  Cal Step 2
#####  Perform the autofits and make the calibration
#####  Option "View" to check the fits make sense
#####  Option "Zero" forces the calibration to go through zero (actually not suitable for the current system)
#####  Option "Linear" use only a pol1, recommended
#####  Option "Elastic" use the histograms filled only with the data selected with the cuts REQUIRED FOR INITIAL CALIBRATION
#####  Option "Normalise" weight all point equally irrespective of fit error
#####  Option "Gaus" use a gausain fit rather than TProfile statistics
#####  Option "ViewGaus" to make sure the charge cuts are giving sensible data which fits well
#####  Option "Prune" Automatically remove the worst points
SurrogateSort/bin/SurrogateCal CalFiles/FirstCal.cal Outputs/InitialCalSort.root Elastic Normalise Gaus Prune View

echo "CalFiles/FirstCal.cal" >> infofile.info

#####  Cal Step 3
#####  Review the calibration and produce the dE/E inverted calculations
SurrogateSort/bin/CalSort -PreCalibrated 0 -o Outputs/PostInitialCalibration.root 

#####  If -PreCalibrated is set, elastic cuts will be set to energy rather than charge
#####  But histograms using energy will be created

#####  The Particle gate can OPTIONALLY be set to use the energy from histograms Calibrared/dEdX_E_at_dE_SumCalibrated3He_A
echo "-ID Gates/Gate3HeEnergyA.root 6" >> infofile.info
echo "-ID Gates/Gate3HeEnergyB.root 7" >> infofile.info

#####  Cal Step 4
#####  Review the calibration shown in the fully corrected E vs theta plots
SurrogateSort/bin/GatedCosSort infofile.info -o Outputs/CalibratedKinematics.root 


#####  Cal Step 5
#####  Make the calibration based on inverse data, i.e. calibrate dE based on E calibration
#####  Option "Comb" Use energy determined from previous calibration and other detector (E or dE)
#####  Option "DeltaOnly" Only update delta-E calibration
cp CalFiles/FirstCal.cal CalFiles/InverseCal.cal
SurrogateSort/bin/SurrogateCal CalFiles/InverseCal.cal Outputs/PostInitialCalibration.root Normalise Linear Delta
echo "CalFiles/InverseCal.cal" >> infofile.info
SurrogateSort/bin/CalSort -PreCalibrated 0 -o Outputs/PostInverseCalibration.root 



#####  For the calibration Steps 1-4 multiple options to fine tune the geometry can be used
#####  The same options must be repeated for Steps 1,3,4 
#####  Options such as TargetZOffset, BeamEnergy, dE_Thickness_um, AllDead_um, Backing_Thickness_um

