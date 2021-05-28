/*******************************************************************/
/* calculatePFCandEfficiency.C                                     */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "../base_code_for_plots/CMS_lumi.h"
#include "../base_code_for_plots/tdrstyle.C"

#include "efficiencyHist.C"


#include "TAxis.h"
#include "TChain.h"
#include "TF1.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH1F.h"

#include "THStack.h"
#include "TLegend.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"

#include <cmath>
#include <iostream>
#include <math.h>
#include <string>
#include <TMath.h>
#include <vector>

#ifndef CALCULATE_PFCAND_EFFICIENCY_INCL
#define CALCULATE_PFCAND_EFFICIENCY_INCL

/*******************************************************************/

/* Returns efficiency of PF Candidates as a TGraphAsymmErrors*. 
   (Denominator is all true taus, numerator is PF Cands with pT 
    greater than pfPtCut.) Use to troubleshoot low efficiencies from
   poor PF Candidates during reconstruction. */

TGraphAsymmErrors* calculatePFCandEfficiency(TString treePath, TString rootFileDirectory,
					     int nBins,
					     TString variable,
					     TString region,
					     double xMin,
					     double xMax,
					     double recoPtCut,
					     double genPtCut,
					     double pfPtCut)
{
  /* Numerator and denominator histograms. */
  TH1F* numerator = new TH1F("numerator", "numerator", nBins, xMin, xMax);
  TH1F* denominator = new TH1F("denominator", "denominator", nBins, xMin, xMax);
  TH1F* efficiencyHist = new TH1F("efficiency", "efficiency", nBins, xMin, xMax);

  numerator->Sumw2();
  denominator->Sumw2();

  /* Load file */
  TFile *file = new TFile(rootFileDirectory);
  if (!file->IsOpen() || file==0 )
    {
      std::cout<<"ERROR FILE "<< rootFileDirectory <<" NOT FOUND; EXITING"<<std::endl;
      return 0;
    }
  
  TTree* tree = (TTree*)file->Get(treePath);
  if (tree == 0)
    {
      std::cout<<"ERROR Tau Tree is "<< tree<<" NOT FOUND; EXITING"<<std::endl;
      return 0;
    }

  if (! ((variable == "recoPt") || (variable == "genPt")))
    {
      std::cout<<"ERROR: parameter 'variable' is not recognized; EXITING"<<std::endl;
      return 0;
    }

  if (! ((region == "barrel") || (variable == "endcap")))
    {
      std::cout<<"ERROR: parameter 'region' must be 'barrel' or 'endcap'; EXITING"<<std::endl;
      return 0;
    }

  /* Declare variables to read in. */
  double recoPt, recoEta, recoPhi;
  double genPt, genEta, genPhi;
  int recoDM;
  
  double pfCand_pt;

  /* Set branch addresses. */
  tree->SetBranchAddress("recoPt",  &recoPt);
  tree->SetBranchAddress("recoEta", &recoEta);

  tree->SetBranchAddress("genPt",  &genPt);
  tree->SetBranchAddress("genEta", &genEta);

  tree->SetBranchAddress("recoDM", &recoDM);

  tree->SetBranchAddress("pfCand_pt", &pfCand_pt);

  /* Loop through TTree. */
  for (int i = 0; i < tree->GetEntries(); i++)
    {
      tree->GetEntry(i);
      
      bool passesOverallCut;
      bool passesEta;

      if (variable == "genPt")
	{
	  // Pseudorapidity
	  if (region == "barrel")
	    passesEta = (abs(genEta) < 1.44);
	  else if (region == "endcap")
	    passesEta = (abs(genEta) > 1.44);

	  passesOverallCut = ((genPt > genPtCut)
			      && passesEta
			      && (recoDM == 0)
			      );
	}
      else if (variable == "recoPt")
	{
	  // Pseudorapidity
	  if (region == "barrel")
            passesEta = (abs(recoEta) < 1.44);
	  else if (region == "endcap")
            passesEta = (abs(recoEta) > 1.44);

	  passesOverallCut = ((recoPt > recoPtCut) 
			      && (genPt > genPtCut)
			      && passesEta
			      && (recoDM == 0)
			      );
	}

      /* Fill denominator. */
      if (passesOverallCut)
	{
	  //	  printf("Denominator filled:  pfCand_pt is %f and recoPt is %f.\n", pfCand_pt, recoPt);   
	  if (variable == "genPt")
	    denominator->Fill(genPt);
	  else if (variable == "recoPt")
	    denominator->Fill(recoPt);

	  /* Fill numerator. */
	  if (pfCand_pt > pfPtCut)
	    {
	      if (variable == "genPt")
		numerator->Fill(genPt);
	      else if (variable == "recoPt")
		numerator->Fill(recoPt);

	      //	      printf("Numerator filled: l1Track_pt is %f and recoPt is %f.\n", l1Track_pt, recoPt);
	    }
	  
	}

    } /* end of loop over TTree */

  efficiencyHist->Divide(numerator, denominator);
  /*  for (int i = 0; i < nBins; i++)
    {
      printf("PF Cand efficiencyHist bin %d content is %f, with error %f\n", i, efficiencyHist->GetBinContent(i),
	     efficiencyHist->GetBinError(i));
      
    }
  */

  TGraphAsymmErrors* effGraph = new TGraphAsymmErrors();
  *effGraph = TGraphAsymmErrors(numerator, denominator);
  
  setMaxErrorTo1(effGraph);

  return effGraph;

}

/*******************************************************************/

#endif
