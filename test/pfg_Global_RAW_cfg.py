#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import FWCore.ParameterSet.VarParsing as VarParsing

#------------------------------------------------------------------------------------
# Declare the process and input variables
#------------------------------------------------------------------------------------
process = cms.Process('PFG',eras.Run2_2017)
inputFiles = "root://eoscms//store/caf/user/ccecal/TPG/splash2017_run293591_all.root"
skipEvents = 0
processEvents = 10
outputFile = "HcalTupleMaker.root"

#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(processEvents) )
process.source = cms.Source("PoolSource",
    fileNames  = cms.untracked.vstring(inputFiles),
    skipEvents = cms.untracked.uint32(skipEvents)
)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string(outputFile)
)

#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('RecoMET.METProducers.hcalnoiseinfoproducer_cfi')
process.load("CommonTools.RecoAlgos.HBHENoiseFilter_cfi")
process.load("CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi")

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi") # Dont want to use this, load modules individually
process.hcalTupleHFDigis.DoEnergyReco = False
process.hcalTupleHFDigis.FilterChannels = False
process.hcalTupleHFDigis.ChannelFilterList = cms.untracked.VPSet(
	# Notice the only channel listed here will be saved, if the FilterChannels flag is set to true
    cms.PSet(iEta = cms.int32(29), iPhi = cms.int32(39), depth = cms.int32(1)),
    )

from Configuration.StandardSequences.RawToDigi_Data_cff import *
process.CustomizedRawToDigi = cms.Sequence(
        gtDigis*
		#siPixelDigis*
		#siStripDigis*
		#ecalDigis*
		#ecalPreshowerDigis*
		hcalDigis
		#muonDTDigis*
		#muonCSCDigis*
		#muonRPCDigis*
		#castorDigis*
		#scalersRawToDigi*
		#tcdsDigis
)

#------------------------------------------------------------------------------------
# FED numbers 
#------------------------------------------------------------------------------------
#process.hcalDigis.FEDs = cms.untracked.vint32(	1100, 1102, 1104, # HBHEa 
#                                              	1106, 1108, 1110, # HBHEb
#                                              	1112, 1114, 1116, # HBHEc
#                                              	1118, 1120, 1122, # HF
#                                              	1119, 1121, 1123  # HF
#												724, 725, 726, 727, 728, 729, 730, 731, # HO
# 												1134, # HcalLaser
#                                              )

#------------------------------------------------------------------------------------
# QIE10  Unpacker
#------------------------------------------------------------------------------------
process.qie10Digis = process.hcalDigis.clone()
#process.qie10Digis.FEDs = cms.untracked.vint32(1118,1120,1122,1119,1121,1123)

#------------------------------------------------------------------------------------
# Specify Global Tag
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '90X_dataRun2_Prompt_v3'

#------------------------------------------------------------------------------------
# Create Noise Filter
#------------------------------------------------------------------------------------
# Could be out of date, commented out in the main sequence, need to be fixed
process.hcalnoise.fillCaloTowers = cms.bool(False)
process.hcalnoise.fillTracks = cms.bool(False)
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
			inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
			reverseDecision = cms.bool(False)
			)

#------------------------------------------------------------------------------------
# HcalTupleMaker sequence definition
#------------------------------------------------------------------------------------
process.tuple_step = cms.Sequence(
    
	## Make HCAL tuples: Event, run, ls number
    #process.hcalTupleEvent*
    
	## Make HCAL tuples: FED info
    #process.hcalTupleFEDs*
    
	## Make HCAL tuples: digi info
    process.hcalTupleHBHEDigis*
    process.hcalTupleHODigis*
    process.hcalTupleHFDigis*
    process.hcalTupleQIE10Digis*
     
	## Make HCAL tuples: reco info
    #process.hcalTupleHBHERecHits*
    #process.hcalTupleHFRecHits*
    #process.hcalTupleHORecHits*
    #process.hcalTupleHFRecHits*

    ## Trigger info
    #process.hcalTupleTrigger*
    #process.hcalTupleTriggerPrimitives*
    #process.hcalTupleTriggerObjects*

    ## Package everything into a tree
    process.hcalTupleTree
)

#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(
    #process.RawToDigi * #needed for RAW files
    #process.CustomizedRawToDigi *
    process.hcalDigis*
    process.qie10Digis*
    #process.L1Reco *
    #process.reconstruction * #needed for RAW files
    #process.hcalLocalRecoSequence *
    #
    #process.horeco *
    #process.hfreco *
    #
    #process.hcalnoise *  #needed for RAW files
    #process.HBHENoiseFilterResultProducer *
    #process.ApplyBaselineHBHENoiseFilter *
    #
    process.tuple_step
)
