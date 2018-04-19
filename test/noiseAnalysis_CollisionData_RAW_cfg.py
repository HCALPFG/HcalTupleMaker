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
options.outputFile = 'results_207.root'
options.maxEvents = 5000  #-1 # means all events

# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideEDMPathsAndTriggerBits#Summary_report
#process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
options.parseArguments()
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.source = cms.Source("PoolSource",
    fileNames  = cms.untracked.vstring(
   #"file:/eos/cms/store/user/jaehyeok/00056890-5BAE-E711-AD51-02163E014439.root"
   "/store/data/Run2017E/JetHT/RAW/v1/000/304/797/00000/00056890-5BAE-E711-AD51-02163E014439.root",
   "/store/data/Run2017E/JetHT/RAW/v1/000/304/797/00000/00094B71-75AE-E711-8DE3-02163E019B35.root", 
   "/store/data/Run2017E/JetHT/RAW/v1/000/304/797/00000/000B1159-61AE-E711-846E-02163E01A6D1.root", 
   "/store/data/Run2017E/JetHT/RAW/v1/000/304/797/00000/0016D67E-A7AE-E711-81D1-02163E01A644.root", 
   "/store/data/Run2017E/JetHT/RAW/v1/000/304/797/00000/00186C2C-62AE-E711-B0B6-02163E013522.root"
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
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
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

#process.GlobalTag.globaltag = '92X_dataRun2_Prompt_v9'
process.GlobalTag.globaltag = '100X_dataRun2_v1'

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

process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.es_pool = cms.ESSource("PoolDBESSource",
    process.CondDBSetup,
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
      cms.PSet(record = cms.string("HcalRecoParamsRcd"),
        tag = cms.string("HcalRecoParams_HEP17shape207")
        )
      ),
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
    authenticationMethod = cms.untracked.uint32(0)
    )
process.es_prefer_es_pool = cms.ESPrefer( "PoolDBESSource", "es_pool" )


#====================================================================================
#------------------------------------------------------------------------------------
#Remove Method 0, Rename Method 2 as "default" where necessary:
#------------------------------------------------------------------------------------
#1#process.hcalTupleCaloJetMet = cms.EDProducer("HcalTupleMaker_CaloJetMet",
#1#         recoInputTag         = cms.untracked.string("hbhereco"),
#1#         Prefix = cms.untracked.string(""),
#1#         Suffix = cms.untracked.string("")
#1#)
process.hcalTupleHcalNoiseFilters = cms.EDProducer("HcalTupleMaker_HcalNoiseFilters",
         noiseSummaryInputTag = cms.untracked.InputTag("hcalnoise"),
         noiseResultInputTag  = cms.untracked.string("HBHENoiseFilterResultProducer"),
         recoInputTag         = cms.untracked.string("hbheprereco"), #"hbheplan1"),#"hbhereco"),
         recoHFInputTag       = cms.untracked.string("hfreco"),
         recoVertexInputTag   = cms.untracked.string("offlinePrimaryVertices"),
         isRAW  = cms.untracked.bool(True), 
         isRECO = cms.untracked.bool(True), 
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)

# To apply filter decision in CMSSW as an EDFilter:
process.hcalnoise.fillCaloTowers = cms.bool(False)
process.hcalnoise.fillTracks = cms.bool(False)
process.hcalnoise.recHitCollName = cms.string("hbheprereco")#"hbheplan1")
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter("BooleanFlagFilter",
    inputLabel = cms.InputTag("HBHENoiseFilterResultProducer","HBHENoiseFilterResult"),
    reverseDecision = cms.bool(False)
)

# Remove HFDigiTime SevLevel calculation because it does not exist in phase1 flags
import RecoLocalCalo.HcalRecAlgos.RemoveAddSevLevel as HcalRemoveAddSevLevel
HcalRemoveAddSevLevel.RemoveFlag(process.hcalRecAlgos,"HFDigiTime")
#HcalRemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"HFSignalAsymmetry",8)

# do not apply TDC/Qasym cuts
process.hfreco.algorithm.rejectAllFailures = cms.bool(False)
process.hfreco.setNoiseFlags = cms.bool(True)

# HcalTupleMaker sequence definition
#------------------------------------------------------------------------------------
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    process.hcalTupleHBHERecHits*
    process.hcalTupleHFRecHits*
    process.hcalTupleHFPhase1RecHits*
    process.hcalTupleHcalNoiseFilters*
    #1#process.hcalTupleHcalIsoNoiseFilterParameters* #for studying iso-noise-filter
    #1#process.hcalTupleCaloJetMet*
    #1#process.hcalTupleMuonTrack*
    # 
    #process.hcalTupleTrigger*
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

    process.hcalnoise *
    process.HBHENoiseFilterResultProducer *
#    process.ApplyBaselineHBHENoiseFilter *
    
    process.tuple_step
) 

#dump = file('dump.py', 'w')
#dump.write( process.dumpPython() )
#dump.close()
process.FastTimerService = cms.Service("FastTimerService",
    dqmLumiSectionsRange = cms.untracked.uint32(2500),
    dqmMemoryRange = cms.untracked.double(1000000.0),
    dqmMemoryResolution = cms.untracked.double(5000.0),
    dqmModuleMemoryRange = cms.untracked.double(100000.0),
    dqmModuleMemoryResolution = cms.untracked.double(500.0),
    dqmModuleTimeRange = cms.untracked.double(40.0),
    dqmModuleTimeResolution = cms.untracked.double(0.2),
    dqmPath = cms.untracked.string('HLT/TimerService'),
    dqmPathMemoryRange = cms.untracked.double(1000000.0),
    dqmPathMemoryResolution = cms.untracked.double(5000.0),
    dqmPathTimeRange = cms.untracked.double(100.0),
    dqmPathTimeResolution = cms.untracked.double(0.5),
    dqmTimeRange = cms.untracked.double(2000.0),
    dqmTimeResolution = cms.untracked.double(5.0),
    enableDQM = cms.untracked.bool(True),
    enableDQMTransitions = cms.untracked.bool(False),
    enableDQMbyLumiSection = cms.untracked.bool(True),
    enableDQMbyModule = cms.untracked.bool(False),
    enableDQMbyPath = cms.untracked.bool(False),
    enableDQMbyProcesses = cms.untracked.bool(True),
    printEventSummary = cms.untracked.bool(False),
    printJobSummary = cms.untracked.bool(True),
    printRunSummary = cms.untracked.bool(True)
)
process.options = cms.untracked.PSet(
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    sizeOfStackForThreadsInKB = cms.untracked.uint32(10240),
    wantSummary = cms.untracked.bool(True)
)
