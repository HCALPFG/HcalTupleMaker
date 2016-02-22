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
                 0, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to process")

options.register('inputFiles',
                 "file:inputFile.root", #default value
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

options.register('outputFile',
                 "file:outputFile.root", #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.register('globalTag',
		'GR_H_V58C',
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

process = cms.Process('PFG')

#------------------------------------------------------------------------------------
# Set up the input source
#------------------------------------------------------------------------------------

process.source = cms.Source("PoolSource")

#------------------------------------------------------------------------------------
# What files should we run over?
#------------------------------------------------------------------------------------

process.source = cms.Source("PoolSource", 
   fileNames = cms.untracked.vstring(
       options.inputFiles
   ),
   skipEvents = cms.untracked.uint32(
       options.skipEvents
   )
)

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

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string( options.outputFile )
)

#------------------------------------------------------------------------------------
# Various python configuration files below
# 
# Used the following cmsDriver command:
# cmsDriver.py reco \
# -s RAW2DIGI,L1Reco,RECO \
# --conditions GR_P_V49 \
# --eventcontent RECO \
# --magField=38T_PostLS1 \
# --scenario=cosmics \
# --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 
# --filein=root://eoscms//eos/cms/store/data/Commissioning2014/Cosmics/RAW/v3/000/229/713/00000//0A23AFDB-5B6A-E411-A4D1-02163E010DDF.root \
# --no_exec
#------------------------------------------------------------------------------------

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('RecoMET.METProducers.hcalnoiseinfoproducer_cfi')
process.load("CommonTools.RecoAlgos.HBHENoiseFilter_cfi")
process.load("CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi")

#------------------------------------------------------------------------------------------------------------------------------------
# Global tag
#------------------------------------------------------------------------------------------------------------------------------------
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, options.globalTag, '')


#------------------------------------------------------------------------------------------------------------------------------------
# Configure Unpacking
#------------------------------------------------------------------------------------------------------------------------------------
from Configuration.StandardSequences.RawToDigi_Data_cff import hcalDigis,gtDigis 
process.CustomizedRawToDigi = cms.Sequence(
		gtDigis*
		hcalDigis
)

#------------------------------------------------------------------------------------------------------------------------------------
# Configure Reconstruction Method
#------------------------------------------------------------------------------------------------------------------------------------
process.hbheprereco.puCorrMethod = cms.int32(0)

#------------------------------------------------------------------------------------------------------------------------------------
# Create Noise Filter
#------------------------------------------------------------------------------------------------------------------------------------
process.hcalnoise.fillCaloTowers = cms.bool(False)
process.hcalnoise.fillTracks = cms.bool(False)
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
			inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
			reverseDecision = cms.bool(False)
			)

#------------------------------------------------------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------------------------------------------------------
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")

#------------------------------------------------------------------------------------------------------------------------------------
# Set up new HO emap
#------------------------------------------------------------------------------------------------------------------------------------
# New HO emap
process.es_pool = cms.ESSource("PoolDBESSource",
    process.CondDBSetup,
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string("HcalElectronicsMapRcd"),
            tag = cms.string("HcalElectronicsMap_v7.00_offline")
        )
    ),
    connect = cms.string('frontier://FrontierPrep/CMS_COND_HCAL'),
    authenticationMethod = cms.untracked.uint32(2)
)
process.es_prefer_es_pool = cms.ESPrefer( "PoolDBESSource", "es_pool" )



process.hcalTupleHBHEDigis.recHits = cms.untracked.InputTag("hbheprereco")
process.hcalnoise.recHitCollName = cms.string("hbheprereco")

#------------------------------------------------------------------------------------
# Define the tuple-making sequence
#------------------------------------------------------------------------------------
    
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    # Make HCAL tuples: FED info
    process.hcalTupleFEDs*
    # Make HCAL tuples: digi info
    process.hcalTupleHBHEDigis*
    process.hcalTupleHODigis*
    process.hcalTupleHFDigis*
    # process.hcalTupleTriggerPrimitives*
    # Trigger info
    process.hcalTupleTrigger*
    # process.hcalTupleTriggerObjects*
    # Package everything into a tree
    process.hcalTupleTree
)

#------------------------------------------------------------------------------------
# Define the path
#------------------------------------------------------------------------------------

# Path and EndPath definitions
process.preparation = cms.Path(
    process.CustomizedRawToDigi*
    # process.RawToDigi *
    process.hcalLocalRecoSequence *
    process.hcalnoise *
    process.HBHENoiseFilterResultProducer *
    process.ApplyBaselineHBHENoiseFilter* 
    process.tuple_step
)

