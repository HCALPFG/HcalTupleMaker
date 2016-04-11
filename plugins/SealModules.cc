#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Tree.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Event.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_FEDs.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalDigis.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalLaserDigis.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalRecHits.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Trigger.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_TriggerObjects.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_RecoTracks.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_L1Jets.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalNoiseFilters.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalIsoNoiseFilterParameters.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_CaloJetMet.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_MuonTrack.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_L1GCTJets.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalTriggerPrimitives.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalUnpackerReport.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalCosmicDigisProducer.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalL1JetDigisProducer.h"

DEFINE_FWK_MODULE(HcalTupleMaker_Tree);
DEFINE_FWK_MODULE(HcalTupleMaker_Event);
DEFINE_FWK_MODULE(HcalTupleMaker_FEDs);
DEFINE_FWK_MODULE(HcalTupleMaker_HBHEDigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HODigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HFDigis);
DEFINE_FWK_MODULE(HcalTupleMaker_CalibDigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HcalLaserDigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HBHERecHits);
DEFINE_FWK_MODULE(HcalTupleMaker_HORecHits);
DEFINE_FWK_MODULE(HcalTupleMaker_HFRecHits);
DEFINE_FWK_MODULE(HcalTupleMaker_Trigger);
DEFINE_FWK_MODULE(HcalTupleMaker_TriggerObjects);
DEFINE_FWK_MODULE(HcalTupleMaker_RecoTracks);
DEFINE_FWK_MODULE(HcalTupleMaker_L1Jets);
DEFINE_FWK_MODULE(HcalTupleMaker_HcalNoiseFilters);
DEFINE_FWK_MODULE(HcalTupleMaker_HcalIsoNoiseFilterParameters);
DEFINE_FWK_MODULE(HcalTupleMaker_CaloJetMet);
DEFINE_FWK_MODULE(HcalTupleMaker_MuonTrack);
DEFINE_FWK_MODULE(HcalTupleMaker_L1GCTJets);
DEFINE_FWK_MODULE(HcalTupleMaker_HcalTriggerPrimitives);
DEFINE_FWK_MODULE(HcalTupleMaker_HcalUnpackerReport);
DEFINE_FWK_MODULE(HcalCosmicDigisProducer);
DEFINE_FWK_MODULE(HcalL1JetDigisProducer);
