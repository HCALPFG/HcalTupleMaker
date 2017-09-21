import FWCore.ParameterSet.Config as cms

hcalTupleHBHERecHits = cms.EDProducer("HcalTupleMaker_HBHERecHits",
  #source = cms.untracked.InputTag("hbhereco"),
  source = cms.untracked.InputTag("hbheplan1"),
  isHBHE = cms.untracked.bool(True),
  energyThresholdHFHO = cms.untracked.double(-9999.0),
  Prefix = cms.untracked.string  ("HBHERecHit"),
  Suffix = cms.untracked.string  ("")
)
