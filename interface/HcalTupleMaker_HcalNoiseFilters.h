#ifndef HcalTupleMaker_HcalNoiseFilters_h
#define HcalTupleMaker_HcalNoiseFilters_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HcalNoiseFilters : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalNoiseFilters(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag   noiseSummaryInputTag;
  std::string     noiseResultInputTag;
  std::string     recoInputTag;
  bool            isRAW;
  bool            isRECO;
  std::string     prefix,suffix;
};

#endif
