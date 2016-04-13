#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------

import FWCore.ParameterSet.Config as cms

#------------------------------------------------------------------------------------
# Declare the process
#------------------------------------------------------------------------------------

process = cms.Process('RECO')

#------------------------------------------------------------------------------------
# Set up the input source
#------------------------------------------------------------------------------------

process.source = cms.Source("PoolSource")

#------------------------------------------------------------------------------------
# What files should we run over?
#------------------------------------------------------------------------------------

#fileNames = cms.untracked.vstring('root://xrootd.unl.edu//store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/000DA20F-3775-E311-B89A-003048C692E0.root')
process.source.fileNames = cms.untracked.vstring(
    #FILENAMES
    "root://xrootd.unl.edu//store/mc/Fall13dr/DYJetsToLL_M-50_13TeV-pythia6/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/000DA20F-3775-E311-B89A-003048C692E0.root"
    #"root://eoscms//eos/cms/store/mc/Fall13dr/QCD_Pt-300to470_Tune4C_13TeV_pythia8/GEN-SIM-RAW/castor_tsg_PU40bx25_POSTLS162_V2-v1/00000/FE19AB09-8BA4-E311-89F2-0025905A60BC.root"
    #"root://eoscms//eos/cms/store/mc/Spring14dr/QCD_Pt-30to50_Tune4C_13TeV_pythia8/GEN-SIM-RAW/castor_Flat0to10_POSTLS170_V5-v1/00000/FC765A4A-6F0B-E411-B606-02163E008CDD.root"
)

process.source.skipEvents = cms.untracked.uint32(0
    #SKIPEVENTS
)

#------------------------------------------------------------------------------------
# How many events should we run over?
#------------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100
        #PROCESSEVENTS
    )
)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('file:/tmp/hsaka/test.root')
)

#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
#process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
#process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Set up cosmic digis
#process.load("HCALPFG.HcalTupleMaker.HcalCosmicDigisProducer_cfi")

# Set up L1 Jet digis
#process.load("HCALPFG.HcalTupleMaker.HcalL1JetDigisProducer_cfi")

# Set up our analyzer
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Tree_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Event_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHEDigis_cfi")
process.hcalTupleHBHEDigis.recHits        = cms.untracked.InputTag("hbherecoMethod2")#this is used to store rechit time and energy in digi analysis
process.hcalTupleHBHECosmicsDigis.recHits = cms.untracked.InputTag("hbherecoMethod2")#this is used to store rechit time and energy in digi analysis
process.hcalTupleHBHEL1JetsDigis.recHits  = cms.untracked.InputTag("hbherecoMethod2")#this is used to store rechit time and energy in digi analysis
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHERecHits_CollisionData_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHERecHits_CollisionMC_cfi")
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Trigger_cfi")

# Set up noise filters: Method 2 (default) and Method 0
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalNoiseFilters_cfi")

# Set up noise filters: Method 2 (default) and Method 0
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_CaloJetMet_cfi")


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.AlCa.autoCond import autoCond
#process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_73_V1A::All', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'POSTLS162_V2::All', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'PHYS14_25_V1::All', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'POSTLS170_V5::All', '')
process.GlobalTag.globaltag = autoCond['run2_mc']

# ECAL problem specific lines: 
# https://hypernews.cern.ch/HyperNews/CMS/get/recoDevelopment/1334/2.html
# https://hypernews.cern.ch/HyperNews/CMS/get/recoDevelopment/1325/1/1/2.html
process.particleFlowClusterECAL.energyCorrector.autoDetectBunchSpacing = False
process.particleFlowClusterECAL.energyCorrector.bunchSpacing = cms.int32(25)
process.ecalMultiFitUncalibRecHit.activeBXs = cms.vint32(-5,-4,-3,-2,-1,0,1,2,3,4)
process.ecalMultiFitUncalibRecHit.useLumiInfoRunHeader = False

# Options and Output Report
#process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
#process.MessageLogger = cms.Service("MessageLogger",
# cout = cms.untracked.PSet(
#   default = cms.untracked.PSet( ## kill all messages in the log
#   limit = cms.untracked.int32(-1)
#  ),
#  FwkJob = cms.untracked.PSet( ## but FwkJob category - those unlimited
#   limit = cms.untracked.int32(-1)
#  ),
#  FwkReport = cms.untracked.PSet(
#   reportEvery = cms.untracked.int32(1), ## print event record number
#   limit = cms.untracked.int32(-1)
#  ),
#  FwkSummary = cms.untracked.PSet(
#    optionalPSet = cms.untracked.bool(True),
#  #  reportEvery = cms.untracked.int32(1),
#  #  limit = cms.untracked.int32(10000000)
#  )
# ),
# categories = cms.untracked.vstring('FwkJob','FwkReport','FwkSummary'),
# destinations = cms.untracked.vstring('cout')
#)

    
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    # Make HCAL tuples: FED info
    #
    #    process.hcalTupleFEDs*
    #    # Make HCAL tuples: digi info
    process.hcalTupleHBHEDigis*
    #    process.hcalTupleHODigis*
    #    process.hcalTupleHFDigis*
    #    # Make HCAL tuples: digi info
    #    process.hcalTupleHBHECosmicsDigis*
    #    process.hcalTupleHOCosmicsDigis*
    #    # Make HCAL tuples: digi info
    #    process.hcalTupleHBHEL1JetsDigis*
    #    process.hcalTupleHFL1JetsDigis*
    #    # Make HCAL tuples: reco info
    process.hcalTupleHBHERecHits*
    process.hcalTupleHcalNoiseFilters*
    process.hcalTupleCaloJetMet*
    #
    process.hcalTupleHBHERecHitsMethod0*
    process.hcalTupleHcalNoiseFiltersMethod0*
    process.hcalTupleCaloJetMetMethod0*
    #    process.hcalTupleHORecHits*
    #    process.hcalTupleHFRecHits*
    #    # L1 jet info
    #    process.hcalTupleL1Jets*
    #    # Make HCAL tuples: cosmic muon info
    #    process.hcalTupleCosmicMuons*
    #    # Package everything into a tree
    #
    process.hcalTupleTree
)



# Path and EndPath definitions
process.preparation = cms.Path(
    #process.my_hlt *
    process.RawToDigi *
    #process.L1Reco *
    process.reconstruction *
    #process.particleFlowClusterECAL*
    #process. caloglobalreco *
    #process.reconstructionCosmics *
    #
    #process.horeco *
    #process.hfreco *
    #
    process.hbheprerecoMethod0 *
    process.hbheprerecoMethod2 *
    process.hbherecoMethod0 *
    process.hbherecoMethod2 *
    #
    process.towerMakerMethod0 *
    process.towerMakerMethod2 *
    #
    process.hcalnoiseMethod0 *
    process.hcalnoiseMethod2 *
    #
    process.HBHENoiseFilterResultProducerMethod0 *
    process.HBHENoiseFilterResultProducerMethod2 *
    #
    #process.hcalCosmicDigis *
    #process.hcalL1JetDigis *
    process.tuple_step
)

#from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1
#process = customisePostLS1(process)
