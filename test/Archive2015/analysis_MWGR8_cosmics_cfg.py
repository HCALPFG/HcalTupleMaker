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
)

process.source.skipEvents = cms.untracked.uint32(
    #SKIPEVENTS
)

#------------------------------------------------------------------------------------
# How many events should we run over?
#------------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(
        #PROCESSEVENTS
    )
)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.TFileService = cms.Service("TFileService",
    fileName = cms.string( 'OUTPUTFILENAME.root' )
)

#------------------------------------------------------------------------------------
# Various python configuration files
# Used cmsDriver.py reco -s RAW2DIGI,L1Reco,RECO --conditions STARTUP_V4::All --eventcontent RECO --magField=0T --scenario=cosmics
#------------------------------------------------------------------------------------

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentCosmics_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_0T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Set up cosmic digis
process.load("HCALPFG.HcalTupleMaker.HcalCosmicDigisProducer_cfi")

# Set up our analyzer
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")
process.hcalTupleTree = cms.EDAnalyzer("HcalTupleMaker_Tree",
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_hcalTupleEvent_*_*',
        'keep *_hcalTupleFEDs_*_*',
        'keep *_hcalTupleHBHEDigis_*_*',
        'keep *_hcalTupleHODigis_*_*',
        'keep *_hcalTupleHBHECosmicsDigis_*_*',
        'keep *_hcalTupleHOCosmicsDigis_*_*',
        'keep *_hcalTupleHFDigis_*_*',
        'keep *_hcalTupleHBHERecHits_*_*',
        'keep *_hcalTupleHORecHits_*_*',
        'keep *_hcalTupleHFRecHits_*_*',
        'keep *_hcalTupleTrigger_*_*',
        'keep *_hcalTupleTriggerObjects_*_*',
        'keep *_hcalTupleCosmicMuons_*_*'
    )
)             

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V47::All', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstructionCosmics)
process.hcalCosmicDigis_step = cms.Path(process.hcalCosmicDigis)
process.tuple_step = cms.Path(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    # Make HCAL tuples: FED info
    process.hcalTupleFEDs*
    # Make HCAL tuples: digi info
    process.hcalTupleHBHEDigis*
    process.hcalTupleHODigis*
    process.hcalTupleHFDigis*
    # Make HCAL tuples: digi info
    process.hcalTupleHBHECosmicsDigis*
    process.hcalTupleHOCosmicsDigis*
    # Make HCAL tuples: reco info
    process.hcalTupleHBHERecHits*
    process.hcalTupleHORecHits*
    process.hcalTupleHFRecHits*
    # Trigger info
    process.hcalTupleTrigger*
    process.hcalTupleTriggerObjects*
    # Make HCAL tuples: cosmic muon info
    process.hcalTupleCosmicMuons*
    # Package everything into a tree
    process.hcalTupleTree
)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,
                                process.reconstruction_step,
                                process.hcalCosmicDigis_step,
                                process.tuple_step)

