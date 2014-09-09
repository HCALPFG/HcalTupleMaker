#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Tree.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Event.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_FEDs.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HBHEDigis.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HODigis.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HFDigis.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HBHERecHits.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HORecHits.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HFRecHits.h"

DEFINE_FWK_MODULE(HcalTupleMaker_Tree);
DEFINE_FWK_MODULE(HcalTupleMaker_Event);
DEFINE_FWK_MODULE(HcalTupleMaker_FEDs);
DEFINE_FWK_MODULE(HcalTupleMaker_HBHEDigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HODigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HFDigis);
DEFINE_FWK_MODULE(HcalTupleMaker_HBHERecHits);
DEFINE_FWK_MODULE(HcalTupleMaker_HORecHits);
DEFINE_FWK_MODULE(HcalTupleMaker_HFRecHits);
