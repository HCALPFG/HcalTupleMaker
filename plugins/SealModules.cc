#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Tree.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Event.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_FEDs.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalDigis.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalRecHits.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Trigger.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_TriggerObjects.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_RecoTracks.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_L1Jets.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalCosmicDigisProducer.h"

DEFINE_FWK_MODULE(HcalTupleMaker_Tree);
DEFINE_FWK_MODULE(HcalTupleMaker_Event);
DEFINE_FWK_MODULE(HcalTupleMaker_FEDs);
DEFINE_FWK_MODULE(HcalTupleMaker_HBHEDigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HODigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HFDigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HBHERecHits);
DEFINE_FWK_MODULE(HcalTupleMaker_HORecHits);
DEFINE_FWK_MODULE(HcalTupleMaker_HFRecHits);
DEFINE_FWK_MODULE(HcalTupleMaker_Trigger);
DEFINE_FWK_MODULE(HcalTupleMaker_TriggerObjects);
DEFINE_FWK_MODULE(HcalTupleMaker_RecoTracks);
DEFINE_FWK_MODULE(HcalTupleMaker_L1Jets);
DEFINE_FWK_MODULE(HcalCosmicDigisProducer);
