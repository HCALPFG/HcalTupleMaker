#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
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
                 #"root://eoscms.cern.ch//eos/cms/store/group/dpg_hcal/comm_hcal/USC/run292140/USC_292140.root", #default value
                 "root://eoscms.cern.ch//eos/cms/store/group/dpg_hcal/comm_hcal/USC/run292350/USC_292350.root", #default value
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

options.register('outputFile',
                 "outputFile_"+options.inputFiles[0].split("USC_")[1], #default value
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
process = cms.Process('PFG')

#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.processEvents) )
process.source = cms.Source(
    "HcalTBSource",
    fileNames  = cms.untracked.vstring(options.inputFiles),
    skipEvents = cms.untracked.uint32(options.skipEvents)
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile)
)

#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
# process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
# process.hbhereco = process.hbheprereco.clone()
process.load("CondCore.CondDB.CondDB_cfi")

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
# process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi") # Dont want to use this, load modules individually
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Tree_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Event_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalUnpackerReport_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_QIE11Digis_cfi")

#------------------------------------------------------------------------------------
# Since this is a local run, make sure we're looking for the FEDs in the right place
#------------------------------------------------------------------------------------
process.hcalDigis.InputLabel = cms.InputTag("source")

#------------------------------------------------------------------------------------
# FED numbers
#------------------------------------------------------------------------------------
process.hcalDigis.FEDs = cms.untracked.vint32(1100, 1102, 1104, # HBHEa 
                                              1106, 1108, 1110, # HBHEb
                                              1112, 1114, 1116, # HBHEc
                                              1118, 1120, 1122, # HF
                                              1119, 1121, 1123  # HF
                                              )

#------------------------------------------------------------------------------------
# QIE11  Unpacker
#------------------------------------------------------------------------------------
process.qie11Digis = process.hcalDigis.clone()
process.qie11Digis.InputLabel = cms.InputTag("source") 
process.qie11Digis.FEDs = cms.untracked.vint32(1114)

#------------------------------------------------------------------------------------
# Specify Global Tag
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '90X_dataRun2_Prompt_v2'
print "GlobalTag = ", str(process.GlobalTag.globaltag).split("'")[1]
print " "

# Customize global tag with local emap
# process.es_ascii = cms.ESSource(
#     'HcalTextCalibrations',
#     input = cms.VPSet(
#         cms.PSet(
#             object = cms.string('ElectronicsMap'),
#             file = cms.FileInPath('HCALPFG/HcalTupleMaker/data/2017-feb-06/HBHEP17_crf_emap.txt')  # EMAP here!
#             )
#         )
#     )
# process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

process.dump = cms.EDAnalyzer("HcalDigiDump")

#------------------------------------------------------------------------------------
# HcalTupleMaker sequence definition
#------------------------------------------------------------------------------------
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    # Make HCAL tuples: FED info
    # process.hcalTupleFEDs*
    # Make HCAL tuples: unpacker info
    # process.hcalTupleUnpackReport*
    # Make HCAL tuples: digi info
    # process.hcalTupleHBHEDigis*
    # process.hcalTupleHODigis*
    # process.hcalTupleHFDigis*
    # process.hcalTupleQIE10Digis*
    process.hcalTupleQIE11Digis*
    # process.hcalCosmicDigis*
    # process.hcalTupleTriggerPrimitives*
    # Make HCAL tuples: digi info
    # process.hcalTupleHBHECosmicsDigis*
    # process.hcalTupleHOCosmicsDigis*
    # Make HCAL tuples: digi info
    # process.hcalTupleHBHEL1JetsDigis*
    # process.hcalTupleHFL1JetsDigis*
    # process.hcalTupleL1JetTriggerPrimitives*
    # Make HCAL tuples: reco info
    # process.hcalTupleHBHERecHits*
    # process.hcalTupleHFRecHits*
    # process.hcalTupleHcalNoiseFilters*
    # process.hcalTupleMuonTrack*
    # process.hcalTupleHBHERecHitsMethod0*
    # process.hcalTupleHcalNoiseFiltersMethod0*
    # process.hcalTupleCaloJetMetMethod0*
    # process.hcalTupleHORecHits*
    # process.hcalTupleHFRecHits*
    # Trigger info
    # process.hcalTupleTrigger*
    # process.hcalTupleTriggerObjects*
    # Make HCAL tuples: cosmic muon info
    # process.hcalTupleCosmicMuons*
    # Package everything into a tree
    process.hcalTupleTree
)

#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(
    # Unpack digis from RAW
    process.hcalDigis*
    # process.qie10Digis*
    process.qie11Digis*
    # Do energy reconstruction
    # process.hbhereco*
    # process.horeco*
    # process.hfreco*
    # Make the ntuples
    process.tuple_step
)
