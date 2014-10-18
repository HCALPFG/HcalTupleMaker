import FWCore.ParameterSet.Config as cms

hcalTupleTrigger = cms.EDProducer("HcalTupleMaker_Trigger",
    L1InputTag  = cms.InputTag('gtDigis'),
    HLTInputTag = cms.InputTag('TriggerResults','','HLT'),
    SourceName  = cms.string("")
)
