#ifndef CommonIMapIncludes_h
#define CommonIMapIncludes_h

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <TH2.h>
#include <TH1.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TBranch.h>
#include <TString.h>
#include <fstream>
#include <TFile.h>

using namespace std;

const double hbar = 0.197/(3e8 * 1e15);
const double massproton = 0.938272;
const double pival = 3.1416;

struct HistList{

  //Single array histos
  vector<TH2D *> _hmat;	
  vector<TH2D *> _hvol;
  vector<TH2D *> _hmatbkw;
  vector<TH2D *> _hxfpt_tot;
  vector<TH1F *> _henergytotal;
  vector<TH1F *> _hkepop_tot;
  vector<TH1F *> _htmpop_tot;
	
  //Double array histos
  vector< vector<TH2D *> > _hxfpt;
  vector< vector<TH1F *> > _henergymaterial;
  vector< vector<TH1F *> > _henergyvolume;
  vector< vector<TH1F *> > _hkepop;
  vector< vector<TH1F *> > _htmpop; 
	
  //individual histos
  TH2D * _h_in_vs_mat;	
  TH2D * _h_nint_vs_enu;
  TH2D * _h_nint_vs_enu_cuts;
  TH1D * _h_aveint_vs_enu_thin_pCpion; 
  TH1D * _h_aveint_vs_enu_thin_pCkaon; 
  TH1D * _h_aveint_vs_enu_thin_nCpion;
  TH1D * _h_aveint_vs_enu_thin_pCnucleon;
  TH1D * _h_aveint_vs_enu_thin_mesoninc;
  TH1D * _h_aveint_vs_enu_pipCpip;           //Bhumika
  TH1D * _h_aveint_vs_enu_thin_nucleona;
  TH1D * _h_aveint_vs_enu_others;
  TH1D * _h_aveint_vs_enu_tot;
  TH1D * _h_nuflux;

  // histograms documenting the HP weights as a function of xF,pT
  TH2D* _h_occ_xfpt_pc_pip;
  TH2D* _h_hpwgt_xfpt_pc_pip;
  TH2D* _h_occ_xfpt_pc_kp;
  TH2D* _h_hpwgt_xfpt_pc_kp;

};

namespace IMap {
  static const int npop=9;
  static const std::string popparticle[npop] = {"proton","pi+","pi-","neutron","K+","K-","K_S0","K_L0","Lambda0"};

static const int nspecialmat=5;
 static const std::string matlist[nspecialmat] = {"Iron","Aluminum","Carbon","Helium","Steel"};

 static const int nvol=261;
  static const int nvoldune = 145;//280;
 static const std::string volume[nvol] = {"TGAR","TUNE","AddedLV","Al_BLK1","Al_BLK2","Al_BLK3","Al_BLK4","Al_BLK5","Al_BLK6","Al_BLK7","Al_BLK8","AlholeL","AlholeR","AlTube1LV","AlTube2LV","BeDWLV","BEndLV","BeUp1LV","BeUp2LV","BeUp3LV","BFrontLV","blu_BLK1","blu_BLK10","blu_BLK11","blu_BLK12","blu_BLK13","blu_BLK14","blu_BLK15","blu_BLK16","blu_BLK17","blu_BLK18","blu_BLK19","blu_BLK2","blu_BLK20","blu_BLK21","blu_BLK22","blu_BLK23","blu_BLK24","blu_BLK25","blu_BLK26","blu_BLK27","blu_BLK28","blu_BLK29","blu_BLK3","blu_BLK30","blu_BLK31","blu_BLK32","blu_BLK33","blu_BLK34","blu_BLK35","blu_BLK36","blu_BLK37","blu_BLK38","blu_BLK39","blu_BLK4","blu_BLK40","blu_BLK41","blu_BLK42","blu_BLK43","blu_BLK44","blu_BLK45","blu_BLK46","blu_BLK47","blu_BLK48","blu_BLK49","blu_BLK5","blu_BLK50","blu_BLK51","blu_BLK52","blu_BLK53","blu_BLK54","blu_BLK55","blu_BLK56","blu_BLK57","blu_BLK58","blu_BLK59","blu_BLK6","blu_BLK60","blu_BLK61","blu_BLK62","blu_BLK63","blu_BLK64","blu_BLK65","blu_BLK66","blu_BLK67","blu_BLK68","blu_BLK69","blu_BLK7","blu_BLK70","blu_BLK71","blu_BLK72","blu_BLK73","blu_BLK74","blu_BLK75","blu_BLK76","blu_BLK77","blu_BLK78","blu_BLK79","blu_BLK8","blu_BLK80","blu_BLK81","blu_BLK82","blu_BLK83","blu_BLK84","blu_BLK85","blu_BLK86","blu_BLK87","blu_BLK88","blu_BLK9","BodyLV","BudalMonitor","CeramicRod","CerTubeLV","ChamberLayer","CLid1LV","CLid2LV","conc_BLK","Concrete Chase Section","ConcShield","Conn1LV","Conn2LV","Conn3LV","CShld_BLK1","CShld_BLK10","CShld_BLK11","CShld_BLK12","CShld_BLK2","CShld_BLK3","CShld_BLK4","CShld_BLK5","CShld_BLK6","CShld_BLK7","CShld_BLK8","CShld_BLK9","CShld_stl,BLK","DNWN","DPIP","DuratekBlock","DVOL","HadCell","HadronAbsorber","MuCell","MuMon_0","MuMon_1","MuMon_2","MuMonAlcv_0","MuMonAlcv_1","MuMonAlcv_2","MuMonAlcvFill_0","MuMonAlcvShot_1_Down","MuMonAlcvShot_1_Up","MuMonAlcvShot_2_Down","MuMonAlcvShot_2_Up","PHorn1CPB1slv","PHorn1CPB2slv","PHorn1F","PHorn1Front","PHorn1IC","PHorn1InsRingslv","PHorn1OC","PHorn2CPB1slv","PHorn2CPB2slv","PHorn2F","PHorn2Front","PHorn2IC","PHorn2InsRingslv","PHorn2OC","Pipe1","Pipe1_water","Pipe1tp","Pipe1tp_water","Pipe2","Pipe2_water","Pipe2btm","Pipe2btm_water","Pipe3","Pipe3_water","Pipe4","Pipe4_water","Pipe5","Pipe5_water","Pipe6","Pipe6_water","Pipe7","Pipe8","Pipe8_water","Pipe9","PipeAdapter1","PipeAdapter1_water","PipeAdapter2","PipeAdapter2_water","PipeBellowB","PipeBellowB_water","PipeBellowT","PipeBellowT_water","PipeC1","PipeC1_water","PipeC2","PipeC2_water","PipeEndB","PipeEndB_water","PipeEndT","PipeEndT_water","pvBaffleMother","pvDPInnerTrackerEnd","pvDPInnerTrackerTube","pvDPOuterTrackerEnd","pvDPOuterTrackerTube","PVHadMon","pvMHorn1Mother","pvMHorn2Mother","pvTargetMother","Ring1LV","Ring2LV","Ring3LV","Ring4LV","Ring5LV","ROCK","SC01","SpiderSupport","Stl_BLK1","Stl_BLK10","Stl_BLK2","Stl_BLK3","Stl_BLK4","Stl_BLK5","Stl_BLK6","Stl_BLK7","Stl_BLK8","Stl_BLK9","stl_slab1","stl_slab225","stl_slab3","stl_slab4","stl_slab5","stl_slabL","stl_slabR","Stlhole","TGT1","TGTExitCyl1LV","TGTExitCyl2LV","TGTExitTopLV","topstl1_BLK","topstl2_BLK","topstl3_BLK","topstl4_BLK","topstl5_BLK","topstl6_BLK","topstl7_BLK","Tube1aLV","Tube1bLV","UpWn1","UpWn2","UpWnAl1SLV","UpWnAl2SLV","UpWnAl3SLV","UpWnFe1SLV","UpWnFe2SLV","UpWnPolyCone","PHorn1ICWater","PHorn2ICWater"};

 static const std::string materials[nvol] = {"Air","Air","Iron","Aluminum","Aluminum","Aluminum","Aluminum","Aluminum","Aluminum","Aluminum","Aluminum","Aluminum","Aluminum","Aluminum","Aluminum","Berillium","Iron","Berillium","Iron","Iron","Iron","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Iron","Carbon","CT852","CT852","Helium","Iron","Iron","Rebar_Concrete","Concrete","Air","Iron","Iron","Iron","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Rebar_Concrete","Steel","Iron","Iron","Iron","Helium","Helium","Air","Helium","Air","Air","Air","Air","Air","Air","Shotcrete","Shotcrete","Shotcrete","Shotcrete","Shotcrete","Aluminum","Aluminum","Air","Aluminum","Aluminum","CT852","Aluminum","Aluminum","Aluminum","Air","Aluminum","Aluminum","CT852","Aluminum","Iron","Water","Iron","Water","Iron","Water","Iron","Water","Iron","Water","Iron","Water","Iron","Water","Iron","Water","Iron","Iron","Water","Iron","Iron","Water","Iron","Water","Iron","Water","Iron","Water","CT852","Water","CT852","Water","Iron","Water","Iron","Water","Carbon","Helium","Helium","Vacuum","Vacuum","Air","Argon","Argon","Helium","Aluminum","Aluminum","Aluminum","Aluminum","Aluminum","DoloStone","Concrete","Aluminum","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Carbon","Air","Air","Air","Steel","Steel","Steel","Steel","Steel","Steel","Steel","Iron","Iron","Aluminum","Iron","Aluminum","Aluminum","Aluminum","Iron","Iron","Iron","Water","Water"};

  static const std::string volumedune[nvoldune] ={"Tunnel", "TargetNoSplitRingTube", "TargetNoSplitSegment", "DecayPipeVolume", "LBNFConceptHornAOC", "TargetNoSplitHeContainer", "TargetHallAndHorn1", "TargetNoSplitBaffletCold", "TargetNoSplitM1", "Horn2IC", "LBNFConceptHornCICFlDTr", "DecayPipeWall", "DecayPipeHall", "Horn1IC", "DecayPipeConcrete", "LBNFConceptHornBUpstrCone", "LBNFConceptHornAICTaperWater_1", "LBNFSimpleHorn2Container", "LBNFConceptHornAICCylWater", "RockLogical", "LBNFConceptHornAFlangeDwnstrU", "LBNFConceptHornBOC", "DecayPipeOuterWall", "TargetNoSplitM1Helium", "LBNFConceptHornCOCEQbody", "Horn1PolyM1", "TargetNoSplitSimpleBafflet", "LBNFConceptHornBICCyl0", "LBNFConceptHornBICCyl0Weld", "LBNFConceptHornBOCDwnstrFl", "LBNFConceptHornCStrpLVert", "LBNFConceptHornCICEQFl4", "LBNFConceptHornCOC", "LBNFConceptHornBNeck", "LBNFConceptHornCICEQFl5", "LBNFConceptHornCICPoly", "LBNFSimpleHorn3Container", "LBNFConceptHornBUpstrConeWaterUp", "LBNFConceptHornCOCFlU2", "LBNFConceptHornCICEQbodyUp", "LBNFConceptHornBDwnstrCone", "LBNFConceptHornBCon2ICICEQ", "LBNFConceptHornCOCEQFlUp", "LBNFConceptHornBInsul", "HadronAbsorberSculpTop", "HadronAbsorberSculpMaskAl-2", "LBNFConceptHornCOCDwnEndFl", "LBNFConceptHornBOCCurrEq", "LBNFConceptHornAFlangeDwnstrD", "HadronAbsorberSculpAibufferDiag", "LBNFConceptHornCICFlUTr", "LBNFConceptHornATgtSupTitCTube", "LBNFConceptHornBICFlU", "HadronAbsorberSculpSculpAl-6", "HadronAbsorberSculpSculpAl-7", "HadronAbsorberSculpSculpAl-8", "LBNFConceptHornCInsulR", "LBNFConceptHornCICU1Water", "LBNFConceptHornBDwnstrFlIOSect9", "LBNFConceptHornCICDw0Water", "LBNFConceptHornBICCyl1WaterDw", "LBNFConceptHornBCon3ICICEQ", "LBNFConceptHornBICCyl1", "LBNFConceptHornCICU2Water", "LBNFConceptHornBICCyl0SPWS", "LBNFConceptHornBCon1ICICEQ", "TargetNoSplitDSSupportLargeCones", "LBNFConceptHornBUpstrConeSPWS", "LBNFConceptHornBICCurrEq", "TargetNoSplitDSSupportConnectionRing", "LBNFConceptHornBOCCurrEqFl", "LBNFConceptHornCStrpLMother", "LBNFConceptHornBICCyl1SPWS", "LBNFConceptHornBICCyl0WaterUp", "HadronAbsorberSculpSpoilerAir", "HadronAbsorberSculpMaskAl-1", "TargetNoSplitDSSupportInnerRing", "LBNFConceptHornBDwnstrFlIOSect2", "LBNFConceptHornBSpider_1_supportHanger", "LBNFConceptHornCSuppRing", "LBNFConceptHornCStrpLLong45D", "LBNFConceptHornCICEQFl3", "LBNFConceptHornBICCyl1Weld", "HadronAbsorberSculpSculpAl-11", "HadronAbsorberSculpSculpAl-13", "HadronAbsorberSculpSculpAl-14", "HadronAbsorberSculpSolidAl-15", "LBNFConceptHornCOCEQF3Dw", "HadronAbsorberSculpSculpAir-8", "HadronAbsorberSculpSculpAl-9", "HadronAbsorberSculpSculpAl-10", "HadronAbsorberSculpMaskAl-4", "HadronAbsorberSculpMaskAl-5", "TargetNoSplitCoolingTubeFirstMoth", "HadronAbsorberSculpMaskAl-3", "HadronAbsorberSculpMaskAir-3", "LBNFConceptHornAICTaperWater_2", "LBNFConceptHornBUpstrConeWaterDw", "LBNFConceptHornCOCFlU1", "LBNFConceptHornBDwnstrFlIOSect0", "LBNFConceptHornCWeldUp", "LBNFConceptHornCStrpLHorz", "TargetNoSplitSimpleBaffletFlange", "TargetNoSplitLargeConeHe", "TargetNoSplitCoolingTubeLast", "LBNFConceptHornCICEQFl1", "LBNFConceptHornBDwnstrFlIOSect3", "HadronAbsorberSculpSculpAl-12", "HadronAbsorberSculpSculpAir-12", "LBNFConceptHornBICCurrEqFlo_1", "LBNFConceptHornBDwnstrFlIOSect5", "LBNFConceptHornBICCurrEqFlo", "LBNFConceptHornBDwnstrFlIOSect7", "LBNFConceptHornBNeckWater", "LBNFConceptHornBICCyl0WaterDw", "HadronAbsorberSculpSculpAir-7", "HadronAbsorberSculpMaskAir-1", "HadronAbsorberSculpSolidAl-16", "HadronAbsorberSculpSolidAl-17", "TargetNoSplitCoolingTubeFirstHelium", "LBNFConceptHornBDwnstrFlIOSect1", "LBNFConceptHornCICD1Water", "LBNFConceptHornASpSuppHanger", "LBNFConceptHornASpSuppRiser", "HadronAbsorberSculpSculpAir-6", "TargetNoSplitBaffletColdHe", "LBNFConceptHornBOCCurrEqFld", "LBNFConceptHornAUpstrIOSect1", "HadronAbsorberSculpMaskHole-2", "LBNFConceptHornBICCurrEqFlo_2", "TargetNoSplitLargeCone", "HadronAbsorberSculpMaskHole-1", "LBNFConceptHornBICCyl1WaterUp", "HadronAbsorberSculpMaskHole-3", "LBNFConceptHornAUpstrIOSect4", "HadronAbsorberSculpMaskHole-4", "LBNFConceptHornBSpider_1_support_Riser", "LBNFConceptHornBDwnstrFlIOSect4", "HadronAbsorberSculpSculpAir-13", "Horn1TrackingPlaneLogical", "HadronAbsorberSculpEndIron-22", "LBNFConceptHornBICCurrEqFlo_3", "HadronAbsorberSculpSculpAir-10", "LBNFConceptHornAUpstrIOSect3", "LBNFConceptHornBDwnstrFlIOSect6"};

 static const std::string materialsdune[nvoldune] = {"Air", "Titanium", "Carbon", "DecayPipeGas", "Aluminum", "Titanium", "Air", "Titanium", "Titanium", "Aluminum", "Aluminum", "Steel", "Air", "Aluminum", "Concrete", "Aluminum", "Water", "Air", "Water", "Concrete", "Aluminum", "Aluminum", "Steel", "Helium", "Aluminum", "Argon", "Titanium", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Air", "Water", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Iron", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Air", "Aluminum", "Titanium", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Water", "Aluminum", "Aluminum", "Water", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Titanium", "Aluminum", "Aluminum", "Titanium", "Aluminum", "Air", "Aluminum", "Water", "Aluminum", "Aluminum", "Titanium", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Air", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Titanium", "Aluminum", "Air", "Water", "Water", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Titanium", "Helium", "Titanium", "Aluminum", "Aluminum", "Aluminum", "Air", "Aluminum", "Aluminum", "Aluminum", "Aluminum", "Water", "Water", "Air", "Air", "Aluminum", "Aluminum", "Helium", "Aluminum", "Water", "Aluminum", "Aluminum", "Air", "Helium", "Aluminum", "Aluminum", "Air", "Aluminum", "Titanium", "Air", "Water", "Air", "Aluminum", "Air", "Aluminum", "Aluminum", "Air", "Air", "Iron", "Aluminum", "Air", "Aluminum", "Aluminum"};


// is this needed? Should try to comment out and build.
 static const std::string vol_list[193] = {"AddedLV","BEndLV","BeUp2LV","BeUp3LV","BFrontLV","BodyLV","CLid1LV","CLid2LV","Conn1LV","Conn2LV","Conn3LV","DNWN","DPIP","DuratekBlock","Pipe1","Pipe1tp","Pipe2","Pipe2btm","Pipe3","Pipe4","Pipe5","Pipe6","Pipe7","Pipe8","Pipe9","PipeAdapter1","PipeAdapter2","PipeBellowB","PipeBellowT","PipeEndB","PipeEndT","Tube1aLV","Tube1bLV","UpWn2","UpWnFe1SLV","UpWnFe2SLV","UpWnPolyCone","Al_BLK1","Al_BLK2","Al_BLK3","Al_BLK4","Al_BLK5","Al_BLK6","Al_BLK7","Al_BLK8","AlholeL","AlholeR","AlTube1LV","AlTube2LV","PHorn1CPB1slv","PHorn1CPB2slv","PHorn1Front","PHorn1IC","PHorn1OC","PHorn2CPB1slv","PHorn2CPB2slv","PHorn2Front","PHorn2IC","PHorn2OC","Ring1LV","Ring2LV","Ring3LV","Ring4LV","Ring5LV","SpiderSupport","UpWn1","UpWnAl1SLV","UpWnAl2SLV","UpWnAl3SLV","BudalMonitor","pvBaffleMother","TGT1","ChamberLayer","DVOL","HadCell","MuCell","pvDPInnerTrackerEnd","pvDPInnerTrackerTube","pvTargetMother","blu_BLK1","blu_BLK10","blu_BLK11","blu_BLK12","blu_BLK13","blu_BLK14","blu_BLK15","blu_BLK16","blu_BLK17","blu_BLK18","blu_BLK19","blu_BLK2","blu_BLK20","blu_BLK21","blu_BLK22","blu_BLK23","blu_BLK24","blu_BLK25","blu_BLK26","blu_BLK27","blu_BLK28","blu_BLK29","blu_BLK3","blu_BLK30","blu_BLK31","blu_BLK32","blu_BLK33","blu_BLK34","blu_BLK35","blu_BLK36","blu_BLK37","blu_BLK38","blu_BLK39","blu_BLK4","blu_BLK40","blu_BLK41","blu_BLK42","blu_BLK43","blu_BLK44","blu_BLK45","blu_BLK46","blu_BLK47","blu_BLK48","blu_BLK49","blu_BLK5","blu_BLK50","blu_BLK51","blu_BLK52","blu_BLK53","blu_BLK54","blu_BLK55","blu_BLK56","blu_BLK57","blu_BLK58","blu_BLK59","blu_BLK6","blu_BLK60","blu_BLK61","blu_BLK62","blu_BLK63","blu_BLK64","blu_BLK65","blu_BLK66","blu_BLK67","blu_BLK68","blu_BLK69","blu_BLK7","blu_BLK70","blu_BLK71","blu_BLK72","blu_BLK73","blu_BLK74","blu_BLK75","blu_BLK76","blu_BLK77","blu_BLK78","blu_BLK79","blu_BLK8","blu_BLK80","blu_BLK81","blu_BLK82","blu_BLK83","blu_BLK84","blu_BLK85","blu_BLK86","blu_BLK87","blu_BLK88","blu_BLK9","CShld_stl,BLK","Stl_BLK1","Stl_BLK10","Stl_BLK2","Stl_BLK3","Stl_BLK4","Stl_BLK5","Stl_BLK6","Stl_BLK7","Stl_BLK8","Stl_BLK9","stl_slab1","stl_slab225","stl_slab3","stl_slab4","stl_slab5","stl_slabL","stl_slabR","Stlhole","topstl1_BLK","topstl2_BLK","topstl3_BLK","topstl4_BLK","topstl5_BLK","topstl6_BLK","topstl7_BLKroot"};




}


#endif
