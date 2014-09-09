#ifndef HcalTupleMaker_HFDigis_h
#define HcalTupleMaker_HFDigis_h

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalDigis.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HFDigis : public HcalTupleMaker_HcalDigis {
 public:
  explicit HcalTupleMaker_HFDigis(const edm::ParameterSet&);
 
 private:
  void produce( edm::Event &, const edm::EventSetup & );

};

#endif
