import FWCore.ParameterSet.Config as cms

hcalTupleQIE10Digis = cms.EDProducer("HcalTupleMaker_QIE10Digis",
  #tagQIE10  = cms.untracked.InputTag("qie10Digis"),
  tagQIE10  = cms.untracked.InputTag("hcalDigis"),
  Prefix  = cms.untracked.string("QIE10Digi"),
  Suffix  = cms.untracked.string("")
)
