#ifndef HcalTupleMaker_HODigis_h
#define HcalTupleMaker_HODigis_h

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalDigis.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HODigis : public HcalTupleMaker_HcalDigis {
 public:
  explicit HcalTupleMaker_HODigis(const edm::ParameterSet&);
 
 private:
  void produce( edm::Event &, const edm::EventSetup & );

};

#endif
