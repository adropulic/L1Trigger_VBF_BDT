# Standard python import
import conifer
import datetime
import xml.etree.ElementTree as ET
import sys    # exit
import time   # time accounting
import getopt # command line parser
import glob, array, argparse, math
import ROOT
import numpy as np
#from root_numpy import tree2array
ROOT.gROOT.SetBatch(True)
import sys, os
#ROOT.TMVA.Tools.Instance()
#THIS ONE IS CORRECT PLACE
# Import ROOT classes
from ROOT import gSystem, gROOT, gApplication, TFile, TTree, TCut
#inputFile_VBF = ROOT.TFile.Open("/afs/cern.ch/work/a/addropul/l1TNtuple-VBF_restrictedeta_062420_reta3p2.root","READ")
inputFile_VBF = ROOT.TFile.Open("/afs/cern.ch/work/a/addropul/public/forAditya/l1TNtuple-VBF_072020_norecomatch_newbdtcuts.root","READ")
signal = inputFile_VBF.Get("l1NtupleProducer/Stage3Regions/efficiencyTree")
if( not signal ):
	print("Error, could not get input tree")

#inputFile_ZB = ROOT.TFile.Open("/afs/cern.ch/work/a/addropul/l1TNtuple-ZeroBias-070520_newmatch.root")
inputFile_ZB = ROOT.TFile.Open('/afs/cern.ch/work/a/addropul/public/forAditya/l1TNtuple-ZeroBias-072020_norecomatch_newbdtcut.root', "READ")
background = inputFile_ZB.Get("l1NtupleProducer/Stage3Regions/efficiencyTree")
if( not background ):
	print("Error, could not get input tree")

# check ROOT version, give alarm if 5.18 
if gROOT.GetVersionCode() >= 332288 and gROOT.GetVersionCode() < 332544:
	print("*** You are running ROOT version 5.18, which has problems in PyROOT such that TMVA")
	print("*** does not run properly (function calls with enums in the argument are ignored)")
	print("*** Solution: either use CINT or a C++ compiled version (see TMVA/macros or TMVA/examples),")
	print("*** or use another ROOT version (e.g., ROOT 5.19).")
	sys.exit(1)

# Logon not automatically loaded through PyROOT (logon loads TMVA library) load also GUI
gROOT.SetMacroPath( "./" )
gROOT.Macro       ( "./TMVAlogon.C" )
#gROOT.LoadMacro   ( "./TMVAGui.C" )
# Import TMVA classes from ROOT
from ROOT import TMVA

# Output file
outputFile = TFile( "TMVA_output_ZB_newsig.root", 'RECREATE' )
print(outputFile)
# Create instance of TMVA factory (see TMVA/macros/TMVAClassification.C for more factory options)
# All TMVA output can be suppressed by removing the "!" (not) in 
# front of the "Silent" argument in the option string
factory = TMVA.Factory( "TMVAClassification", outputFile,
		    "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" )
dataloader = TMVA.DataLoader("dataset")

factory.SetVerbose( True )

# If you wish to modify default settings 
# (please check "src/Config.h" to see all available global options)
#    gConfig().GetVariablePlotting()).fTimesRMS = 8.0
#    gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory"

# Define the input variables that shall be used for the classifier training
# note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
# [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
dataloader.AddVariable( "l1Pt_1", 'F' )
dataloader.AddVariable( "l1Pt_2", 'F' )
dataloader.AddVariable( "l1DeltaEta", 'F' )
dataloader.AddVariable( "l1DeltaPhi", 'F' )
dataloader.AddVariable( "l1Mass",'F')



# Global event weights (see below for setting event-wise weights)
signalWeight     = 1.0
backgroundWeight = 1.0

# ====== register trees ====================================================
#
# the following method is the prefered one:
# you can add an arbitrary number of signal or background trees
dataloader.AddSignalTree    ( signal,     signalWeight     )
dataloader.AddBackgroundTree( background, backgroundWeight )

#dataloader.SetBackgroundWeightExpression( "weight" )

# Apply additional cuts on the signal and background sample. 
# example for cut: mycut = TCut( "abs(var1)<0.5 && abs(var2-0.5)<1" )
mycutSig = TCut( "l1Pt_1 > 0 && l1Pt_2 > 0 && l1Mass > 0" )
mycutBkg = TCut( "l1Pt_1 > 0 && l1Pt_2 > 0 && l1Mass > 0" )

# Here, the relevant variables are copied over in new, slim trees that are
# used for TMVA training and testing
# "SplitMode=Random" means that the input events are randomly shuffled before
# splitting them into training and test samples
dataloader.PrepareTrainingAndTestTree( mycutSig, mycutBkg,
				"nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" )
#factory.BookMethod(dataloader, TMVA.Types.kBDT, "BDT","!H:!V:NTrees=10:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" )
factory.BookMethod(dataloader, TMVA.Types.kBDT, "BDT", "!H:!V:NTrees=2:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5")
# --------------------------------------------------------------------------------------------------

# ---- Now you can tell the factory to train, test, and evaluate the MVAs. 

# Train MVAs
factory.TrainAllMethods()

# Test MVAs
factory.TestAllMethods()

# Evaluate MVAs
factory.EvaluateAllMethods()

# Save the output.
outputFile.Close()

print("=== wrote root file %s\n" % outputFile)
print("=== TMVAClassification is done!\n")

# open the GUI for the result macros    
#gROOT.ProcessLine( "TMVAGui(\"%s\")" % outputFile )

# keep the ROOT thread running
#gApplication.Run()

# ------------------------------------------------------
#import imp

#foo = imp.load_source('conifer', '/afs/cern.ch/work/a/addropul/hls4ml/conifer/conifer/__init__.py')
#foo.MyClass()

# Create a conifer config
cfg = conifer.backends.vivadohls.auto_config()
# Set the output directory to something unique
cfg['OutputDir'] = 'prj_{}'.format(int(0504215))

# Create and compile the model
model = conifer.model(ET.parse('./dataset/weights/TMVAClassification_BDT.weights.xml'), conifer.converters.tmva, conifer.backends.vivadohls, cfg)
model.compile()

#need to get input variables into the proper form, numpy array
X = np.load('/afs/cern.ch/user/a/addropul/CMSSW_10_6_0_pre4/src/L1Trigger/Run3Ntuplizer/test/June_July_2019/x_vbf072020_zb072020.npy')
print(np.shape(X))
# Run HLS C Simulation and get the output
y_hls = model.decision_function(X)
#y_skl = clf.decision_function(X)

# Synthesize the model
model.build()



