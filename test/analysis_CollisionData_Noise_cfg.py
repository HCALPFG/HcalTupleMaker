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

process.source.fileNames = cms.untracked.vstring(
    #FILENAMES
    'root://xrootd.unl.edu//store/data/Run2012D/MET/RAW/v1/000/208/913/2007AEB7-7244-E211-B95E-001D09F2983F.root'
    #"root://eoscms//eos/cms/store/data/Run2012D/MET/RAW/v1/000/208/487/02185AD3-F03D-E211-BF08-001D09F25479.root"
    #-rw-r--r--   2 phedex   zh         3967684833 Feb 13 03:06 02185AD3-F03D-E211-BF08-001D09F25479.root
    #-rw-r--r--   2 phedex   zh         3950736096 Feb 13 03:08 04C9E4B2-E73D-E211-8845-003048F11112.root
    #-rw-r--r--   2 phedex   zh         3886995990 Feb 13 03:06 0854F16B-DE3D-E211-BBCF-0025901D631E.root
    #-rw-r--r--   2 phedex   zh         4010155229 Feb 13 03:11 0C0FE569-D83D-E211-AF9E-BCAEC5329703.root
    #-rw-r--r--   2 phedex   zh         3863887005 Feb 13 03:09 18D5AD21-D63D-E211-80F1-001D09F24399.root
    #-rw-r--r--   2 phedex   zh         3922974702 Feb 13 03:11 1C18171D-E43D-E211-A08B-BCAEC518FF8F.root
    #-rw-r--r--   2 phedex   zh         3989679673 Feb 13 03:11 1CF426C7-EE3D-E211-8D07-003048D2C174.root
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

# Set up cosmic digis
#process.load("HCALPFG.HcalTupleMaker.HcalCosmicDigisProducer_cfi")

# Set up L1 Jet digis
#process.load("HCALPFG.HcalTupleMaker.HcalL1JetDigisProducer_cfi")

# Set up our analyzer
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")
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
from Configuration.AlCa.autoCond_condDBv2 import autoCond
process.GlobalTag.globaltag = autoCond['run1_data']

#process.my_hlt = cms.EDFilter("HLTHighLevel",
#     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
#     HLTPaths = cms.vstring("HLT_L1SingleJet16*"), # provide list of HLT paths (or patterns) you want
#     eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
#     andOr = cms.bool(True),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
#     throw = cms.bool(False)    # throw exception on unknown path names
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
    #process.hcalCosmicDigis *
    #process.hcalL1JetDigis *
    process.tuple_step
)

#from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1
#process = customisePostLS1(process)
