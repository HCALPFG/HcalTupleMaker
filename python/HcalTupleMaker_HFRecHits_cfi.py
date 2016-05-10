import FWCore.ParameterSet.Config as cms

hcalTupleHFRecHits = cms.EDProducer("HcalTupleMaker_HFRecHits",
  source = cms.untracked.InputTag("hfreco"),
  isHBHE = cms.untracked.bool(False),
  energyThresholdHFHO = cms.untracked.double(-999.9),
  Prefix = cms.untracked.string  ("HFRecHit"),
  Suffix = cms.untracked.string  ("")
)
