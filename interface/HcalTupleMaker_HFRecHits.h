#ifndef HcalTupleMaker_HFRecHits_h
#define HcalTupleMaker_HFRecHits_h

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalRecHits.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HFRecHits : public HcalTupleMaker_HcalRecHits {
 public:
  explicit HcalTupleMaker_HFRecHits(const edm::ParameterSet&);
 
 private:
  void produce( edm::Event &, const edm::EventSetup & );

};

#endif
