#ifndef HcalTupleMaker_HBHERecHits_h
#define HcalTupleMaker_HBHERecHits_h

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalRecHits.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HBHERecHits : public HcalTupleMaker_HcalRecHits {
 public:
  explicit HcalTupleMaker_HBHERecHits(const edm::ParameterSet&);
 
 private:
  void produce( edm::Event &, const edm::EventSetup & );

};

#endif
