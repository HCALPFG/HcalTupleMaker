import FWCore.ParameterSet.Config as cms

hcalTupleHFRecHits = cms.EDProducer("HcalTupleMaker_HFRecHits",
  source = cms.untracked.InputTag("hfreco"),
  isHBHE = cms.untracked.bool(False),
  Prefix = cms.untracked.string  ("HFRecHit"),
  Suffix = cms.untracked.string  ("")
)
