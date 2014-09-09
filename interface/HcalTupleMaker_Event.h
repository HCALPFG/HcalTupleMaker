#ifndef HcalTupleMaker_Event_h
#define HcalTupleMaker_Event_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class HcalTupleMaker_Event : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_Event(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );

};

#endif
