import FWCore.ParameterSet.Config as cms

from SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff import *

hcalTupleHFDigis = cms.EDProducer(
    "HcalTupleMaker_HFDigis",
    source  = cms.untracked.InputTag("hcalDigis"),
    recHits = cms.untracked.InputTag("hfreco"),
    Prefix  = cms.untracked.string ( "HFDigi"),
    Suffix  = cms.untracked.string ( ""),
    DoChargeReco = cms.untracked.bool ( True ) ,
    DoEnergyReco = cms.untracked.bool ( True ) ,
    TotalFCthreshold = cms.untracked.double ( -9999 ),
    FilterChannels = cms.untracked.bool ( False ) ,
    ChannelFilterList = cms.untracked.VPSet(cms.PSet(iEta = cms.int32(0), iPhi = cms.int32(0), depth = cms.int32(0)))
    )
hcalTupleHFUTCADigis = cms.EDProducer(
    "HcalTupleMaker_HFDigis",
    source  = cms.untracked.InputTag("utcaDigis"),
    recHits = cms.untracked.InputTag("hfreco"),
    Prefix  = cms.untracked.string ( "HFUTCADigi"),
    Suffix  = cms.untracked.string ( ""),
    DoChargeReco = cms.untracked.bool ( True ) ,
    DoEnergyReco = cms.untracked.bool ( True ) ,
    TotalFCthreshold = cms.untracked.double ( -9999 )
    )

hcalTupleHFL1JetsDigis = cms.EDProducer(
    "HcalTupleMaker_HFDigis",
    source  = cms.untracked.InputTag("hcalL1JetDigis"),
    recHits = cms.untracked.InputTag("hfreco"),
    Prefix  = cms.untracked.string ( "HFL1JetDigi"),
    Suffix  = cms.untracked.string ( ""),
    DoChargeReco = cms.untracked.bool ( True ) ,
    DoEnergyReco = cms.untracked.bool ( True ) ,
    TotalFCthreshold = cms.untracked.double ( -9999 )
    )
