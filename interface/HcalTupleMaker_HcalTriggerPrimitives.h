#ifndef HcalTupleMaker_HcalTriggerPrimitives_h
#define HcalTupleMaker_HcalTriggerPrimitives_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

class HcalTupleMaker_HcalTriggerPrimitives : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalTriggerPrimitives(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const edm::InputTag   hbheInputTag;
  const edm::InputTag   hfInputTag;
  const std::string     prefix,suffix;

  edm::EDGetTokenT<HBHEDigiCollection> hbheToken_;
  edm::EDGetTokenT<HFDigiCollection> hfToken_;
  edm::EDGetTokenT<HcalTrigPrimDigiCollection> tpToken_;

};

#endif
