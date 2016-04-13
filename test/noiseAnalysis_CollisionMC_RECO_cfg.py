#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import FWCore.ParameterSet.VarParsing as VarParsing


#------------------------------------------------------------------------------------
# Declare the process and input variables
#------------------------------------------------------------------------------------
#process = cms.Process('NOISE',eras.Run2_50ns)#for 50ns 13 TeV data
process = cms.Process('NOISE',eras.Run2_25ns)#for 25ns 13 TeV data
options = VarParsing.VarParsing ('analysis')
options.register ('skipEvents', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "no of skipped events")
#options.inputFiles = 'root://xrootd.unl.edu//store/mc/RunIIFall15DR75/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/GEN-SIM-RECO/Asympt25nsReco_75X_mcRun2_asymptotic_v8-v1/50000/02453C84-9A71-E511-A4E5-0025905A48BC.root'
options.inputFiles = '/store/mc/RunIIFall15DR75/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/GEN-SIM-RECO/Asympt25nsReco_75X_mcRun2_asymptotic_v8-v1/50000/02453C84-9A71-E511-A4E5-0025905A48BC.root'
options.outputFile = 'results.root'
#options.maxEvents = 100 # -1 means all events
#options.skipEvents = 0 # default is 0.


#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
options.parseArguments()
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.source = cms.Source("PoolSource",
    fileNames  = cms.untracked.vstring(options.inputFiles),
    skipEvents = cms.untracked.uint32(options.skipEvents) # default is 0.
)

process.TFileService = cms.Service("TFileService",
   # fileName = cms.string('file:/tmp/hsaka/Test.root')
     fileName = cms.string(options.outputFile)
)


#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')


#------------------------------------------------------------------------------------
# Set up L1 Jet digis #Disabled 
#------------------------------------------------------------------------------------
#process.load("HCALPFG.HcalTupleMaker.HcalL1JetDigisProducer_cfi")


#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_cfi") # Dont want to use this, load modules individually
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Tree_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Event_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHEDigis_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHERecHits_cfi")
#process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Trigger_cfi")


#------------------------------------------------------------------------------------
# Set up noise filters
#------------------------------------------------------------------------------------
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalNoiseFilters_cfi") # This is over-ridden below to remove Method0-Method2 dual reco.
#------------------------------------------------------------------------------------
# Set up iso noise filter parameters, used for iso-noise filter study in 25ns.
#------------------------------------------------------------------------------------
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalIsoNoiseFilterParameters_cfi")
#------------------------------------------------------------------------------------
# Set up CaloJetMet quantities 
#------------------------------------------------------------------------------------
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_CaloJetMet_cfi") # This is over-ridden below to remove Method0-Method2 dual reco.
#------------------------------------------------------------------------------------
# Set up MuonTrack quantities 
#------------------------------------------------------------------------------------
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_MuonTrack_cfi")


#------------------------------------------------------------------------------------
# Specify Global Tag
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v12'
#from Configuration.AlCa.autoCond import autoCond
#process.GlobalTag.globaltag = autoCond['run2_data']


#------------------------------------------------------------------------------------
# Disabled since we dont deal with HLT
#------------------------------------------------------------------------------------
#process.my_hlt = cms.EDFilter("HLTHighLevel",
#     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
#     HLTPaths = cms.vstring("HLT_L1SingleJet16*"), # provide list of HLT paths (or patterns) you want
#     eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
#     andOr = cms.bool(True),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
#     throw = cms.bool(False)    # throw exception on unknown path names
#)


#------------------------------------------------------------------------------------
#Remove Method 0, Rename Method 2 as "default" where necessary:
#------------------------------------------------------------------------------------
process.hcalTupleCaloJetMet = cms.EDProducer("HcalTupleMaker_CaloJetMet",
         recoInputTag         = cms.untracked.string("hbhereco"),
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)
process.hcalTupleHcalNoiseFilters = cms.EDProducer("HcalTupleMaker_HcalNoiseFilters",
         noiseSummaryInputTag = cms.untracked.InputTag("hcalnoise"),
         noiseResultInputTag  = cms.untracked.string("HBHENoiseFilterResultProducer"),
         recoInputTag         = cms.untracked.string("hbhereco"),
         isRAW  = cms.untracked.bool(False), # new Flag necessary for HcalNoiseFilters to run on RECO data
         isRECO = cms.untracked.bool(True), 
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)


#------------------------------------------------------------------------------------
# Place-holder for applying HBHE noise filter:
#------------------------------------------------------------------------------------
#process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
#    inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),    
#    #inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun1'),
#    #inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Loose'),
#    #inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Tight'),
#    reverseDecision = cms.bool(False)
#)


#------------------------------------------------------------------------------------
# This enables NEF flagging, but needs reconstruction of RAW data.
# This is not needed for datasets reconstructed with >=CMSSW748:
# i.e. 2015C Prompt-reco has NEF flags computed out-of-the-box.
#------------------------------------------------------------------------------------
#process.hbheprereco.setNegativeFlags          = cms.bool(True)


#------------------------------------------------------------------------------------
# HcalTupleMaker sequence definition
#------------------------------------------------------------------------------------
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    # Make HCAL tuples: FED info
    #    process.hcalTupleFEDs*
    #    # Make HCAL tuples: digi info
    #raw# process.hcalTupleHBHEDigis*
    #    process.hcalTupleHODigis*
    #    process.hcalTupleHFDigis*
    #    process.hcalTupleTriggerPrimitives*
    #    # Make HCAL tuples: digi info
    #    process.hcalTupleHBHECosmicsDigis*
    #    process.hcalTupleHOCosmicsDigis*
    #    # Make HCAL tuples: digi info
    #    process.hcalTupleHBHEL1JetsDigis*
    #    process.hcalTupleHFL1JetsDigis*
    #    process.hcalTupleL1JetTriggerPrimitives*
    #    # Make HCAL tuples: reco info
    process.hcalTupleHBHERecHits*
    process.hcalTupleHcalNoiseFilters*
    process.hcalTupleHcalIsoNoiseFilterParameters* #for studying iso-noise-filter
    process.hcalTupleCaloJetMet*
    process.hcalTupleMuonTrack*
    #
    #process.hcalTupleHBHERecHitsMethod0*
    #process.hcalTupleHcalNoiseFiltersMethod0*
    #process.hcalTupleCaloJetMetMethod0*
    #    process.hcalTupleHORecHits*
    #    process.hcalTupleHFRecHits*
    #    # Trigger info
    #process.hcalTupleTrigger*
    
    #    process.hcalTupleTriggerObjects*
    #    # L1 jet info
    #    process.hcalTupleL1Jets*
    #    # Make HCAL tuples: cosmic muon info
    #    process.hcalTupleCosmicMuons*
    #    # Package everything into a tree
    #
    process.hcalTupleTree
)


#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(
    #process.my_hlt *
    #process.RawToDigi * #needed for RAW files
    #process.L1Reco *
    #rprocess.reconstruction * #needed for RAW files
    #process.caloglobalreco *
    #process.reconstructionCosmics *
    #
    #process.horeco *
    #process.hfreco *
    #
    #process.hbheprerecoMethod0 *
    #process.hbheprerecoMethod2 *
    #process.hbherecoMethod0 *
    #process.hbherecoMethod2 *
    #
    #process.towerMakerMethod0 *
    #process.towerMakerMethod2 *
    #
    #process.hcalnoiseMethod0 *
    #process.hcalnoiseMethod2 *
    #
    #process.HBHENoiseFilterResultProducerMethod0 *
    #process.HBHENoiseFilterResultProducerMethod2 *
    #
    #
    #process.hcalCosmicDigis *
    #process.hcalL1JetDigis *
    #
    #process.hcalnoise *  #needed for RAW files
    process.HBHENoiseFilterResultProducer *
    #process.ApplyBaselineHBHENoiseFilter *
    #
    process.tuple_step
)
