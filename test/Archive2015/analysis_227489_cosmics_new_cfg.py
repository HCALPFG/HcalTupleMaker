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
    # "root://eoscms//eos/cms/store/data/Commissioning2014/Cosmics/RAW/v3/000/228/525/00000/5CB57BED-A45E-E411-B7DE-02163E00ECDE.root"
    # "root://eoscms//eos/cms/store/data/Commissioning2014/HcalHPDNoise/RAW/v3/000/228/525/00000/14016335-A45E-E411-8DED-02163E00ECE7.root"
    "root://eoscms//eos/cms/store/data/Commissioning2014/MinimumBias/RAW/v3/000/227/489/00000/08D96C2D-5855-E411-A100-02163E00F01E.root"
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
process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

# Set up cosmic digis
process.load("HCALPFG.HcalTupleMaker.HcalCosmicDigisProducer_cfi")

# Set up L1 Jet digis
process.load("HCALPFG.HcalTupleMaker.HcalL1JetDigisProducer_cfi")

# Set up our analyzer
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V47::All', '')

process.my_hlt = cms.EDFilter("HLTHighLevel",
     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
     HLTPaths = cms.vstring("HLT_L1SingleJet16*"), # provide list of HLT paths (or patterns) you want
     eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
     andOr = cms.bool(True),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
     throw = cms.bool(False)    # throw exception on unknown path names
)

    
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    # Make HCAL tuples: FED info
    process.hcalTupleFEDs*
    # Make HCAL tuples: digi info
    process.hcalTupleHBHEDigis*
    process.hcalTupleHODigis*
    process.hcalTupleHFDigis*
    process.hcalTupleTriggerPrimitives*
    # Make HCAL tuples: digi info
    process.hcalTupleHBHECosmicsDigis*
    process.hcalTupleHOCosmicsDigis*
    # Make HCAL tuples: digi info
    process.hcalTupleHBHEL1JetsDigis*
    process.hcalTupleHFL1JetsDigis*
    process.hcalTupleL1JetTriggerPrimitives*
    # Make HCAL tuples: reco info
    process.hcalTupleHBHERecHits*
    process.hcalTupleHORecHits*
    process.hcalTupleHFRecHits*
    # Trigger info
    process.hcalTupleTrigger*
    process.hcalTupleTriggerObjects*
    # L1 jet info
    process.hcalTupleL1Jets*
    # Make HCAL tuples: cosmic muon info
    process.hcalTupleCosmicMuons*
    # Package everything into a tree
    process.hcalTupleTree
)



# Path and EndPath definitions
process.preparation = cms.Path(
    process.my_hlt *
    process.RawToDigi *
    process.L1Reco *
    process.reconstructionCosmics *
    process.hcalCosmicDigis *
    process.hcalL1JetDigis *
    process.tuple_step
)
