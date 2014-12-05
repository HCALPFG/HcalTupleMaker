import FWCore.ParameterSet.Config as cms

hcalTupleTree = cms.EDAnalyzer("HcalTupleMaker_Tree",
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_hcalTupleEvent_*_*',
        'keep *_hcalTupleFEDs_*_*',
        'keep *_hcalTupleHBHEDigis_*_*',
        'keep *_hcalTupleHODigis_*_*',
        'keep *_hcalTupleHFDigis_*_*',
        'keep *_hcalTupleHFDigis_*_*',
        'keep *_hcalTupleLaserDigis_*_*',
        'keep *_hcalTupleHBHECosmicsDigis_*_*',
        'keep *_hcalTupleHOCosmicsDigis_*_*',
        'keep *_hcalTupleHBHEL1JetsDigis_*_*',
        'keep *_hcalTupleHFL1JetsDigis_*_*',
        'keep *_hcalTupleHBHERecHits_*_*',
        'keep *_hcalTupleHORecHits_*_*',
        'keep *_hcalTupleHFRecHits_*_*',
        'keep *_hcalTupleTrigger_*_*',
        'keep *_hcalTupleTriggerObjects_*_*',
        'keep *_hcalTupleTriggerPrimitives_*_*',
        'keep *_hcalTupleL1JetTriggerPrimitives_*_*',
        'keep *_hcalTupleL1Jets_*_*',
        'keep *_hcalTupleCosmicMuons_*_*'
    )
)             

