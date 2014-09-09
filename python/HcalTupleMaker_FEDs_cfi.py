import FWCore.ParameterSet.Config as cms

hcalTupleFEDs = cms.EDProducer("HcalTupleMaker_FEDs",
   source = cms.untracked.InputTag("rawDataCollector"),
   Prefix = cms.untracked.string("FED"),
   Suffix = cms.untracked.string("")
)                               
