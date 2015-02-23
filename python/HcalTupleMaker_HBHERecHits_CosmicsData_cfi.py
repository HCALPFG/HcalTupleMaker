import FWCore.ParameterSet.Config as cms

############################################################################################################
# Cosmic HCAL Rechit reconstruction                                                                        #
# http://cmslxr.fnal.gov/lxr/source/RecoLocalCalo/Configuration/python/RecoLocalCalo_Cosmics_cff.py        #
# For example, by default, this has: samplesToAdd = cms.int32(8) PLUS other settings                       #
############################################################################################################
from RecoLocalCalo.Configuration.RecoLocalCalo_Cosmics_cff import hbheprereco
#
hbherecoMethod2 = hbheprereco.clone()
hbherecoMethod2.puCorrMethod = cms.int32(2)
hbherecoMethod0 = hbheprereco.clone()
hbherecoMethod0.puCorrMethod = cms.int32(0)


hcalTupleHBHERecHits = cms.EDProducer("HcalTupleMaker_HBHERecHits",
                                      source = cms.untracked.InputTag("hbherecoMethod2"),
                                      Prefix = cms.untracked.string  ("HBHERecHit"),
                                      Suffix = cms.untracked.string  ("")
                                      )


hcalTupleHBHERecHitsMethod0 = cms.EDProducer("HcalTupleMaker_HBHERecHits",
                                             source = cms.untracked.InputTag("hbherecoMethod0"),
                                             Prefix = cms.untracked.string ("HBHERecHit"),
                                             Suffix = cms.untracked.string ("Method0")
                                             )
