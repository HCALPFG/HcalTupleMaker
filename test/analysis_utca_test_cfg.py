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

options.register('doReco',
                 False, # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Run HCAL reconstruction algo?")

options.parseArguments()

print "Skip events =", options.skipEvents
print "Process events =", options.processEvents
print "inputFiles =", options.inputFiles
print "outputFile =", options.outputFile
print "doReco =", options.doReco

#------------------------------------------------------------------------------------
# Declare the process
#------------------------------------------------------------------------------------

process = cms.Process("ANA")


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
# Various python configuration files
#------------------------------------------------------------------------------------

# Need to set up MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)

# Need to set up the global tag
# Which to use?  https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V49', '')

# Need the topology to unpack
process.load("Geometry.HcalEventSetup.HcalTopology_cfi")

# Make trigger tower geometry
process.HcalTrigTowerGeometryESProducer = cms.ESProducer("HcalTrigTowerGeometryESProducer")

# Need to unpack digis from RAW
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")

# Use an emap that has the uTCA
process.es_ascii = cms.ESSource('HcalTextCalibrations',
    input = cms.VPSet(
        cms.PSet(
            object = cms.string('ElectronicsMap'),
            file = cms.FileInPath('version_E_emap_rctonly_jm_12nov.txt')
            ),
        )
    )
process.es_prefer = cms.ESPrefer('HcalTextCalibrations','es_ascii')


# Set up utcaDigis unpacker
process.utcaDigis = process.hcalDigis.clone()
process.utcaDigis.FEDs = cms.untracked.vint32( 1118, 1120, 1122 ) 

# for FED in range ( 718, 724 ):
#     process.utcaDigis.FEDs.append ( FED ) 


print process.utcaDigis.FEDs

# Need the geometry to get digi and rechit positions
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

# Set up our analyzer
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")

# Modify hfdigis tuple maker for this specific test
process.hcalTupleHFUTCADigis = process.hcalTupleHFDigis.clone()
process.hcalTupleHFUTCADigis.Prefix =  cms.untracked.string("HFUTCA")
process.hcalTupleHFUTCADigis.source = cms.untracked.InputTag("utcaDigis")
process.hcalTupleHFUTCADigis.DoEnergyReco = cms.untracked.bool ( False ) 

process.hcalTupleHFDigis.DoEnergyReco = cms.untracked.bool ( False ) 


# Modify TP tuple maker for this specific test
process.hcalTupleTriggerPrimitives.source = cms.untracked.InputTag("utcaDigis")
process.hcalTupleTriggerPrimitives.hfDigis = cms.untracked.InputTag("utcaDigis")
process.hcalTupleTriggerPrimitives.hbheDigis = cms.untracked.InputTag("utcaDigis")

# Modify unpacker report tuple maker for this specific test
process.hcalTupleUnpackReport.source = cms.untracked.InputTag("utcaDigis")


# Make a path 
process.p = cms.Path(
    process.utcaDigis*
    process.hcalDigis*
    process.hcalTupleEvent*
    process.hcalTupleHFDigis*
    process.hcalTupleHFUTCADigis*
    process.hcalTupleTriggerPrimitives*
    process.hcalTupleUnpackReport*
    process.hcalTupleTree
)

# Make an endpath
process.output = cms.OutputModule("PoolOutputModule",
     fileName = cms.untracked.string('dump.root'),
     outputCommands = cms.untracked.vstring("keep *", "drop *_hcalTuple*_*_*" )
)
process.outputPath = cms.EndPath(process.output)


