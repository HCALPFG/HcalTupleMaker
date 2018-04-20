import FWCore.ParameterSet.Config as cms

hcalTupleQIE11Digis = cms.EDProducer("HcalTupleMaker_QIE11Digis",
  #tagQIE11  = cms.untracked.InputTag("qie11Digis"),
  #taguMNio  = cms.untracked.InputTag("qie11Digis"),
  tagQIE11  = cms.untracked.InputTag("hcalDigis"),
  taguMNio  = cms.untracked.InputTag("hcalDigis"),
  Prefix  = cms.untracked.string("QIE11Digi"),
  Suffix  = cms.untracked.string(""),
  StoreLaser = cms.untracked.bool(False)
)
