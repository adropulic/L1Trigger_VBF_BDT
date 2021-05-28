/*******************************************************************/
/* makeEfficienciesPlot.C                                          */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeEfficienciesPlot.C                     */
/*******************************************************************/

#include "efficiencyHist.C"
#include "calculateEfficiency.C"
#include "calculateL1TrackEfficiency.C"
#include "calculatePFCandEfficiency.C"

#include <string>
/*********************************************************************/

/* Plots L1 tau efficiency for different BDT discriminant working
   points. */

void makeEfficienciesPlot(void)
{
  gROOT->ProcessLine(".L calculateEfficiency.C");

  /* Load the TTree. */
        //TString inputFilename_ZB = "/afs/cern.ch/work/a/addropul/ZeroBias_all.root";
	//does not have bdt disciminant in it yet
        //TFile *inputFile_ZB = new TFile(inputFilename_ZB.Data());
        //TTree *inputTree_ZB = (TTree*) inputFile_ZB->Get("l1NtupleProducer/Stage3Regions/efficiencyTree");
  TString treePath = "l1NtupleProducer/Stage3Regions/efficiencyTree";
  TString rootFileDirectory = "/afs/cern.ch/work/a/addropul/public/forAditya/l1TNtuple-ZeroBias-080620_norecomatch_manybdtstest.root";
  //TString rootFileDirectory = "/afs/cern.ch/work/a/addropul/public/forAditya/l1TNtuple-ZeroBias-072020_norecomatch_newbdtcut.root";
  //l1TNtuple-ZeroBias-080220_norecomatch_manybdtstest.root
  TString outputDirectory = "/afs/cern.ch/user/a/addropul/CMSSW_10_6_0_pre4/src/L1Trigger/Run3Ntuplizer/test/June_July_2019/efficiency_plots/plots/";

  float xMin, xMax;
  TString recoCut, l1Cut, l1Cut_cutbasedcomparison, l1Cut_forcompcut;

  /*******************************************************/
  /* efficiency as a function of recoPt                  */
  /*******************************************************/

  xMin = 0;
  xMax = 100;
  recoCut = "recoPt_1>10 && recoPt_2>10";
  l1Cut   = "recoPt_1>10 && recoPt_2>10 && l1Pt_1>0 && l1Pt_2>0";//add l1mass > 100 for comparison to cut-based
  l1Cut_forcompcut   = "recoPt_1>10 && recoPt_2>10 && l1Pt_1>0 && l1Pt_2>0 && l1Mass>100";
  l1Cut_cutbasedcomparison = "recoPt_1>10 && recoPt_2>10 && l1Pt_1>30 && l1Pt_2>30 && l1Mass>200";
  
  TGraphAsymmErrors* recoPtVLoose = calculateEfficiency("recoPt_1", treePath, rootFileDirectory,
						     l1Cut + "&& (bdtDiscriminant_2>0.100)",
						     recoCut, xMin, xMax, true);
  
  TGraphAsymmErrors* recoPtLoose = calculateEfficiency("recoPt_1", treePath, rootFileDirectory,
						       l1Cut,
						       recoCut, xMin, xMax, true);
/*  
  TGraphAsymmErrors* recoPtMedium = calculateEfficiency("recoPt_1", treePath, rootFileDirectory,
							l1Cut + "&& (bdtDiscriminant_10>0.100)",
							recoCut, xMin, xMax, true);
*/  
  TGraphAsymmErrors* recoPtMedium = calculateEfficiency("recoPt_1", treePath, rootFileDirectory,
                                                        l1Cut_cutbasedcomparison,
                                                        recoCut, xMin, xMax, true);
  
  TGraphAsymmErrors* recoPtTight = calculateEfficiency("recoPt_1", treePath, rootFileDirectory,
						       l1Cut + "&& (bdtDiscriminant_25>0.100)",
						       recoCut, xMin, xMax, true);


  TGraphAsymmErrors* recoPtNoBDT = calculateEfficiency("recoPt_1", treePath, rootFileDirectory,
						       l1Cut_forcompcut +"&& (bdtDiscriminant>0.100)",
						       recoCut, xMin, xMax, true);

  plotThreeHists(
                recoPtMedium, "Cut-based",
                recoPtNoBDT, "50 trees, BDT Discriminant > 0.1",
                recoPtLoose, "No BDT",
                "Leading reco jet  p_{T} [GeV]",
                "Zero Bias",
                "effleading_recoPt_ZB_norecomatch_comparecutbased.png",
                outputDirectory);

/*
  plotFiveHists(
		recoPtVLoose, "2 trees", kBlue-3,
		recoPtMedium, "10 trees", kPink+8,
		recoPtTight, "25 trees", kPink+6,
                recoPtNoBDT, "50 trees", kAzure+1,
                recoPtLoose, "No BDT", kViolet-5,
		"Subleading reco jet  p_{T} [GeV]",
		"Zero Bias, BDT Discriminant > 0.1",
		"effsubleading_recoPt_ZB_norecomatch_tight.png",
		outputDirectory);
*/
  /*******************************************************/
  /* efficiency as a function of recoEta                 */
  /*******************************************************/
  /*  xMin = -3;
  xMax = 3;
  
  recoCut = "recoPt>30 && genPt>20 && l1Track_pt>10 && genDM>9";
  l1Cut   = "recoPt>30 && genPt>20 && l1Track_pt>10 && genDM>9 && l1Pt>10";

  TGraphAsymmErrors* effVsRecoEta90 = calculateEfficiency("recoEta", treePath, rootFileDirectory,
							  l1Cut + "&& (l1BDTDisriminant > -0.0435867)",
							  recoCut, xMin, xMax, false);
  TGraphAsymmErrors* effVsRecoEta95 = calculateEfficiency("recoEta", treePath, rootFileDirectory,
                                                          l1Cut + "&& (l1BDTDisriminant > -0.0486184)",
							  recoCut, xMin, xMax, false);
  TGraphAsymmErrors* effVsRecoEtaNoBDT = calculateEfficiency("recoEta", treePath, rootFileDirectory, 
							     l1Cut,
							     recoCut, xMin, xMax, false);

  plotThreeHists(
		 effVsRecoEta90, "BDT Loose",
		 effVsRecoEta95, "BDT VLoose",
		 effVsRecoEtaNoBDT, "No BDT",
		 "Reco #tau_{H} #eta",
		 "Phase 2 L1 Taus (all #tau_{H} decay modes)",
		 "effVsRecoEta_allDM_l1TracksPt10.png",
		 outputDirectory);
  */
}


/*********************************************************************/
