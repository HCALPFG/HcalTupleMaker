
def tweakEventList(textFilePath):
	lines = open(textFilePath,"rb").readlines()
	eventList = [ ":".join(line.split())  for line in lines]
	return eventList

#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

#------------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------------

options = VarParsing.VarParsing()

options.register('outputFile',
                 "file:outputFile.root", #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.register('textFile',
                 "", #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "text file for event list")

options.parseArguments()

print options.textFile
print options.outputFile

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

from HCALPFG.HcalTupleMaker.FileList.PrefireEventList import prefireEventList
inputFileNames = cms.untracked.vstring()
inputFileNames.extend( prefireEventList )
process.source.fileNames = inputFileNames

#------------------------------------------------------------------------------------
# What events should we run over?
#------------------------------------------------------------------------------------
inputEvtList = tweakEventList(options.textFile)
print inputEvtList
process.source.eventsToProcess = cms.untracked.VEventRange(*inputEvtList)
#process.source.eventsToProcess.extend(inputEvtList)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string(options.outputFile) )

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

#------------------------------------------------------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------------------------------------------------------
process.load("HCALPFG.HcalTupleMaker.CSCFilter_cfi")

#------------------------------------------------------------------------------------
# Define the path
#------------------------------------------------------------------------------------

# Path and EndPath definitions
process.p = cms.Path(
    process.cscFilter
    #process.RawToDigi*
    #process.reconstruction
    #process.reco_step *
    #process.hcalnoise *
    #process.HBHENoiseFilterResultProducer *
    #process.ApplyBaselineHBHENoiseFilter * 
    #process.tuple_step * 
    #process.hcalTupleTree
)

process.e = cms.EndPath(process.out) 	
