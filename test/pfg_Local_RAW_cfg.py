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
                 "file:/eos/cms/store/group/dpg_hcal/comm_hcal/USC/run322831/USC_322831.root",
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

options.register('outputFile',
                 #"file:/eos/cms/store/user/jaehyeok/HcalTupleMaker_ped_322831_cmssw_10_2_1.root", # default value
                 "file:HcalTupleMaker_ped_322831.root", # default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.parseArguments()

print "Skip events =", options.skipEvents
print "Process events =", options.processEvents
print "inputFiles =", options.inputFiles
print "outputFile =", options.outputFile


#------------------------------------------------------------------------------------
# Declare the process and input variables
#------------------------------------------------------------------------------------
from Configuration.StandardSequences.Eras import eras
process = cms.Process('PFG',eras.Run2_2018)

#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.processEvents) )
process.source = cms.Source("HcalTBSource",
    fileNames  = cms.untracked.vstring(options.inputFiles),
    skipEvents = cms.untracked.uint32(options.skipEvents),
)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string(options.outputFile)
)

#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load("CondCore.CondDB.CondDB_cfi")

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi") # Dont want to use this, load modules individually
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Tree_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Event_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHEDigis_cfi")
process.hcalTupleHBHEDigis.DoEnergyReco = False
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HODigis_cfi")
process.hcalTupleHODigis.DoEnergyReco = False
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HFDigis_cfi")
process.hcalTupleHFDigis.DoEnergyReco = False
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalUnpackerReport_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_QIE10Digis_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_QIE11Digis_cfi")

#------------------------------------------------------------------------------------
# Since this is a local run, make sure we're looking for the FEDs in the right place
#------------------------------------------------------------------------------------
process.hcalDigis.InputLabel = cms.InputTag("source")

#------------------------------------------------------------------------------------
# FED numbers
#------------------------------------------------------------------------------------
#process.hcalDigis.FEDs = cms.untracked.vint32(1100, 1102, 1104, 1106, 1108, 1110, 1112, 1114, 1116)

#------------------------------------------------------------------------------------
# Specify Global Tag
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.GlobalTag.globaltag = '102X_dataRun2_PromptLike_v4' #'101X_dataRun2_Prompt_v10'
process.GlobalTag.globaltag = '101X_dataRun2_Prompt_v11'

#------------------------------------------------------------------------------------
# HcalTupleMaker sequence definition
#------------------------------------------------------------------------------------
process.tuple_step = cms.Sequence(
    ### Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    ### Make HCAL tuples: FED info
#    process.hcalTupleFEDs*
    ### Make HCAL tuples: unpacker info
#    process.hcalTupleUnpackReport*
    ### Make HCAL tuples: digi info
    process.hcalTupleHBHEDigis* # HB digis
    process.hcalTupleHODigis*
    process.hcalTupleQIE10Digis* # HE digis
    process.hcalTupleQIE11Digis* # HF digis
    ### Make HCAL tuples: reco info
#    process.hcalTupleHBHERecHits*
#    process.hcalTupleHFRecHits*
#    process.hcalTupleHORecHits*
#    process.hcalTupleHFRecHits*
    ### Make HCAL tuples: tree
    process.hcalTupleTree
)


#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(
    # Unpack digis from RAW
    process.hcalDigis*
    # Do energy reconstruction
#    process.hbheprereco*
#    process.hbhereco*
#    process.hfprereco*
#    process.hfreco*
#    process.horeco*
    # Make the ntuples
    process.tuple_step
)

#-----------------------------------------------------------------------------------
# Dump configuraiton
#-----------------------------------------------------------------------------------
#dump = file('dump.py', 'w')
#dump.write( process.dumpPython() )
#dump.close()
