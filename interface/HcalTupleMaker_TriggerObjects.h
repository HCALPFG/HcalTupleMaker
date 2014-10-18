#ifndef HcalTupleMaker_TriggerObjects_h
#define HcalTupleMaker_TriggerObjects_h

#include <string> 

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_TriggerObjects : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_TriggerObjects(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     prefix, suffix;

};

#endif
