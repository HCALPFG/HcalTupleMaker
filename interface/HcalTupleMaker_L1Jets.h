#ifndef HcalTupleMaker_L1Jets_h
#define HcalTupleMaker_L1Jets_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

class HcalTupleMaker_L1Jets : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_L1Jets(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  std::vector<edm::InputTag> inputTags;
  edm::InputTag   tpInputTag;
  std::string     prefix,suffix;
  edm::EDGetTokenT<HcalTrigPrimDigiCollection> tpToken_;
  std::vector<edm::EDGetTokenT<l1extra::L1JetParticleCollection>> l1JetsTokens_;
};

#endif
