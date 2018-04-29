import FWCore.ParameterSet.Config as cms

hcalTupleLasermon = cms.EDProducer("HcalTupleMaker_Lasermon",
  #tagLasermon = cms.untracked.InputTag("lasermonDigis"),
  laserMonCBoxList = cms.untracked.vint32( 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5 ),
  laserMonIPhiList = cms.untracked.vint32(23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0),
  laserMonIEtaList = cms.untracked.vint32( 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ),

)
