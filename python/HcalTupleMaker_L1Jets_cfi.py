import FWCore.ParameterSet.Config as cms

hcalTupleL1Jets = cms.EDProducer("HcalTupleMaker_L1Jets",
  source  = cms.untracked.VInputTag( cms.InputTag( "l1extraParticles", "Central" ),
                                     cms.InputTag( "l1extraParticles", "Forward" ),
                                     cms.InputTag( "l1extraParticles", "Tau"     )),
  hcalTPs = cms.untracked.InputTag("hcalL1JetDigis"),
  Prefix  = cms.untracked.string("L1Jet"),
  Suffix  = cms.untracked.string("")                                 
)                                 

