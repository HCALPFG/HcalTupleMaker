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

options.parseArguments()

print "Skip events =", options.skipEvents
print "Process events =", options.processEvents
print "inputFiles =", options.inputFiles
print "outputFile =", options.outputFile

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
# Set up the input source
#------------------------------------------------------------------------------------

if isLocalRun:
    process.source = cms.Source("HcalTBSource",
#       quiet = cms.untracked.bool ( False ),
#       streams = cms.untracked.vstring(
#           "HCAL_Trigger","HCAL_SlowData","HCAL_QADCTDC",
#           "HCAL_DCC700","HCAL_DCC701","HCAL_DCC702","HCAL_DCC703","HCAL_DCC704","HCAL_DCC705",
#           "HCAL_DCC706","HCAL_DCC707","HCAL_DCC708","HCAL_DCC709","HCAL_DCC710","HCAL_DCC711",
#           "HCAL_DCC712","HCAL_DCC713","HCAL_DCC714","HCAL_DCC715","HCAL_DCC716","HCAL_DCC717",
#           "HCAL_DCC718","HCAL_DCC719","HCAL_DCC720","HCAL_DCC721","HCAL_DCC722","HCAL_DCC723",
#           "HCAL_DCC724","HCAL_DCC725","HCAL_DCC726","HCAL_DCC727","HCAL_DCC728","HCAL_DCC729",
#           "HCAL_DCC730","HCAL_DCC731"
#   )
    )

if isGlobalRun:
    process.source = cms.Source("PoolSource")

#------------------------------------------------------------------------------------
# What files should we run over?
#------------------------------------------------------------------------------------

process.source.fileNames = cms.untracked.vstring(
    options.inputFiles
)

process.source.skipEvents = cms.untracked.uint32(
    options.skipEvents
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

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string( options.outputFile )
)

#------------------------------------------------------------------------------------
# Various python configuration files
#------------------------------------------------------------------------------------

# Need to set up MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)

# Need to set up the global tag
# Which to use?  https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V54', '')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_P_V53::All'

# Need to unpack digis from RAW
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")

# Need the topology to analyze digis
process.load("Geometry.HcalEventSetup.HcalTopology_cfi")

# Need the geometry to get digi and rechit positions
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

# Need to reconstruct RecHits
# process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.hbhereco = process.hbheprereco.clone()

# ESSources for reconstruction
process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")

# Need to look at laser digis
process.load("HCALPFG.HcalTupleMaker.HcalLaserDigiReco_cfi")

# Set up our analyzer
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")

#------------------------------------------------------------------------------------
# Since this is a local run, make sure we're looking for the FEDs in the right place
#------------------------------------------------------------------------------------

process.hcalLaserReco.fedRawDataCollectionTag = cms.InputTag("source")
process.hcalDigis.InputLabel = cms.InputTag("source")
process.hcalTupleFEDs.source = cms.untracked.InputTag("source")

# Global Tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_P_V50::All'

# for HF uTCA
mapEnable = True
if mapEnable:
	process.es_pool = cms.ESSource("PoolDBESSource",
		process.CondDBSetup,
		timetype = cms.string('runnumber'),
		toGet = cms.VPSet(
			cms.PSet(
				record	= cms.string("HcalElectronicsMapRcd"),
#				tag		= cms.string("HcalElectronicsMap_v7.05_hlt")
				tag		= cms.string("HcalElectronicsMap_v7.00_offline")
			)
		),
		connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
		authenticationMethod = cms.untracked.uint32(0)
	)
	process.es_prefer_es_pool = cms.ESPrefer("PoolDBESSource", "es_pool" )

#------------------------------------------------------------------------------------
# Define the tuple-making sequence
#------------------------------------------------------------------------------------
    
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    # Make HCAL tuples: FED info
    process.hcalTupleFEDs*
    # Make HCAL tuples: unpacker info
    process.hcalTupleUnpackReport*
    # Make HCAL tuples: digi info
    process.hcalTupleHBHEDigis*
    process.hcalTupleHODigis*
    process.hcalTupleHFDigis*
    # Make HCAL tuples: laser digi info
    #process.hcalTupleLaserDigis*
    # Make HCAL tuples: reco info
    process.hcalTupleHBHERecHits*
    process.hcalTupleHORecHits*
    process.hcalTupleHFRecHits*
    # Package everything into a tree
    process.hcalTupleTree
)

#------------------------------------------------------------------------------------
# Define the final path
#------------------------------------------------------------------------------------

process.p = cms.Path(
    # Unpack digis from RAW
    process.hcalDigis*
    # Unpack the HCAL laser digis
    #process.hcalLaserReco*
    # Do energy reconstruction
    process.hbhereco* 
    process.horeco* 
    process.hfreco* 
    # Make the ntuples
    process.tuple_step
)

#------------------------------------------------------------------------------------
# Make a schedule and run
#------------------------------------------------------------------------------------

process.schedule = cms.Schedule(process.p)

