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
                 #"file:inputFile.root", #default value
    		 "file:input.root",
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

options.register('outputFile',
                 "file:outputFile_full.root", #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.register('globalTag',
		'74X_dataRun2_Express_v3',
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

process = cms.Process('RECO')

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
   ),
)

process.source.fileNames = cms.untracked.vstring(
    options.inputFiles
    # "root://eoscms//eos/cms/store/data/Commissioning2014/Cosmics/RAW/v3/000/228/525/00000/5CB57BED-A45E-E411-B7DE-02163E00ECDE.root"
    #"root://cmsxrootd.fnal.gov//store/data/Commissioning2016/Cosmics/RAW/v1/000/264/593/00000/F63DF1C1-40D2-E511-8435-02163E011FBB.root"
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
# Used cmsDriver.py reco -s RAW2DIGI,L1Reco,RECO --conditions STARTUP_V4::All --eventcontent RECO --magField=0T --scenario=cosmics
#------------------------------------------------------------------------------------

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('CondCore.CondDB.CondDB_cfi')
process.load('Configuration.EventContent.EventContentCosmics_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_0T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

#------------------------------------------------------------------------------------------------------------------------------------
# Configure Unpacking
#------------------------------------------------------------------------------------------------------------------------------------
from Configuration.StandardSequences.RawToDigi_Data_cff import *
process.CustomizedRawToDigi = cms.Sequence(
		gtDigis*
		siPixelDigis*
		siStripDigis*
		ecalDigis*
		ecalPreshowerDigis*
		hcalDigis*
		muonDTDigis*
		muonCSCDigis*
		muonRPCDigis*
		castorDigis*
		scalersRawToDigi*
		tcdsDigis
)

# Set up cosmic digis
process.load("HCALPFG.HcalTupleMaker.HcalCosmicDigisProducer_cfi")

# Set up L1 Jet digis
process.load("HCALPFG.HcalTupleMaker.HcalL1JetDigisProducer_cfi")

# Set up our analyzer
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi")
      
# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, options.globalTag, '')
    
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*    # Make HCAL tuples: FED info
    # Make HCAL tuples: FED info
    #process.hcalTupleFEDs*
    # Make HCAL tuples: digi info
    #process.hcalTupleHBHEDigis*
    #process.hcalTupleHODigis*
    #process.hcalTupleHFDigis*
    #process.hcalTupleTriggerPrimitives*
    # Make HCAL tuples: digi info
    process.hcalTupleHBHECosmicsDigis*
    # Make HCAL tuples: cosmic muon info
    process.hcalTupleCosmicMuons*
    # Package everything into a tree
    process.hcalTupleTree
)



# Path and EndPath definitions
process.preparation = cms.Path(
    #process.RawToDigi *
    process.CustomizedRawToDigi*
    #process.L1Reco *
    process.reconstructionCosmics *
    process.hcalCosmicDigis *
    #process.hcalL1JetDigis *
    process.tuple_step
)
