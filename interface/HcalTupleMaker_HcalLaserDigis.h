#ifndef HcalTupleMaker_HcalLaserDigis_h
#define HcalTupleMaker_HcalLaserDigis_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HcalLaserDigis : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalLaserDigis(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag inputTag;
  std::string   prefix,suffix;
  const int nQADC;
};

#endif
