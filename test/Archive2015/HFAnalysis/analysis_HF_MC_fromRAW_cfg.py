#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

#------------------------------------------------------------------------------------
# Declare the process
#------------------------------------------------------------------------------------

process = cms.Process("ANA")

#------------------------------------------------------------------------------------
# Set up the input source, depending on whether this is a local or global run
#------------------------------------------------------------------------------------

process.source = cms.Source("PoolSource")

#------------------------------------------------------------------------------------
# What files should we run over?
#------------------------------------------------------------------------------------

process.source.fileNames = cms.untracked.vstring(
    # File of your choice
    "file:/afs/cern.ch/user/o/odryha/public/test/raw.root"
)

#------------------------------------------------------------------------------------
# How many events should we run over?
#------------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.TFileService = cms.Service("TFileService",
    fileName = cms.string( 'HcalTupleMaker_output.root' )
)

#------------------------------------------------------------------------------------
# Various python configuration files
#------------------------------------------------------------------------------------

# Need to set up MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)

# Need to set up the global tag
# Which to use?  https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V46::All')

# Need to unpack digis from RAW
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")

# Need the topology to analyze digis
process.load("Geometry.HcalEventSetup.HcalTopology_cfi")

# Need the geometry to get digi and rechit positions
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

# Need to reconstruct RecHits
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_hbhe_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_ho_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_hf_cfi")

# ESSources for reconstruction
process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")

# Set up our analyzer
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")
process.hcalTupleTree = cms.EDAnalyzer("HcalTupleMaker_Tree",
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_hcalTupleEvent_*_*',
        'keep *_hcalTupleFEDs_*_*',
        'keep *_hcalTupleHBHEDigis_*_*',
        'keep *_hcalTupleHODigis_*_*',
        'keep *_hcalTupleHFDigis_*_*',
        'keep *_hcalTupleHBHERecHits_*_*',
        'keep *_hcalTupleHORecHits_*_*',
        'keep *_hcalTupleHFRecHits_*_*'
    )
)

#------------------------------------------------------------------------------------
# Define the final path
#------------------------------------------------------------------------------------

process.p = cms.Path(
    # Unpack digis from RAW
    process.hcalDigis*
    # Do energy reconstruction
    process.hfreco* 
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    # Make HCAL tuples: FED info
    process.hcalTupleFEDs*
    # Make HCAL tuples: digi info
    process.hcalTupleHFDigis*
    # Make HCAL tuples: reco info
    process.hcalTupleHFRecHits*
    # Package everything into a tree
    process.hcalTupleTree
)

#------------------------------------------------------------------------------------
# Make a schedule and run
#------------------------------------------------------------------------------------

process.schedule = cms.Schedule(process.p)

