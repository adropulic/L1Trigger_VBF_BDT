/*******************************************************************/
/* efficiencyHist.C                                                */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH1F.h"

#include "TAxis.h"
#include "TChain.h"

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

#include "../base_code_for_plots/tdrstyle.C"

#ifndef EFFICIENCY_HIST_INCL
#define EFFICIENCY_HIST_INCL

/*******************************************************************/

void plotThreeHists(TGraphAsymmErrors* histLoose, TString labelLoose,
		    TGraphAsymmErrors* histVLoose, TString labelVLoose,
		    TGraphAsymmErrors* histNoBDT, TString labelNoBDT,
		    TString xAxisLabel,
		    TString legendName,
		    TString outputName,
		    TString outputDir
		    )
{
  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 1000, 800);
  TLegend* leg = new TLegend(0.60,0.75,0.85,0.9);
  Tcan->SetGrid();

  Tcan->cd();     /* Set current canvas */
  Tcan->SetFillColor(0);
  leg = new TLegend(0.55, 0.2, 0.90, 0.5);
  applyLegStyle(leg);

  /* Loose */
  histLoose->SetMarkerColor(kViolet+9);
  histLoose->SetLineColor(kViolet+9);
  histLoose->SetMarkerSize(1.5);
  histLoose->SetMarkerStyle(kFullCircle);
  histLoose->SetLineWidth(2);

  /* VLoose */
  histVLoose->SetMarkerColor(kViolet-5);
  histVLoose->SetLineColor(kViolet-5);
  histVLoose->SetMarkerSize(1.5);
  histVLoose->SetMarkerStyle(kFullCircle);
  histVLoose->SetLineWidth(2);

  /* No BDT */
  histNoBDT->SetMarkerColor(kPink+5);
  histNoBDT->SetLineColor(kPink+5);
  histNoBDT->SetMarkerSize(1.5);
  histNoBDT->SetMarkerStyle(kFullCircle);
  histNoBDT->SetLineWidth(2);
  
  /* Dummy */
  TGraphAsymmErrors* histDummy = new TGraphAsymmErrors(*histLoose);
  histDummy->SetMarkerColor(0);
  histDummy->SetLineColor(0);
  

  histDummy->Draw("");
  histLoose->Draw("P");
  histVLoose->Draw("P");
  histNoBDT->Draw("P");

  histDummy->GetXaxis()->SetTitle(xAxisLabel);
  histDummy->GetYaxis()->SetTitle("L1 Efficiency");
  histDummy->GetXaxis()->SetTitleSize(0.06); // default is 0.03                                                              

  /* Set y-axis limits */
  histDummy->GetYaxis()->SetRangeUser(0.0, 1.1);

  /* Customize legend */
  //  leg->SetHeader(legendName);
  leg->AddEntry(histNoBDT, labelNoBDT, "P");
  leg->AddEntry(histVLoose, labelVLoose, "P");
  leg->AddEntry(histLoose, labelLoose, "P");

  gStyle->SetLegendFont(25);
  gStyle->SetLegendTextSize(0.03);
  leg->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDir+outputName);


}

/*******************************************************************/

/* Plots loose/medium/tight efficiency histograms and saves to a .png. */

void plotFiveHists(
		   TGraphAsymmErrors* h1, TString s1, int c1,
		   TGraphAsymmErrors* h2, TString s2, int c2,
		   TGraphAsymmErrors* h3, TString s3, int c3,
		   TGraphAsymmErrors* h4, TString s4, int c4,
		   TGraphAsymmErrors* h5, TString s5, int c5,
		   TString xAxisLabel,
		   TString legendName,
		   TString outputName,
		   TString outputDir
		   )
{

  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 1000, 800);
  TLegend* leg = new TLegend(0.60,0.75,0.85,0.9);
  Tcan->SetGrid();

  Tcan->cd();     /* Set current canvas */
  Tcan->SetFillColor(0);
  leg = new TLegend(0.55, 0.2, 0.90, 0.5);
  applyLegStyle(leg);

  h1->SetMarkerColor(c1);
  h1->SetMarkerStyle(kFullCircle);
  h1->SetLineWidth(2);
  h1->SetLineColor(c1);
  h1->SetMarkerSize(2);

  h2->SetMarkerColor(c2);
  h2->SetMarkerStyle(kFullCircle);
  h2->SetLineWidth(2);
  h2->SetLineColor(c2);
  h2->SetMarkerSize(2);

  h3->SetMarkerColor(c3);
  h3->SetMarkerStyle(kFullCircle);
  h3->SetLineWidth(2);
  h3->SetLineColor(c3);
  h3->SetMarkerSize(2);

  h4->SetMarkerColor(c4);
  h4->SetMarkerStyle(kFullCircle);
  h4->SetLineWidth(2);
  h4->SetLineColor(c4);
  h4->SetMarkerSize(2);

  h5->SetMarkerColor(c5);
  h5->SetMarkerStyle(kFullCircle);
  h5->SetLineWidth(2);
  h5->SetLineColor(c5);		    
  h5->SetMarkerSize(2);

  /* Dummy */
  TGraphAsymmErrors* histDummy = new TGraphAsymmErrors(*h1);
  histDummy->SetMarkerColor(0);
  histDummy->SetLineColor(0);

  histDummy->Draw("");
  h1->Draw("P");
  h2->Draw("P");
  h3->Draw("P");
  h4->Draw("P");
  h5->Draw("P");

  histDummy->GetXaxis()->SetTitle(xAxisLabel);
  histDummy->GetYaxis()->SetTitle("L1 Efficiency");
  histDummy->GetXaxis()->SetTitleSize(0.06); // default is 0.03                                                                         

  /* Set y-axis limits */
  histDummy->GetYaxis()->SetRangeUser(0.0, 1.1);

  /* Customize legend */
  leg->SetHeader(legendName);
  leg->AddEntry(h5, s5, "P");
  leg->AddEntry(h4, s4, "P");
  leg->AddEntry(h3, s3, "P");
  leg->AddEntry(h2, s2, "P");
  leg->AddEntry(h1, s1, "P");
		
  gStyle->SetLegendFont(20);
  gStyle->SetLegendTextSize(0.03);
  leg->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDir+outputName);

}

/*******************************************************************/

/* Plots loose/medium/tight efficiency histograms and saves to a .png. */

void plotSixHists(
	       TGraphAsymmErrors* histLoose,
	       TGraphAsymmErrors* histMedium,
	       TGraphAsymmErrors* histMedium2,
               TGraphAsymmErrors* histTight,
               TGraphAsymmErrors* histTight2,
               TGraphAsymmErrors* histNoBDT,
	       TString xAxisLabel,
	       TString legendName,
	       TString outputName,
	       TString outputDir
	       )
{
  /*******************************************************/
  /* plotting                                            */
  /*******************************************************/
  
  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 1000, 800);
  TLegend* leg = new TLegend(0.60,0.75,0.85,0.9);
  Tcan->SetGrid();

  Tcan->cd();     /* Set current canvas */
  Tcan->SetFillColor(0);
  leg = new TLegend(0.55, 0.2, 0.90, 0.5);
  applyLegStyle(leg);

  /* Loose */
  histLoose->SetMarkerColor(kViolet+9);
  histLoose->SetMarkerStyle(kFullCircle);
  histLoose->SetLineWidth(2);
  histLoose->SetLineColor(kViolet+9);

  /* Medium */
  histMedium->SetMarkerColor(kViolet-5);
  histMedium->SetMarkerStyle(kFullCircle);
  histMedium->SetLineWidth(2);
  histMedium->SetLineColor(kViolet-5);

  /* Medium 2*/
  histMedium2->SetMarkerColor(kPink+5);
  histMedium2->SetMarkerStyle(kFullCircle);
  histMedium2->SetLineWidth(2);
  histMedium2->SetLineColor(kPink+5);

  /* Tight */
  histTight->SetMarkerColor(kOrange+2);
  histTight->SetMarkerStyle(kFullCircle);
  histTight->SetLineWidth(2);
  histTight->SetLineColor(kOrange+2);

  /* Tight2  */
  histTight2->SetMarkerColor(kRed-4);
  histTight2->SetMarkerStyle(kFullCircle);
  histTight2->SetLineWidth(2);
  histTight2->SetLineColor(kRed-4);

  /* No BDT */
  histNoBDT->SetMarkerColor(kGray+1);
  histNoBDT->SetMarkerStyle(kFullCircle);
  histNoBDT->SetLineWidth(2);
  histNoBDT->SetLineColor(kGray+1);

  /* Set y-axis limits */
  histLoose->GetYaxis()->SetRangeUser(0.0, 1.1);

  histLoose->Draw("");
  histMedium->Draw("SAME");
  histMedium2->Draw("SAME");
  histTight->Draw("SAME");
  histTight2->Draw("SAME");
  histNoBDT->Draw("SAME");

  histLoose->GetXaxis()->SetTitle(xAxisLabel);
  histLoose->GetYaxis()->SetTitle("L1 Efficiency");
  histLoose->GetXaxis()->SetTitleSize(0.06); // default is 0.03                                                                                                     
  /* Customize legend */
  leg->SetHeader(legendName);
  leg->AddEntry(histLoose, "60%", "l");
  leg->AddEntry(histMedium, "70%", "l");
  leg->AddEntry(histMedium2, "80%", "l");
  leg->AddEntry(histTight, "90%", "l");
  leg->AddEntry(histTight2, "95%", "l");
  leg->AddEntry(histNoBDT, "No BDT", "l");

  gStyle->SetLegendFont(30);
  leg->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDir+outputName);


}



/*******************************************************************/

#endif 
