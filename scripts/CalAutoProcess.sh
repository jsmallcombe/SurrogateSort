  
cd SurrogateSort
make cal -j
make QuickCal -j
cd ..

./SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/InitialCalFile.cal -ID Gates/Elastic_dEdX.root 0 -ID Gates/Beam.root 1 -ID Gates/Protons.root 3 -ID Gates/ENDdedxtotcharge4HeA.root 4 -ID Gates/ENDdedxtotcharge4HeB.root 5 -ID Gates/ENDdedxtotcharge3HeA.root 6 -ID Gates/ENDdedxtotcharge3HeB.root 7 -o Outputs/OxyThin_dE106.root -null 0 -Oxygen 0  -dE_Thickness_um 106

./SurrogateSort/bin/SurrogateCal Outputs/OxyThin_dE106.root CalFiles/InitialCalFile.cal CalFiles/OxyThindE06.cal e

./SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyThindE06.cal -ID Gates/Elastic_dEdX.root 0 -ID Gates/Beam.root 1 -ID Gates/Protons.root 3 -ID Gates/ENDdedxtotcharge4HeA.root 4 -ID Gates/ENDdedxtotcharge4HeB.root 5 -ID Gates/ENDdedxtotcharge3HeA.root 6 -ID Gates/ENDdedxtotcharge3HeB.root 7 -o Outputs/OxyThin_dE106.root -null 0 -Oxygen 0  -dE_Thickness_um 106

./SurrogateSort/bin/SurrogateCal Outputs/OxyThin_dE106.root CalFiles/OxyThindE06.cal CalFiles/OxyThindE06Fin.cal

./SurrogateSort/bin/SurrogateSort Outputs/FilterSiliconTree.root CalFiles/OxyThindE06Fin.cal -ID Gates/Elastic_dEdX.root 0 -ID Gates/Beam.root 1 -ID Gates/Protons.root 3 -ID Gates/ENDdedxtotcharge4HeA.root 4 -ID Gates/ENDdedxtotcharge4HeB.root 5 -ID Gates/ENDdedxtotcharge3HeA.root 6 -ID Gates/ENDdedxtotcharge3HeB.root 7 -o Outputs/OxyThin_dE106.root -null 0 -Oxygen 0  -dE_Thickness_um 106
