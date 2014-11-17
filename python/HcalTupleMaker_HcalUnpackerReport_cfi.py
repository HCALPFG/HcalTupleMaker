import FWCore.ParameterSet.Config as cms

hcalTupleUnpackReport = cms.EDProducer("HcalTupleMaker_HcalUnpackerReport",
  source = cms.untracked.InputTag("hcalDigis"),
  Prefix = cms.untracked.string("HcalUnpacker"),
  Suffix = cms.untracked.string("")
)
