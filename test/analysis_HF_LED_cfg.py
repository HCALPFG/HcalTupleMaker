#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

#------------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------------

options = VarParsing.VarParsing()

options.register('skipEvents',
                 0, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to skip")

options.register('processEvents',
                 -1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to process")

options.register('inputFiles',
                 "root://cmsxrootd-site.fnal.gov//store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/150/00000/1A844C25-14E5-E511-B9B5-02163E013471.root",
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

options.register('outputFile',
                 "file:HCAL_output.root", #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.register('globalTag',
    '80X_dataRun2_Prompt_v2',
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.string,
    "Global Tag")



options.parseArguments()

print "Skip events =", options.skipEvents
print "Process events =", options.processEvents
print "inputFiles =", options.inputFiles
print "outputFile =", options.outputFile
print "Global Tag =", options.globalTag

#------------------------------------------------------------------------------------
# Declare the process
#------------------------------------------------------------------------------------

process = cms.Process("ANA")

#------------------------------------------------------------------------------------
# Is this a local run? Or a global run?
#------------------------------------------------------------------------------------

isLocalRun = False
isGlobalRun = not isLocalRun 

#------------------------------------------------------------------------------------
# Set up the input source, depending on whether this is a local or global run
#------------------------------------------------------------------------------------

if isLocalRun: 
    process.source = cms.Source("HcalTBSource",
       quiet = cms.untracked.bool ( False ),
       streams = cms.untracked.vstring(
           "HCAL_Trigger","HCAL_SlowData","HCAL_QADCTDC",
           "HCAL_DCC700","HCAL_DCC701","HCAL_DCC702","HCAL_DCC703","HCAL_DCC704","HCAL_DCC705",
           "HCAL_DCC706","HCAL_DCC707","HCAL_DCC708","HCAL_DCC709","HCAL_DCC710","HCAL_DCC711",
           "HCAL_DCC712","HCAL_DCC713","HCAL_DCC714","HCAL_DCC715","HCAL_DCC716","HCAL_DCC717",
           "HCAL_DCC718","HCAL_DCC719","HCAL_DCC720","HCAL_DCC721","HCAL_DCC722","HCAL_DCC723",
           "HCAL_DCC724","HCAL_DCC725","HCAL_DCC726","HCAL_DCC727","HCAL_DCC728","HCAL_DCC729",
           "HCAL_DCC730","HCAL_DCC731"
       ),
       fileNames = cms.untracked.vstring(options.inputFiles),
    )

if isGlobalRun:
    process.source = cms.Source("PoolSource",
      fileNames = cms.untracked.vstring(options.inputFiles),
      skipEvents = cms.untracked.uint32(options.skipEvents),
    )

#------------------------------------------------------------------------------------
# What files should we run over?
#------------------------------------------------------------------------------------

#process.source.fileNames = cms.untracked.vstring(
    # Example local run:
    #"root://eoscms//eos/cms/store/group/comm_hcal/LS1/USC_228119.root"
    # Example global run:
    # "root://eoscms//eos/cms/store/data/Commissioning2014/MinimumBias/RAW/v3/000/225/826/00000/DEE3076C-9C33-E411-90F0-02163E00F114.root" 
#)

#------------------------------------------------------------------------------------
# How many events should we run over?
#------------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(
       options.processEvents
   )
)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

#process.TFileService = cms.Service("TFileService",
#    fileName = cms.string( 'HCAL_output.root' )
#)
process.TFileService = cms.Service("TFileService", fileName = cms.string( options.outputFile ))

#------------------------------------------------------------------------------------
# Various python configuration files
#------------------------------------------------------------------------------------

# Need to set up MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# Need to set up the global tag
# Which to use?  https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V46::All')
process.GlobalTag = GlobalTag(process.GlobalTag, options.globalTag)

# Need to unpack digis from RAW
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")

# Aleko's EMAP
process.es_ascii = cms.ESSource('HcalTextCalibrations',
    input = cms.VPSet(
        cms.PSet(
            object = cms.string('ElectronicsMap'),
            file = cms.FileInPath('HCALPFG/HcalTupleMaker/data/2016-feb-24/version_G_emap_all.txt')
            ),
        )
    )
process.es_prefer = cms.ESPrefer('HcalTextCalibrations','es_ascii')

# Need the topology to analyze digis
process.load("Geometry.HcalCommonData.hcalDBConstants_cff")
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
        #'keep *_hcalTupleHBHEDigis_*_*',
        'keep *_hcalTupleHODigis_*_*',
        'keep *_hcalTupleHFDigis_*_*',
        #'keep *_hcalTupleHBHERecHits_*_*',
        'keep *_hcalTupleHORecHits_*_*',
        'keep *_hcalTupleHFRecHits_*_*'
    )
)

#------------------------------------------------------------------------------------
# If this is a local run, make sure we're looking for the FEDs in the right place
#------------------------------------------------------------------------------------

if isLocalRun:
    process.hcalDigis.InputLabel = cms.InputTag("source")
    process.hcalTupleFEDs.source = cms.untracked.InputTag("source")

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
    #process.hcalTupleHBHEDigis*
    # Make HCAL tuples: reco info
    process.hcalTupleHFRecHits*
    #process.hcalTupleHBHERecHits*
    # Package everything into a tree
    process.hcalTupleTree
)

#------------------------------------------------------------------------------------
# Make a schedule and run
#------------------------------------------------------------------------------------

process.schedule = cms.Schedule(process.p)

