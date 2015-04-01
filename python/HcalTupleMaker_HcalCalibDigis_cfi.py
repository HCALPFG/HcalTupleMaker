import FWCore.ParameterSet.Config as cms

from SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff import *

hcalTupleCalibDigis = cms.EDProducer("HcalTupleMaker_CalibDigis",
  source  = cms.untracked.InputTag("hcalDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ("HcalCalibDigi"),
  Suffix  = cms.untracked.string (""),
  DoChargeReco = cms.untracked.bool ( False ) ,
  DoEnergyReco = cms.untracked.bool ( False ) ,
  TotalFCthreshold = cms.untracked.double ( -9999 )
)
