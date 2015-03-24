import FWCore.ParameterSet.Config as cms
#
############################################################################################################
# Collision HCAL Rechit reconstruction                                                                     #
# http://cmslxr.fnal.gov/lxr/source/RecoLocalCalo/HcalRecProducers/python/HcalHitReconstructor_hbhe_cfi.py #
# For example, by default, this has: samplesToAdd = cms.int32(2) PLUS other settings                       #
############################################################################################################
# Removing this: from Configuration.StandardSequences.Reconstruction_cff import hbheprereco
#
# Isolation filter (in reco sequence) is set via:
# http://cmslxr.fnal.gov/lxr/source/RecoLocalCalo/HcalRecProducers/python/HBHEIsolatedNoiseReflagger_cfi
# This produces hbhereco from hbheprereco (also applying HBHEIsolatedNoiseReflagger).
# So we should use hbhereco instead.
#
from Configuration.StandardSequences.Reconstruction_cff import hbheprereco
from Configuration.StandardSequences.Reconstruction_cff import hbhereco
#
hbheprerecoMethod2 = hbheprereco.clone()
hbheprerecoMethod2.puCorrMethod = cms.int32(2)
hbheprerecoMethod0 = hbheprereco.clone()
hbheprerecoMethod0.puCorrMethod = cms.int32(0)

hbherecoMethod2 = hbhereco.clone()
hbherecoMethod2.hbheInput = cms.InputTag('hbheprerecoMethod2')
hbherecoMethod0 = hbhereco.clone()
hbherecoMethod0.hbheInput = cms.InputTag('hbheprerecoMethod0')

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
