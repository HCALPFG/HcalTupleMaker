import FWCore.ParameterSet.Config as cms

from SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff import *

hcalTupleHODigis = cms.EDProducer("HcalTupleMaker_HODigis",
  source  = cms.untracked.InputTag("hcalDigis"),
  recHits = cms.untracked.InputTag("horeco"),
  Prefix  = cms.untracked.string ( "HODigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) ,
  TotalFCthreshold = cms.untracked.double ( -9999 ) ,
  FilterChannels = cms.untracked.bool ( False ) ,
  ChannelFilterList = cms.untracked.VPSet(cms.PSet(iEta = cms.int32(0), iPhi = cms.int32(0), depth = cms.int32(0)))
)

hcalTupleHOCosmicsDigis = cms.EDProducer("HcalTupleMaker_HODigis",
  source  = cms.untracked.InputTag("hcalCosmicDigis"),
  recHits = cms.untracked.InputTag("horeco"),
  Prefix  = cms.untracked.string ( "HOCosmicDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) ,
  TotalFCthreshold = cms.untracked.double ( -9999 ) ,
  FilterChannels = cms.untracked.bool ( False ) ,
  ChannelFilterList = cms.untracked.VPSet(cms.PSet(iEta = cms.int32(0), iPhi = cms.int32(0), depth = cms.int32(0)))
)
