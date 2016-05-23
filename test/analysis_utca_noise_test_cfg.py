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
                 "root://cmsxrootd-site.fnal.gov//store/data/Commissioning2016/MinimumBias/RAW/v1/000/264/232/00000/1EDB8470-71D1-E511-A299-02163E014504.root", #default value, corresponds to LFN /store/data/Commissioning2016/MinimumBias/RAW/v1/000/264/232/00000/1EDB8470-71D1-E511-A299-02163E014504.root from run 267594
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

options.register('outputFile',
                 "file:/uscms/home/dryu/HCAL/data/HCALPFG/utca_noise_test/hcalTupleTree_267594.root", #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.register('doReco',
                 False, # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Run HCAL reconstruction algo?")
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
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# Need to set up the global tag
# Which to use?  https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V49', '')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V46::All')
process.GlobalTag = GlobalTag(process.GlobalTag, options.globalTag)

# Need the topology to unpack/analyze digis
process.load("Geometry.HcalCommonData.hcalDBConstants_cff")
process.load("Geometry.HcalEventSetup.HcalTopology_cfi")

# Make trigger tower geometry
process.HcalTrigTowerGeometryESProducer = cms.ESProducer("HcalTrigTowerGeometryESProducer")

# Need to unpack digis from RAW
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")

# Use an emap that has the uTCA (now from the database)

#from CalibCalorimetry.HcalPlugins.Hcal_FrontierConditions_cff import es_pool

#process.my_es_pool = es_pool
#process.my_es_pool.connect = cms.string("frontier://FrontierPrep/CMS_COND_HCAL")
#process.my_es_pool.toGet = cms.VPSet(
#    cms.PSet(record = cms.string("HcalElectronicsMapRcd"),
#             tag = cms.string("HcalElectronicsMap_v7.06_hlt_test")
#    )
#)
#process.my_es_prefer = cms.ESPrefer('PoolDBESSource','my_es_pool')
process.es_ascii = cms.ESSource('HcalTextCalibrations',
    input = cms.VPSet(
        cms.PSet(
            object = cms.string('ElectronicsMap'),
            file = cms.FileInPath('HCALPFG/HcalTupleMaker/data/2016-feb-24/version_G_emap_all.txt')
            ),
        )
    )
process.es_prefer = cms.ESPrefer('HcalTextCalibrations','es_ascii')


# Set up utcaDigis unpacker
process.hcalDigis.FilterDataQuality = cms.bool(False)
process.hcalDigis.FEDs = cms.untracked.vint32()
#for FED in [x+700 for x in range(32)] + [1118, 1120, 1122]:
for FED in xrange(1100, 1124, 2):
    process.hcalDigis.FEDs.append ( FED ) 

# Need the geometry to get digi and rechit positions
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

# Set up our analyzer
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")

# Modify hbhedigis tuple maker for this specific test
process.hcalTupleHBHEDigis.DoEnergyReco = cms.untracked.bool ( False ) 

# HF digis
process.hcalTupleHFDigis.DoEnergyReco = cms.untracked.bool ( False ) 

# Make a path 
process.p = cms.Path(
    process.hcalDigis*
    process.hcalTupleEvent*
    process.hcalTupleHBHEDigis*
    process.hcalTupleHFDigis*
    process.hcalTupleUnpackReport*
    process.hcalTupleTree
)
