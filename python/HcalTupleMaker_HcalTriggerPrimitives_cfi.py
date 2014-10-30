import FWCore.ParameterSet.Config as cms

hcalTupleTriggerPrimitives = cms.EDProducer("HcalTupleMaker_HcalTriggerPrimitives",
  source    = cms.untracked.InputTag("hcalDigis"),
  hbheDigis = cms.untracked.InputTag("hcalDigis"),
  hfDigis   = cms.untracked.InputTag("hcalDigis"),
  Prefix    = cms.untracked.string  ("HcalTriggerPrimitive"),
  Suffix    = cms.untracked.string  ("")
)

hcalTupleL1JetTriggerPrimitives = cms.EDProducer("HcalTupleMaker_HcalTriggerPrimitives",
  source    = cms.untracked.InputTag("hcalL1JetDigis"),
  hbheDigis = cms.untracked.InputTag("hcalL1JetDigis"),
  hfDigis   = cms.untracked.InputTag("hcalL1JetDigis"),
  Prefix    = cms.untracked.string  ("HcalL1JetTriggerPrimitive"),
  Suffix    = cms.untracked.string  ("")
)
