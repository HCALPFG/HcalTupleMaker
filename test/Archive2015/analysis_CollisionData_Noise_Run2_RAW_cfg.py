#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------

import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

#------------------------------------------------------------------------------------
# Declare the process
#------------------------------------------------------------------------------------

process = cms.Process('RECO',eras.Run2_50ns)
#process = cms.Process('RECO',eras.Run2_25ns)

#------------------------------------------------------------------------------------
# Set up the input source
#------------------------------------------------------------------------------------

process.source = cms.Source("PoolSource")
#
# To be used if a specific set of events is to be analyzed: Feedback for MET scanners etc.
#
#process.source = cms.Source("PoolSource",
##################template# eventsToProcess = cms.untracked.VEventRange('RUN:EVENT-RUN:EVENT')
#                           eventsToProcess = cms.untracked.VEventRange('251561:65988922-251561:65988922')
#                           )


#------------------------------------------------------------------------------------
# What files should we run over?
#------------------------------------------------------------------------------------

process.source.fileNames = cms.untracked.vstring(
    #FILENAMES
    'root://xrootd.unl.edu//store/data/Run2015A/MET/RAW/v1/000/248/038/00000/DA2F1ED8-0513-E511-BBE5-02163E01451E.root'
    #"root://eoscms//eos/cms/store/data/Run2012D/MET/RAW/v1/000/208/487/02185AD3-F03D-E211-BF08-001D09F25479.root"
)

process.source.skipEvents = cms.untracked.uint32(0
    #SKIPEVENTS
)

#------------------------------------------------------------------------------------
# How many events should we run over?
#------------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10
        #PROCESSEVENTS
    )
)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('file:/tmp/hsaka/Test.root')
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
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

# Set up L1 Jet digis #Disabled 
#process.load("HCALPFG.HcalTupleMaker.HcalL1JetDigisProducer_cfi")

# Set up our analyzer
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi") # Dont want to use this, load modules individually
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Tree_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Event_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHEDigis_cfi")
process.hcalTupleHBHEDigis.recHits          = cms.untracked.InputTag("hbherecoMethod2")#this is used to store rechit time and energy in digi analysis
process.hcalTupleHBHECosmicsDigis.recHits   = cms.untracked.InputTag("hbherecoMethod2")#this is used to store rechit time and energy in digi analysis
process.hcalTupleHBHEL1JetsDigis.recHits    = cms.untracked.InputTag("hbherecoMethod2")#this is used to store rechit time and energy in digi analysis
process.hcalTupleHBHEDigis.TotalFCthreshold = cms.untracked.double(-9999)# per channel charge threshold to store corresponding digis
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHERecHits_CollisionData_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Trigger_cfi")

# Set up noise filters: Method 2 (default) and Method 0
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalNoiseFilters_cfi")

# Set up noise filters: Method 2 (default) and Method 0
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_CaloJetMet_cfi")


# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_75_V5A', '') # This GT includes NEF DB.
#from Configuration.AlCa.autoCond_condDBv2 import autoCond
#process.GlobalTag.globaltag = autoCond['run2_data']

#process.my_hlt = cms.EDFilter("HLTHighLevel",
#     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
#     HLTPaths = cms.vstring("HLT_L1SingleJet16*"), # provide list of HLT paths (or patterns) you want
#     eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
#     andOr = cms.bool(True),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
#     throw = cms.bool(False)    # throw exception on unknown path names
#)


process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
    #inputLabel = cms.InputTag('HBHENoiseFilterResultProducerMethod2','HBHENoiseFilterResult'),    
    inputLabel = cms.InputTag('HBHENoiseFilterResultProducerMethod2','HBHENoiseFilterResultRun1'), #CMSSW >=74 and >=75 has eraw() in noise anyway.
    #inputLabel = cms.InputTag('HBHENoiseFilterResultProducerMethod2','HBHENoiseFilterResultRun2Loose'),
    #inputLabel = cms.InputTag('HBHENoiseFilterResultProducerMethod2','HBHENoiseFilterResultRun2Tight'),
    reverseDecision = cms.bool(False)
)


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
    #    process.hcalTupleTriggerPrimitives*
    #    # Make HCAL tuples: digi info
    #    process.hcalTupleHBHECosmicsDigis*
    #    process.hcalTupleHOCosmicsDigis*
    #    # Make HCAL tuples: digi info
    #    process.hcalTupleHBHEL1JetsDigis*
    #    process.hcalTupleHFL1JetsDigis*
    #    process.hcalTupleL1JetTriggerPrimitives*
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
    #    # Trigger info
    #process.hcalTupleTrigger*
    
    #    process.hcalTupleTriggerObjects*
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
    #process.ApplyBaselineHBHENoiseFilter *
    #
    #process.hcalCosmicDigis *
    #process.hcalL1JetDigis *
    process.tuple_step
)

from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1
process = customisePostLS1(process)
