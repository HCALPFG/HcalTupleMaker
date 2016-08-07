import FWCore.ParameterSet.Config as cms

from SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff import *

hcalTupleHBHEDigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("hcalDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ( "HBHEDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) ,
  TotalFCthreshold = cms.untracked.double ( -9999 ) ,
  FilterChannels = cms.untracked.bool ( False ) ,
  ChannelFilterList = cms.untracked.VPSet(cms.PSet(iEta = cms.int32(0), iPhi = cms.int32(0), depth = cms.int32(0)))
)

hcalTupleHBHECosmicsDigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("hcalCosmicDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ( "HBHECosmicDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) , 
  TotalFCthreshold = cms.untracked.double ( -9999 ) , 
  FilterChannels = cms.untracked.bool ( False ) ,
  ChannelFilterList = cms.untracked.VPSet(cms.PSet(iEta = cms.int32(0), iPhi = cms.int32(0), depth = cms.int32(0)))
)

hcalTupleHBHEL1JetsDigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("hcalL1JetDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ( "HBHEL1JetDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) ,
  TotalFCthreshold = cms.untracked.double ( -9999 ) ,
  FilterChannels = cms.untracked.bool ( False ) ,
  ChannelFilterList = cms.untracked.VPSet(cms.PSet(iEta = cms.int32(0), iPhi = cms.int32(0), depth = cms.int32(0)))
)

hcalTupleHBHEUTCADigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("utcaDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ( "HBHEUTCADigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) ,
  TotalFCthreshold = cms.untracked.double ( -9999 ) , 
  FilterChannels = cms.untracked.bool ( False ) ,
  ChannelFilterList = cms.untracked.VPSet(cms.PSet(iEta = cms.int32(0), iPhi = cms.int32(0), depth = cms.int32(0)))
)

