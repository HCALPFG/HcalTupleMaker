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
                 0, # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to skip")

options.register('processEvents',
                 -1, # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to process")

options.register('inputFiles',
                 "file:/eos/cms/store/data/Commissioning2018/ZeroBias/RAW/v1/000/314/747/00000/A0BFA542-2544-E811-80B3-FA163ECF9759.root",
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

options.register('outputFile',
                 "/eos/cms/store/user/jaehyeok/HcalTupleMaker_314747.root", # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.parseArguments()

print " "
print "Using options:"
print " skipEvents    =", options.skipEvents
print " processEvents =", options.processEvents
print " inputFiles    =", options.inputFiles
print " outputFile    =", options.outputFile
print " "

#------------------------------------------------------------------------------------
# Declare the process and input variables
#------------------------------------------------------------------------------------
from Configuration.StandardSequences.Eras import eras
process = cms.Process('PFG',eras.Run2_2018)

#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.processEvents) )

process.source = cms.Source(
    "PoolSource",
    fileNames  = cms.untracked.vstring(options.inputFiles),
    #fileNames  = cms.untracked.vstring(
    #  "file:/home/users/jaehyeok/HCAL/ntupler/files/splashes2018/BeamSplash_run313574_beam1.root",
    #  "file:/home/users/jaehyeok/HCAL/ntupler/files/splashes2018/BeamSplash_run313574_beam2.root"
    #),
    skipEvents = cms.untracked.uint32(options.skipEvents)
    )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile)
    )

#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')

#process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")

#process.load('Configuration.StandardSequences.L1Reco_cff')
#process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
#process.load("RecoLocalCalo.Configuration.RecoLocalCalo_Cosmics_cff") # cosmics: M0 with sum of all TSs

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('RecoMET.METProducers.hcalnoiseinfoproducer_cfi')
process.load("CommonTools.RecoAlgos.HBHENoiseFilter_cfi")
process.load("CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi")
#process.load("CondCore.CondDB.CondDB_cfi")

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi") # loads all modules
process.hcalTupleHBHEDigis.DoEnergyReco = False
## set desired parameters, for example:
process.hcalTupleHFDigis.DoEnergyReco = False
process.hcalTupleHFDigis.FilterChannels = False

#------------------------------------------------------------------------------------
#process.hcalDigis.saveQIE10DataNSamples = cms.untracked.vint32( 6)
#process.hcalDigis.saveQIE10DataTags = cms.untracked.vstring( "MYDATA" )

#------------------------------------------------------------------------------------
# Specify Global Tag
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '101X_dataRun2_HLT_v7' #'100X_dataRun2_HLT_v3' #'101X_dataRun2_v8' #'101X_dataRun2_HLT_frozen_v6' # '101X_dataRun2_HLT_v7'
print "GlobalTag = ", str(process.GlobalTag.globaltag).split("'")[1]
print " "

#------------------------------------------------------------------------------------
# Create Noise Filter
#------------------------------------------------------------------------------------
# Could be out of date, commented out in the main sequence, need to be fixed
process.hcalnoise.fillCaloTowers = cms.bool(False)
process.hcalnoise.fillTracks = cms.bool(False)
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter(
    'BooleanFlagFilter',
    inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
    reverseDecision = cms.bool(False)
    )

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

## FIXME
process.hcalDigis.saveQIE10DataNSamples = cms.untracked.vint32(6)
process.hcalDigis.saveQIE10DataTags = cms.untracked.vstring( "MYDATA" )

#------------------------------------------------------------------------------------
# HcalTupleMaker sequence definition
#------------------------------------------------------------------------------------
process.tuple_step = cms.Sequence(
    ### Make HCAL tuples: Event info
    process.hcalTupleEvent*
    ### Make HCAL tuples: FED info
#    process.hcalTupleFEDs*
    ## Make HCAL tuples: digi info
#    process.hcalTupleHBHEDigis*  # for HB
#    process.hcalTupleHODigis*
#    process.hcalTupleQIE10Digis* # for HF
#    process.hcalTupleQIE11Digis* # for HE
    ### Make HCAL tuples: reco info
#    process.hcalTupleHBHERecHits*
    process.hcalTupleHFPhase1RecHits*
#    process.hcalTupleHFRecHits*
    #process.hcalTupleHORecHits*
    ### Noise filter
#    process.hcalTupleHcalNoiseFilters*
    ## Package everything into a tree
    process.hcalTupleTree
)

#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(
    ### Unpack digis from RAW
#    process.RawToDigi*
    process.hcalDigis*
    ### reconstruction
#    process.L1Reco*
#    process.reconstruction*
#    process.hcalLocalRecoSequence*
    ### Do energy reconstruction
    process.horeco*
    process.hfprereco*
    process.hfreco*
    process.hbheprereco*
    process.hbheplan1*
    ### For noise filter
    process.hcalnoise*
    process.HBHENoiseFilterResultProducer*
#    process.ApplyBaselineHBHENoiseFilter*
    ### Make the ntuples
    process.tuple_step
)
