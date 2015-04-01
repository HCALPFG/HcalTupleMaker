import FWCore.ParameterSet.Config as cms

from SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff import *

hcalTupleHBHEDigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("hcalDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ( "HBHEDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) 
)

hcalTupleHBHECosmicsDigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("hcalCosmicDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ( "HBHECosmicDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) 
)

hcalTupleHBHEL1JetsDigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("hcalL1JetDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ( "HBHEL1JetDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) 
)
