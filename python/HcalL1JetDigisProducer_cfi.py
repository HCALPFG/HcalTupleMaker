import FWCore.ParameterSet.Config as cms

hcalL1JetDigis = cms.EDProducer("HcalL1JetDigisProducer",
   HBHEDigis  = cms.InputTag("hcalDigis"),
   HFDigis    = cms.InputTag("hcalDigis"),
   TrigPrims  = cms.InputTag("hcalDigis"),
   L1Jets     = cms.VInputTag( cms.InputTag( "l1extraParticles", "Central" ),
                               cms.InputTag( "l1extraParticles", "Forward" ),
                               cms.InputTag( "l1extraParticles", "Tau"     ) )
)                                 
