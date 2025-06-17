// //  The splines are slightly faster and more accurate, but causes edge case misbehaviour
// #define NOSPLINES

    double target_thickness_ug_cm2=158.2;//Jan2025
    double target_density_g_cm3=11.68; //AmO2
    double target_um=1E-2*target_thickness_ug_cm2/target_density_g_cm3;
    double AmHalf_um=target_um/2;
    
    double beam_energy_MeV=30; 
    if(Inputs.TestInput("BeamEnergy")){
        beam_energy_MeV=Inputs.GetInput("BeamEnergy");
    }
    
    double AlBacking_um=1.99;
    if(Inputs.TestInput("Backing_Thickness_um")){
        AlBacking_um=Inputs.GetInput("Backing_Thickness_um");
    }
    double AlBacking_half_um=AlBacking_um*0.5;
    
    double E_Thickness_um=1000; 
    
    double dE_Thickness_um=111; 
    if(Inputs.TestInput("dE_Thickness_um")){
        dE_Thickness_um=Inputs.GetInput("dE_Thickness_um");
    }
    
    double dE_FrontDead_um=0.5065;
    double dE_BackDead_um=dE_FrontDead_um;
    if(Inputs.TestInput("dE_BackDead_um")){
        dE_BackDead_um=Inputs.GetInput("dE_BackDead_um");
    }
    
    double E_Dead_um=0.545;

    if(Inputs.TestInput("AllDead_um")){
        dE_FrontDead_um=Inputs.GetInput("AllDead_um");
        dE_BackDead_um=dE_FrontDead_um;
        E_Dead_um=dE_FrontDead_um;
    }   
    
   double dEActive_um=dE_Thickness_um-dE_FrontDead_um-dE_BackDead_um;
    
   out.mkdir("Ranges");   
   gROOT->cd();
   
   TGraph *Range_umMeV,*Energy_MeVum;
   
  // Protons in Silicon
  Double_t H1SiEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			     1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			     3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			     7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			     17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t H1SiRanmum[51] = {5.97,  6.83,  7.72,  8.66,  9.64,  11.70, 13.92, 16.28, 18.77, 21.41,
			     24.20, 27.13, 30.19, 33.39, 36.73, 40.20, 47.53, 57.42, 68.09, 79.54,
			     91.74, 104.68,118.36,132.76,147.87,180.17,215.21,252.95,293.33,336.32,
			     381.86,480.39,588.80,706.85,834.36,971.15,1120., 1270., 1440., 1610.,
			     1790., 1980., 2380., 2940., 3540., 4190., 4890., 5640., 6480., 7270.,8160.};
  TGraph *H1_SRIM_RangeSilicon_umMeV = new TGraph(51,H1SiEneMeV,H1SiRanmum);

#ifdef NOSPLINES
   TGraph *ProtonsSilicon_Range_umMeV = new TGraph(51,H1SiEneMeV,H1SiRanmum);
   TGraph *ProtonsSilicon_Energy_MeVum = new TGraph(51,H1SiRanmum,H1SiEneMeV);  
#else
   Range_umMeV = new TGraph(51,H1SiEneMeV,H1SiRanmum);
   Energy_MeVum = new TGraph(51,H1SiRanmum,H1SiEneMeV);  
   TSpline3 *ProtonsSilicon_Range_umMeV = new TSpline3("ProtonsSilicon_Range_umMeV",Range_umMeV);  
   TSpline3 *ProtonsSilicon_Energy_MeVum = new TSpline3("ProtonsSilicon_Energy_MeVum",Energy_MeVum);  
   out.cd("Ranges");
      Range_umMeV->Write("ProtonsSilicon_Range_umMeV");
      Energy_MeVum->Write("ProtonsSilicon_Energy_MeVum");
   gROOT->cd();
#endif  
  // Deuterons in Si
  Double_t H2SiEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			     1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			     3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			     7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			     17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t H2SiRanmum[51] = {4.86,  5.47,  6.10,  6.76,  7.45,  8.89,  10.43, 12.06, 13.77, 15.57,
			     17.45, 19.42, 21.45, 23.57, 25.76, 28.02, 32.75, 39.06, 45.81, 53.02,
			     60.67, 68.74, 77.24, 86.14, 95.46, 115.30,136.71,159.67,184.15,210.11,
			     237.55,296.68,361.46,431.75,507.43,588.41,674.61,765.94,862.33,963.72,
			     1070., 1180., 1420., 1740., 2090., 2470., 2880., 3310., 3780., 4270.,4780.};
  TGraph *H2_SRIM_RangeSilicon_umMeV = new TGraph(51,H2SiEneMeV,H2SiRanmum);

  // Tritons in Si
  Double_t H3SiEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			     1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			     3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			     7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			     17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t H3SiRanmum[51] = {4.59,  5.10,  5.63,  6.19,  6.76,  7.95,  9.22,  10.55, 11.95, 13.41,
			     14.94, 16.53, 18.17, 19.88, 21.64, 23.46, 27.25, 32.29, 37.65, 43.32,
			     49.28, 55.52, 62.06, 68.91, 76.06, 91.24, 107.55,124.99,143.53,163.14,
			     183.82,228.27,276.80,329.31,385.73,445.98,509.98,577.70,649.06,724.02,
			     802.55,884.59,1060., 1300., 1550., 1830., 2130., 2450., 2790., 3140.,3520.};
  TGraph *H3_SRIM_RangeSilicon_umMeV = new TGraph(51,H3SiEneMeV,H3SiRanmum);


  // He-3 in Si
  Double_t He3SiEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			      1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			      3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			      7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			      17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t He3SiRanmum[51] = {1.83,  1.98,  2.13,  2.29,  2.44,  2.77,  3.11,  3.46,  3.82,  4.20,
			      4.59,  4.99,  5.41,  5.84,  6.28,  6.74,  7.69,  8.96,  10.30, 11.73,
			      13.22, 14.80, 16.44, 18.16, 19.95, 23.73, 27.78, 32.09, 36.66, 41.48,
			      46.56, 57.52, 69.52, 82.51, 96.47, 111.39,127.24,144.02,161.71,180.29,
			      199.76,220.11,263.38,322.23,386.29,455.45,529.62,608.73,692.69,781.45,874.94};
  TGraph *He3_SRIM_RangeSilicon_umMeV = new TGraph(51,He3SiEneMeV,He3SiRanmum);
 
#ifdef NOSPLINES
   TGraph *RangeSi_umMeV = new TGraph(51,He3SiEneMeV,He3SiRanmum);
   TGraph *EnergySi_MeVum = new TGraph(51,He3SiRanmum,He3SiEneMeV);
#else
   Range_umMeV = new TGraph(51,He3SiEneMeV,He3SiRanmum);
   Energy_MeVum = new TGraph(51,He3SiRanmum,He3SiEneMeV);  
   TSpline3 *RangeSi_umMeV = new TSpline3("RangeSi_umMeV",Range_umMeV);  
   TSpline3 *EnergySi_MeVum = new TSpline3("EnergySi_MeVum",Energy_MeVum);  
   
   out.cd("Ranges");
      Range_umMeV->Write("GRangeSi_umMeV");
      Energy_MeVum->Write("GEnergySi_MeVum");
   gROOT->cd();
#endif   

  // He-4 in Si
  Double_t He4SiEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			      1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			      3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			      7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			      17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t He4SiRanmum[51] = {1.97,  2.12,  2.27,  2.41,  2.56,  2.87,  3.18,  3.50,  3.83,  4.17,
			      4.52,  4.88,  5.25,  5.63,  6.02,  6.42,  7.25,  8.34,  9.50,  10.72,
			      12.00, 13.34, 14.73, 16.19, 17.70, 20.89, 24.30, 27.92, 31.75, 35.78,
			      40.01, 49.05, 58.85, 69.46, 80.88, 93.05, 105.97,119.62,134.00,149.10,
			      164.90,181.39,216.43,264.02,315.74,371.52,431.29,494.97,562.50,633.85,708.95};
  TGraph *He4_SRIM_RangeSilicon_umMeV = new TGraph(51,He4SiEneMeV,He4SiRanmum);
 
#ifdef NOSPLINES
   TGraph *AlphaSilicon_Range_umMeV = new TGraph(51,He4SiEneMeV,He4SiRanmum);
   TGraph *AlphaSilicon_Energy_MeVum = new TGraph(51,He4SiRanmum,He4SiEneMeV); 
#else
   Range_umMeV = new TGraph(51,He4SiEneMeV,He4SiRanmum);
   Energy_MeVum = new TGraph(51,He4SiRanmum,He4SiEneMeV);  
   TSpline3 *AlphaSilicon_Range_umMeV = new TSpline3("AlphaSilicon_Range_umMeV",Range_umMeV);  
   TSpline3 *AlphaSilicon_Energy_MeVum = new TSpline3("AlphaSilicon_Energy_MeVum",Energy_MeVum);    
   out.cd("Ranges");
      Range_umMeV->Write("AlphaSilicon_Range_umMeV");
      Energy_MeVum->Write("AlphaSilicon_Energy_MeVum");
   gROOT->cd();
#endif   
   

  // Protons in Aluminium
  Double_t H1AlEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			     1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			     3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			     7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			     17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t H1AlRanmum[51] = {5.42,  6.17,  6.96,  7.78,  8.63,  10.43, 12.35, 14.40, 16.56, 18.86,
			     21.28, 23.84, 26.51, 29.31, 32.23, 35.26, 41.68, 50.35, 59.71, 69.76,
			     80.48, 91.87, 103.90,116.57,129.87,158.33,189.22,222.50,258.13,296.07,
			     336.28,423.32,519.12,623.50,736.29,857.33,986.50,1120., 1270., 1420.,
			     1580., 1750., 2110., 2600., 3130., 3710., 4330., 5000., 5700., 6450.,7230.};
  TGraph *H1_SRIM_RangeAlBacking_umMeV = new TGraph(51,H1AlEneMeV,H1AlRanmum);

  // Deuterons in Aluminium
  Double_t H2AlEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			     1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			     3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			     7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			     17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t H2AlRanmum[51] = {4.57,  5.11,  5.68,  6.27,  6.88,  8.16,  9.52,  10.95, 12.46, 14.03,
			     15.68, 17.39, 19.16, 21.00, 22.91, 24.87, 28.97, 34.44, 40.32, 46.60,
			     53.27, 60.32, 67.75, 75.54, 83.70, 101.08,119.86,140.01,161.52,184.34,
			     208.47,260.51,317.56,379.49,446.22,517.65,593.71,674.33,759.44,848.98,
			     942.91,1040., 1250., 1540.,1850., 2180., 2540., 2930., 3340., 3770., 4230., };
  TGraph *H2_SRIM_RangeAlBacking_umMeV = new TGraph(51,H2AlEneMeV,H2AlRanmum);

  // Tritons in Aluminium
  Double_t H3AlEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			     1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			     3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			     7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			     17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t H3AlRanmum[51] = {4.40,  4.87,  5.36,  5.86,  6.37,  7.45,  8.58,  9.77,  11.01, 12.31,
			     13.66, 15.06, 16.50, 18.00, 19.55, 21.14, 24.45, 28.85, 33.51, 38.43,
			     43.59, 49.00, 54.68, 60.64, 66.88, 80.11, 94.36, 109.61,125.83,143.02,
			     161.15,200.15,242.77,288.94,338.56,391.59,447.95,507.60,570.48,636.57,
			     705.81,778.17,932.06,1140., 1370., 1620., 1880., 2160., 2460., 2780.,3110.};
  TGraph *H3_SRIM_RangeAlBacking_umMeV = new TGraph(51,H3AlEneMeV,H3AlRanmum);

  // He3 in Aluminium
  Double_t He3AlEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			      1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			      3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			      7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			      17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t He3AlRanmum[51] = {1.66,  1.80,  1.94,  2.08,  2.23,  2.52,  2.82,  3.14,  3.46,  3.80,
			      4.15,  4.50,  4.87,  5.25,  5.64,  6.04,  6.88,  7.99,  9.16,  10.40,
			      11.71, 13.08, 14.51, 16.01, 17.57, 20.87, 24.40, 28.17, 32.17, 36.39,
			      40.84, 50.46, 61.00, 72.41, 84.70, 97.83, 111.79,126.57,142.15,158.54,
			      175.71,193.66,231.84,283.79,340.37,401.49,467.05,537.00,611.27,689.80,772.54};
  TGraph *He3_SRIM_RangeAlBacking_umMeV = new TGraph(51,He3AlEneMeV,He3AlRanmum);
  
#ifdef NOSPLINES
   TGraph *RangeAl_umMeV = new TGraph(51,He3AlEneMeV,He3AlRanmum);
   TGraph *EnergyAl_MeVum = new TGraph(51,He3AlRanmum,He3AlEneMeV);
#else
   Range_umMeV = new TGraph(51,He3AlEneMeV,He3AlRanmum);
   Energy_MeVum = new TGraph(51,He3AlRanmum,He3AlEneMeV);  
   TSpline3 *RangeAl_umMeV = new TSpline3("RangeAl_umMeV",Range_umMeV);  
   TSpline3 *EnergyAl_MeVum = new TSpline3("EnergyAl_MeVum",Energy_MeVum);      
   out.cd("Ranges");
      Range_umMeV->Write("RangeAl_umMeV");
      Energy_MeVum->Write("EnergyAl_MeVum");
   gROOT->cd();
#endif  

  // He4 in Aluminium
  Double_t He4AlEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
			      1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
			      3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
			      7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
			      17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t He4AlRanmum[51] = {1.78,  1.91,  2.05,  2.19,  2.33,  2.61,  2.89,  3.18,  3.48,  3.79,
			      4.10,  4.42,  4.75,  5.09,  5.44,  5.79,  6.53,  7.49,  8.51,  9.57,
			      10.69, 11.86, 13.08, 14.35, 15.67, 18.45, 21.42, 24.58, 27.92, 31.44,
			      35.13, 43.04, 51.62, 60.93, 70.95, 81.64, 92.99, 105.00,117.65,130.94,
			      144.85,159.38,190.26,232.21,277.84,327.07,379.84,436.08,495.75,558.81,625.20};
  TGraph *He4_SRIM_RangeAlBacking_umMeV = new TGraph(51,He4AlEneMeV,He4AlRanmum);
 
#ifdef NOSPLINES
   TGraph *AlphaAluminium_Range_umMeV = new TGraph(51,He4AlEneMeV,He4AlRanmum);
   TGraph *AlphaAluminium_Energy_MeVum = new TGraph(51,He4AlRanmum,He4AlEneMeV); 
#else
   Range_umMeV = new TGraph(51,He4AlEneMeV,He4AlRanmum);
   Energy_MeVum = new TGraph(51,He4AlRanmum,He4AlEneMeV);  
   TSpline3 *AlphaAluminium_Range_umMeV = new TSpline3("AlphaAluminium_Range_umMeV",Range_umMeV);  
   TSpline3 *AlphaAluminium_Energy_MeVum = new TSpline3("AlphaAluminium_Energy_MeVum",Energy_MeVum);   
   out.cd("Ranges");
      Range_umMeV->Write("AlphaAluminium_Range_umMeV");
      Energy_MeVum->Write("AlphaAluminium_Energy_MeVum");
   gROOT->cd();
#endif   
   
  // Proton in AmO2
  Double_t H1TargetEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
				 1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
				 3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
				 7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
				 17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t H1TargetRanmum[51] = {2.94,  3.36,  3.80,  4.26,  4.74,  5.77,  6.87,  8.04,  9.27,  10.56,
				 11.89, 13.28, 14.72, 16.21, 17.75, 19.34, 22.66, 27.08, 31.79, 36.77,
				 42.03, 47.56, 53.35, 59.39, 65.68, 79.00, 93.27, 108.47,124.58,141.57,
				 159.43,197.66,239.19,283.93,331.79,382.70,436.59,493.42,553.12,615.65,
				 680.97,749.04,893.21,1090., 1300., 1530., 1770., 2020., 2300., 2580., 2880.};
  TGraph *H1_SRIM_RangeTarget_umMeV = new TGraph(51,H1TargetEneMeV,H1TargetRanmum);

  // Deuterons in AmO2
  Double_t H2TargetEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
				 1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
				 3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
				 7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
				 17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t H2TargetRanmum[51] = {2.60,  2.89,  3.20,  3.52,  3.86,  4.57,  5.34,  6.15,  7.01,  7.91,
				 8.86,  9.85,  10.88, 11.95, 13.06, 14.20, 16.61, 19.78, 23.12, 26.62,
				 30.28, 34.10, 38.07, 42.21, 46.49, 55.50, 65.10, 75.26, 85.97, 97.22,
				 109.00,134.08,161.16,190.17,221.05,253.77,288.27,324.52,362.50,402.17,
				 443.50,486.46,577.20,699.43,831.24,972.38,1120., 1280., 1450., 1630., 1810.};
  TGraph *H2_SRIM_RangeTarget_umMeV = new TGraph(51,H2TargetEneMeV,H2TargetRanmum);

  // Tritons in AmO2
  Double_t H3TargetEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
				 1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
				 3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
				 7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
				 17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t H3TargetRanmum[51] = {2.66,  2.91,  3.18,  3.54,  3.73,  4.32,  4.94,  5.60,  6.30,  7.03,
				 7.79,  8.59,  9.42,  10.28, 11.18, 12.10, 14.03, 16.60, 19.33, 22.23,
				 25.28, 28.46, 31.74, 35.14, 38.64, 45.98, 53.74, 61.92, 70.51, 79.50,
				 88.88, 108.79,130.19,153.04,177.29,202.91,229.86,258.11,287.64,318.43,
				 350.46,383.70,453.73,547.80,648.96,757.01,871.81,993.19,1120., 1260., 1400.};
  TGraph *H3_SRIM_RangeTarget_umMeV = new TGraph(51,H3TargetEneMeV,H3TargetRanmum);

  // He3 in AmO2
  Double_t He3TargetEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
				  1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
				  3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
				  7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
				  17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t He3TargetRanmum[51] = {0.9149,0.9853,1.06,  1.13,  1.20,  1.35,  1.50,  1.65,  1.82,  1.99,
				  2.16,  2.34,  2.53,  2.72,  2.92,  3.12,  3.55,  4.11,  4.70,  5.33,
				  5.99,  6.67,  7.39,  8.13,  8.90,  10.52, 12.25, 14.07, 16.00, 18.01,
				  20.11, 24.58, 29.40, 34.57, 40.60, 45.88, 52.02, 58.46, 65.21, 72.26,
				  79.60, 87.23, 103.34,125.03,148.41,173.43,200.06,228.27,258.03,289.30,322.07};
  TGraph *He3_SRIM_RangeTarget_umMeV = new TGraph(51,He3TargetEneMeV,He3TargetRanmum);
  
#ifdef NOSPLINES
   TGraph *RangeAm_umMeV = new TGraph(51,He3TargetEneMeV,He3TargetRanmum);
   TGraph *EnergyAm_MeVum = new TGraph(51,He3TargetRanmum,He3TargetEneMeV);
#else
   Range_umMeV = new TGraph(51,He3TargetEneMeV,He3TargetRanmum);
   Energy_MeVum = new TGraph(51,He3TargetRanmum,He3TargetEneMeV);  
   TSpline3 *RangeAm_umMeV = new TSpline3("RangeAm_umMeV",Range_umMeV);  
   TSpline3 *EnergyAm_MeVum = new TSpline3("EnergyAm_MeVum",Energy_MeVum);    
   out.cd("Ranges");
      Range_umMeV->Write("RangeAm_umMeV");
      Energy_MeVum->Write("EnergyAm_MeVum");
   gROOT->cd();
#endif  
   
   
  // He4 in AmO2
  Double_t He4TargetEneMeV[51] = {0.50,  0.55,  0.60,  0.65,  0.70,  0.80,  0.90,  1.00,  1.10,  1.20,
				  1.30,  1.40,  1.50,  1.60,  1.70,  1.80,  2.00,  2.25,  2.50,  2.75,
				  3.00,  3.25,  3.50,  3.75,  4.00,  4.50,  5.00,  5.50,  6.00,  6.50,
				  7.00,  8.00,  9.00,  10.00, 11.00, 12.00, 13.00, 14.00, 15.00, 16.00,
				  17.00, 18.00, 20.00, 22.50, 25.00, 27.50, 30.00, 32.50, 35.00, 37.50,40.00};
  Double_t He4TargetRanmum[51] = {1.01,  1.08,  1.15,  1.23,  1.30,  1.44,  1.58,  1.73,  1.88,  2.03,
				  2.19,  2.35,  2.52,  2.69,  2.87,  3.05,  3.42,  3.91,  4.43,  4.97,
				  5.54,  6.14,  6.75,  7.40,  8.06,  9.46,  10.94, 12.50, 14.15, 15.87,
				  17.67, 21.49, 25.58, 29.94, 34.57, 39.46, 44.62, 50.02, 55.67, 61.56,
				  67.69, 74.06, 84.47, 105.50,124.90,145.62,167.65,190.95,215.49,241.26,268.24};
  TGraph *He4_SRIM_RangeTarget_umMeV = new TGraph(51,He4TargetEneMeV,He4TargetRanmum);
 
#ifdef NOSPLINES
   TGraph *AlphaTarget_Range_umMeV = new TGraph(51,He4TargetEneMeV,He4TargetRanmum);
   TGraph *AlphaTarget_Energy_MeVum = new TGraph(51,He4TargetRanmum,He4TargetEneMeV); 
#else
   Range_umMeV = new TGraph(51,He4TargetEneMeV,He4TargetRanmum);
   Energy_MeVum = new TGraph(51,He4TargetRanmum,He4TargetEneMeV);  
   TSpline3 *AlphaTarget_Range_umMeV = new TSpline3("AlphaTarget_Range_umMeV",Range_umMeV);  
   TSpline3 *AlphaTarget_Energy_MeVum = new TSpline3("AlphaTarget_Energy_MeVum",Energy_MeVum);  
   out.cd("Ranges");
      Range_umMeV->Write("AlphaTarget_Range_umMeV");
      Energy_MeVum->Write("AlphaTarget_Energy_MeVum");
   gROOT->cd();
#endif  

int i_Si=0,i_Al=1,i_Am0=2;
  
#ifdef NOSPLINES
vector<vector<TGraph*>> Range_um ={{RangeSi_umMeV,RangeAl_umMeV,RangeAm_umMeV},
                                          {AlphaSilicon_Range_umMeV,AlphaAluminium_Range_umMeV,AlphaTarget_Range_umMeV}};
vector<vector<TGraph*>> Energy_MeV ={{EnergySi_MeVum,EnergyAl_MeVum,EnergyAm_MeVum},
                                          {AlphaSilicon_Energy_MeVum,AlphaAluminium_Energy_MeVum,AlphaTarget_Energy_MeVum}};
#else

vector<vector<TSpline3*>> Range_um ={{RangeSi_umMeV,RangeAl_umMeV,RangeAm_umMeV},
                                          {AlphaSilicon_Range_umMeV,AlphaAluminium_Range_umMeV,AlphaTarget_Range_umMeV}};
vector<vector<TSpline3*>> Energy_MeV ={{EnergySi_MeVum,EnergyAl_MeVum,EnergyAm_MeVum},
                                          {AlphaSilicon_Energy_MeVum,AlphaAluminium_Energy_MeVum,AlphaTarget_Energy_MeVum}};
#endif  
