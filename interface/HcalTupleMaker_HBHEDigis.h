#ifndef HcalTupleMaker_HBHEDigis_h
#define HcalTupleMaker_HBHEDigis_h

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalDigis.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HBHEDigis : public HcalTupleMaker_HcalDigis {
 public:
  explicit HcalTupleMaker_HBHEDigis(const edm::ParameterSet&);
 
 private:
  void produce( edm::Event &, const edm::EventSetup & );

};

#endif
