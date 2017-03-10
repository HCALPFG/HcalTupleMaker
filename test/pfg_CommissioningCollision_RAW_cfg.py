#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import FWCore.ParameterSet.VarParsing as VarParsing


#------------------------------------------------------------------------------------
# Declare the process and input variables
#------------------------------------------------------------------------------------
process = cms.Process('PFG',eras.Run2_2016)
#inputFiles = "/store/express/Commissioning2016/ExpressPhysics/FEVT/Express-v1/000/268/958/00000/E249E6DE-ECFD-E511-8979-02163E012A42.root"
inputFiles = "/store/data/Run2016H/JetHT/RAW/v1/000/283/877/00000/5C38E8B6-ED9B-E611-A26D-FA163E1C48F0.root"
skipEvents = 0
processEvents = 10
outputFile = "results.root"

#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(processEvents) )
process.source = cms.Source("PoolSource",
    fileNames  = cms.untracked.vstring(inputFiles),
    skipEvents = cms.untracked.uint32(skipEvents)
)

#------------------------------------------------------------------------------------
# Define output file
#------------------------------------------------------------------------------------
process.TFileService = cms.Service("TFileService",fileName = cms.string(outputFile))

#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('RecoMET.METProducers.hcalnoiseinfoproducer_cfi')
process.load("CommonTools.RecoAlgos.HBHENoiseFilter_cfi")
process.load("CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi")

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi") # Dont want to use this, load modules individually
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Tree_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Event_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHEDigis_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HODigis_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HFDigis_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHERecHits_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HFRecHits_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_L1Jets_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalTriggerPrimitives_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalL1JetDigisProducer_cfi")
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Trigger_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_MuonTrack_cfi")
process.load("HLTrigger.HLTfilters.triggerResultsFilter_cfi")

from Configuration.StandardSequences.RawToDigi_Data_cff import *
process.CustomizedRawToDigi = cms.Sequence(
		gtDigis*
		hcalDigis
)

#------------------------------------------------------------------------------------
# Specify Global Tag
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '80X_dataRun2_HLT_v12'

#------------------------------------------------------------------------------------
# Configure modules
#------------------------------------------------------------------------------------
process.my_hlt = cms.EDFilter("HLTHighLevel",
     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
     HLTPaths = cms.vstring("HLT_L1SingleMuOpen_*"), # provide list of HLT paths (or patterns) you want
     eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
     andOr = cms.bool(True),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
     throw = cms.bool(False)    # throw exception on unknown path names
)

#------------------------------------------------------------------------------------------------------------------------------------
# Create Noise Filter
#------------------------------------------------------------------------------------------------------------------------------------
process.hcalnoise.fillCaloTowers = cms.bool(False)
process.hcalnoise.fillTracks = cms.bool(False)
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
			inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
			reverseDecision = cms.bool(False)
			)

#------------------------------------------------------------------------------------
# Configure sequence
#------------------------------------------------------------------------------------
process.hcalTupleHBHEDigis.recHits = cms.untracked.InputTag("hbheprereco")

#------------------------------------------------------------------------------------
# HcalTupleMaker sequence definition
#------------------------------------------------------------------------------------
process.tuple_step = cms.Sequence(
    process.hcalTupleEvent*
    process.hcalTupleHBHEDigis*
    process.hcalLocalRecoSequence *
    process.hcalTupleTree
)

#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(
    process.CustomizedRawToDigi * 
    process.hcalLocalRecoSequence *
    #process.hcalnoise *  #needed for RAW files
    #process.HBHENoiseFilterResultProducer *
    #process.ApplyBaselineHBHENoiseFilter *
    process.tuple_step
)
