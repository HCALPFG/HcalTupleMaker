import FWCore.ParameterSet.Config as cms

hcalTupleQIE11Digis = cms.EDProducer("HcalTupleMaker_QIE11Digis",
  tagQIE11  = cms.untracked.InputTag("qie11Digis"),
  Prefix  = cms.untracked.string("QIE11Digi"),
  Suffix  = cms.untracked.string("")                      
)
