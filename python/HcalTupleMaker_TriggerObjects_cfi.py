import FWCore.ParameterSet.Config as cms

hcalTupleTriggerObjects= cms.EDProducer("HcalTupleMaker_TriggerObjects",
    InputTag = cms.InputTag('hltTriggerSummaryAOD',"","HLT"),
    Prefix = cms.string("HLT"),
    Suffix = cms.string('')
)
