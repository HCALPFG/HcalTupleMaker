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
#    eras.run2_HCAL_2017, 
#    eras.run2_HF_2017,
#    eras.run2_HEPlan1_2017
    eras.Run2_2018
)

options = VarParsing.VarParsing ('analysis')
options.register ('skipEvents', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "no of skipped events")
options.outputFile = 'results.root'
options.maxEvents = 500 #-1 # means all events


#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
options.parseArguments()
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.source = cms.Source("PoolSource",
    fileNames  = cms.untracked.vstring(
   #"file:/eos/cms/store/relval/CMSSW_10_0_0/RelValQCD_FlatPt_15_3000HS_13/GEN-SIM-DIGI-RAW/100X_upgrade2018_realistic_v6_mahiON-v1/10000/C6785BE4-BAFF-E711-8D93-0CC47A78A42E.root"
   #"/store/relval/CMSSW_10_0_0/RelValQCD_FlatPt_15_3000HS_13/GEN-SIM-DIGI-RAW/PU25ns_100X_upgrade2018_realistic_v6_mahiON-v1/10000/EE4959BA-6A00-E811-A196-0025905A609E.root"
   "file:/home/users/jaehyeok/scratch/EE4959BA-6A00-E811-A196-0025905A609E.root"
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

#https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions#Global_Tags_for_PdmVMCcampaignRu
#http://cmslxr.fnal.gov/source/Configuration/AlCa/python/autoCond.py#0053
process.GlobalTag.globaltag = '100X_upgrade2018_realistic_v6' 

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
process.hcalTupleHcalNoiseFilters = cms.EDProducer("HcalTupleMaker_HcalNoiseFilters",
         noiseSummaryInputTag = cms.untracked.InputTag("hcalnoise"),
         noiseResultInputTag  = cms.untracked.string("HBHENoiseFilterResultProducer"),
         recoInputTag         = cms.untracked.string("hbheplan1"),#"hbhereco"),
         recoHFInputTag       = cms.untracked.string("hfreco"),
         recoVertexInputTag   = cms.untracked.string("offlinePrimaryVertices"),
         isRAW  = cms.untracked.bool(True), 
         isRECO = cms.untracked.bool(False), 
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)

# To apply filter decision in CMSSW as an EDFilter:
process.hcalnoise.fillCaloTowers = cms.bool(False)
process.hcalnoise.fillTracks = cms.bool(False)
process.hcalnoise.recHitCollName = cms.string("hbheplan1")
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter("BooleanFlagFilter",
    inputLabel = cms.InputTag("HBHENoiseFilterResultProducer","HBHENoiseFilterResult"),
    reverseDecision = cms.bool(False)
)

# Use latest pulse shape
#process.load("CondCore.DBCommon.CondDBSetup_cfi")
#process.es_pool = cms.ESSource("PoolDBESSource",
#    process.CondDBSetup,
#    timetype = cms.string('runnumber'),
#    toGet = cms.VPSet(
#      cms.PSet(record = cms.string("HcalRecoParamsRcd"),
#        tag = cms.string("HcalRecoParams_HEP17shape207")
#        )
#      ),
#    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
#    authenticationMethod = cms.untracked.uint32(0)
#    )
#process.es_prefer_es_pool = cms.ESPrefer( "PoolDBESSource", "es_pool" )

# Remove HFDigiTime SevLevel calculation because it does not exist in phase1 flags
import RecoLocalCalo.HcalRecAlgos.RemoveAddSevLevel as HcalRemoveAddSevLevel
HcalRemoveAddSevLevel.RemoveFlag(process.hcalRecAlgos,"HFDigiTime")

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

    process.hcalnoise *
    process.HBHENoiseFilterResultProducer* 
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
