  
cd SurrogateSort
make cal -j
cd ..

# -null 0 -Oxygen 0
# -null 0

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/AluminiumBase.root -null 0

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/CarbonBase.root -null 0 -Carbon 0

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxygenBase.root -null 0 -Oxygen 0 -Oxygen 0

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxySRIM.root -null 0 -Oxygen 0  -UseSRIM 0

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyBeamHigh.root -null 0 -Oxygen 0  -BeamEnergy 25

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyBeamLow.root -null 0 -Oxygen 0  -BeamEnergy 35

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyZOffset.root -null 0 -Oxygen 0  -ZOffset 10

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyNegZOffset.root -null 0 -Oxygen 0  -ZOffset -10

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyThickDead.root -null 0 -Oxygen 0  -dE_BackDead_um 10

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyThickdE.root -null 0 -Oxygen 0  -dE_Thickness_um 100

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyThindE.root -null 0 -Oxygen 0  -dE_Thickness_um 120

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyThickBacking.root -null 0 -Oxygen 0 -Backing_Thickness_um 4


# cd Outputs
# root -q processHistograms.C

./SurrogateSort/bin/SurrogateCal Outputs/AluminiumBase.root CalFiles/InitialCalFile.cal CalFiles/AluminiumBase.cal e
./SurrogateSort/bin/SurrogateCal Outputs/CarbonBase.root CalFiles/InitialCalFile.cal CalFiles/CarbonBase.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxygenBase.root CalFiles/InitialCalFile.cal CalFiles/OxygenBase.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxySRIM.root CalFiles/InitialCalFile.cal CalFiles/OxySRIM.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxyBeamHigh.root CalFiles/InitialCalFile.cal CalFiles/OxyBeamHigh.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxyBeamLow.root CalFiles/InitialCalFile.cal CalFiles/OxyBeamLow.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxyZOffset.root CalFiles/InitialCalFile.cal CalFiles/OxyZOffset.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxyNegZOffset.root CalFiles/InitialCalFile.cal CalFiles/OxyNegZOffset.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxyThickDead.root CalFiles/InitialCalFile.cal CalFiles/OxyThickDead.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxyThickdE.root CalFiles/InitialCalFile.cal CalFiles/OxyThickdE.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxyThindE.root CalFiles/InitialCalFile.cal CalFiles/OxyThindE.cal e
./SurrogateSort/bin/SurrogateCal Outputs/OxyThickBacking.root CalFiles/InitialCalFile.cal CalFiles/OxyThickBacking.cal e

SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/AluminiumBase.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/AluminiumBase.root -null 0
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/CarbonBase.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/CarbonBase.root -null 0 -Carbon 0
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxygenBase.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxygenBase.root -null 0 -Oxygen 0 -Oxygen 0
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxySRIM.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxySRIM.root -null 0 -Oxygen 0  -UseSRIM 0
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyBeamHigh.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyBeamHigh.root -null 0 -Oxygen 0  -BeamEnergy 25
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyBeamLow.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyBeamLow.root -null 0 -Oxygen 0  -BeamEnergy 35
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyZOffset.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyZOffset.root -null 0 -Oxygen 0  -ZOffset 10
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyNegZOffset.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyNegZOffset.root -null 0 -Oxygen 0  -ZOffset -10
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyThickDead.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyThickDead.root -null 0 -Oxygen 0  -dE_BackDead_um 10
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyThickdE.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyThickdE.root -null 0 -Oxygen 0  -dE_Thickness_um 100
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyThindE.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyThindE.root -null 0 -Oxygen 0  -dE_Thickness_um 120
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyThindE.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyThindE.root -null 0 -Oxygen 0  -dE_Thickness_um 120
SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyThickBacking.cal -ID Gates/Elastic_dEdX.root 1 -ID Gates/Beam.root 1 -ID Gates/Protons.root 0 -o Outputs/OxyThickBacking.root -null 0 -Oxygen 0 -Backing_Thickness_um 4

cd Outputs
root -q processHistograms.C
