#ifndef HcalTupleMaker_HcalIsoNoiseFilterParameters_h
#define HcalTupleMaker_HcalIsoNoiseFilterParameters_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HcalIsoNoiseFilterParameters : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalIsoNoiseFilterParameters(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag   noiseSummaryInputTag;
  //std::string     noiseResultInputTag;
  //std::string     recoInputTag;
  //bool            isRAW;
  std::string     prefix,suffix;
};

#endif
