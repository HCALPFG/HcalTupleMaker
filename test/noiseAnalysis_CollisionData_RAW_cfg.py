#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import FWCore.ParameterSet.VarParsing as VarParsing
#import os

#------------------------------------------------------------------------------------
# Declare the process and input variables
#------------------------------------------------------------------------------------

from Configuration.StandardSequences.Eras import eras
process                 = cms.Process('NOISE', 
    eras.run2_HCAL_2017, 
    eras.run2_HF_2017,
    eras.run2_HEPlan1_2017
)

options = VarParsing.VarParsing ('analysis')
options.register ('skipEvents', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "no of skipped events")
options.outputFile = 'results.root'
options.maxEvents = -1 #-1 # means all events


#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
options.parseArguments()
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.source = cms.Source("PoolSource",
    fileNames  = cms.untracked.vstring(
#        "root://eoscms//eos/cms/store/data/Run2017A/HLTPhysics1/RAW/v1/000/295/606/00000/4CC8CCBD-3745-E711-9A4E-02163E01341F.root"
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_15.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_7.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_14.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_11.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_8.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_6.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_30.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_3.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_27.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_26.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_20.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_2.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_19.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_18.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_17.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_10.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_1.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_35.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_33.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_32.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_28.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_13.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_12.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_9.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_25.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_16.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_5.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_24.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_31.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_23.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_34.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_21.root",
#   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_4.root",
   "file:/afs/cern.ch/work/o/owen/public/trigger-2017/edm-files-june7-2017/raw-format/pickevents_22.root"
    ),

    skipEvents = cms.untracked.uint32(options.skipEvents) # default is 0.
)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string(options.outputFile)
)


#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.Services_cff')
#process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
#process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
#process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('RecoLocalCalo.Configuration.hcalLocalReco_cff')

process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

process.load("RecoMET.METProducers.hcalnoiseinfoproducer_cfi") 
process.load("CommonTools.RecoAlgos.HBHENoiseFilter_cfi")
process.load("CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi")

process.load('Configuration.StandardSequences.EndOfProcess_cff')

#------------------------------------------------------------------------------------
# Set up L1 Jet digis #Disabled 
#------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Tree_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Event_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHERecHits_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HFRecHits_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HFPhase1RecHits_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Trigger_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalNoiseFilters_cfi")

#------------------------------------------------------------------------------------
# Set up noise filters
#------------------------------------------------------------------------------------
#1#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalNoiseFilters_cfi") 
# This is over-ridden below to remove Method0-Method2 dual reco.
#------------------------------------------------------------------------------------
# Set up iso noise filter parameters, used for iso-noise filter study in 25ns.
#------------------------------------------------------------------------------------
#1#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalIsoNoiseFilterParameters_cfi")
#------------------------------------------------------------------------------------
# Set up CaloJetMet quantities 
#------------------------------------------------------------------------------------
#1#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_CaloJetMet_cfi") 
# This is over-ridden below to remove Method0-Method2 dual reco.
#------------------------------------------------------------------------------------
# Set up MuonTrack quantities 
#------------------------------------------------------------------------------------
#1#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_MuonTrack_cfi")

#------------------------------------------------------------------------------------
# Specify Global Tag
#------------------------------------------------------------------------------------

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

#from Configuration.AlCa.autoCond import autoCond
#process.GlobalTag.globaltag = autoCond['run2_data'] 
#process.GlobalTag.globaltag = autoCond['run2_mc']  

process.GlobalTag.globaltag = '92X_dataRun2_Prompt_v4'

#----------------------------------------------------- replacing conditions
#process.load("CondCore.CondDB.CondDB_cfi")
#
#process.GlobalTag.toGet = cms.VPSet(
#  cms.PSet(record = cms.string("HFPhase1PMTParamsRcd"),
#     tag = cms.string("HFPhase1PMTParams_test"),
#     #connect = cms.string("sqlite_file:/afs/cern.ch/user/t/toropin/CMSSW_9_0_0/src/CondTools/Hcal/test/HFPhase1PMTParams_test.db")
#     connect = cms.string("sqlite_file:HFPhase1PMTParams_test.db")
#
#     )
#)


#====================================================================================
#------------------------------------------------------------------------------------
#Remove Method 0, Rename Method 2 as "default" where necessary:
#------------------------------------------------------------------------------------
#1#process.hcalTupleCaloJetMet = cms.EDProducer("HcalTupleMaker_CaloJetMet",
#1#         recoInputTag         = cms.untracked.string("hbhereco"),
#1#         Prefix = cms.untracked.string(""),
#1#         Suffix = cms.untracked.string("")
#1#)
#1#process.hcalTupleHcalNoiseFilters = cms.EDProducer("HcalTupleMaker_HcalNoiseFilters",
#1#         noiseSummaryInputTag = cms.untracked.InputTag("hcalnoise"),
#1#         noiseResultInputTag  = cms.untracked.string("HBHENoiseFilterResultProducer"),
#1#         recoInputTag         = cms.untracked.string("hbhereco"),
#1#         isRAW  = cms.untracked.bool(False), # new Flag necessary for HcalNoiseFilters to run on RECO data
#1#         isRECO = cms.untracked.bool(True), 
#1#         Prefix = cms.untracked.string(""),
#1#         Suffix = cms.untracked.string("")
#1#)

# To apply filter decision in CMSSW as an EDFilter:
process.hcalnoise.fillCaloTowers = cms.bool(False)
process.hcalnoise.fillTracks = cms.bool(False)
process.hcalnoise.recHitCollName = cms.string("hbheplan1")
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter("BooleanFlagFilter",
    inputLabel = cms.InputTag("HBHENoiseFilterResultProducer","HBHENoiseFilterResult"),
    reverseDecision = cms.bool(False)
)

#------------------------------------------------------------------------------------
# HcalTupleMaker sequence definition
#------------------------------------------------------------------------------------
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    process.hcalTupleHBHERecHits*
    process.hcalTupleHFRecHits*
    process.hcalTupleHFPhase1RecHits*
    #process.hcalTupleHcalNoiseFilters*
    #1#process.hcalTupleHcalIsoNoiseFilterParameters* #for studying iso-noise-filter
    #1#process.hcalTupleCaloJetMet*
    #1#process.hcalTupleMuonTrack*
    # 
    process.hcalTupleTrigger*
    #
    process.hcalTupleTree
)

#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(

    process.hcalDigis *
    
    process.L1Reco *
    process.hfprereco *
    process.hfreco * 
    process.hbheprereco *
    process.hbheplan1 *
    
    #process.reconstruction * 

#    process.hcalnoise *
#    process.HBHENoiseFilterResultProducer *
#    process.ApplyBaselineHBHENoiseFilter *
    
    process.tuple_step
)
