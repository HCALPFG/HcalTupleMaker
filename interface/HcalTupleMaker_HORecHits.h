#ifndef HcalTupleMaker_HORecHits_h
#define HcalTupleMaker_HORecHits_h

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalRecHits.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HORecHits : public HcalTupleMaker_HcalRecHits {
 public:
  explicit HcalTupleMaker_HORecHits(const edm::ParameterSet&);
 
 private:
  void produce( edm::Event &, const edm::EventSetup & );

};

#endif
