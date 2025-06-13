cd SurrogateSort
make -j
cd ..

source SurrogateSort/bin/thisSSlib.sh

#####  Set the inital cal file with correct channel assignments 
CalFile=CalFiles/InitialCalJan.cal

#####  Sort a little data to look at timing
# SurrogateSort/bin/RawSiliconSort $CalFile /Data/SurrogateExptDataJan2025/Tout01/**.root -PartSort 0 -o Outputs/

#####  Manually align detector times in CalFile

#####  Filter To Only The Silicon Events with 1 good event
# RUNS="/Data/SurrogateExptDataJan2025/Tout01/Run**/*.root"
# for f in $RUNS
# do
# 	  r=${f:${#f}-20}
#     SurrogateSort/bin/RawSiliconSort $CalFile $f -OverwriteFilterFile 0 -o out.root
#     mv out.root  Outputs$r
#     mv out_FilterTree.root Outputs/Filter$r
# done

#####  Define the charge Etot - dedx particle gates in file Gates/Gate3He.root

#####  Filter To Only the 3He elastic and Inelastic events
#     SurrogateSort/bin/RawSiliconSort $CalFile -ID Gates/Gate3He.root 0 $f -OverwriteFilterFile 0 -o Outputs/Filtered.root
#     mv Outputs/Filtered.root Outputs/Filter/Filtered3HeOnly

#####  Set the per-dE elastic points currently in CalLoop.h

#####  Do the sort which produces calibration histograms solving for gated elastics
#####  Cal Step 1
# SurrogateSort/bin/CalSort $CalFile -Oxygen 0 -ID Gates/Gate3He.root 1 Outputs/Run3_FilterTree.root -o Outputs/CalSortRun3.root

#####  Copy cal file as it overwrites in the next step
CalTwo=${CalFile::-4}_cp.cal
cp $CalFile $CalTwo

#####  Perform the autofits and make the calibration
#####  Option "Zero" forces the calibration to go through zero
#####  Add option "View" to check the fits make sense.
#####  Cal Step 2
# SurrogateSort/bin/SurrogateCal $CalTwo test.root Elastic Zero 


#####  Review the calibration and produce the dE/E inverted calculations
#####  Cal Step 3
# SurrogateSort/bin/CalSort $CalTwo -Oxygen 0 -ID Gates/Gate3He.root 1 Outputs/Run3_FilterTree.root -o Outputs/PostCalibration.root -PreCalibrated 0


#####  Review the calibration shown in the fully corrected E vs theta plots
#####  Cal Step 4
SurrogateSort/bin/GatedCosSort $CalTwo -ID Gates/Gate3He.root 1 Outputs/Run3_FilterTree.root -o Outputs/FinalKinematics.root 


#####  For the calibration Steps 1-4 multiple options to fine tune the geometry can be used
#####  The same options must be repeated for Steps 1,3,4 
#####  Options such as TargetZOffset, BeamEnergy, dE_Thickness_um, AllDead_um, Backing_Thickness_um


