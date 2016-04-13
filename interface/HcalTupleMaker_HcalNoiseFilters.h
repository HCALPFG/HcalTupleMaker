#ifndef HcalTupleMaker_HcalNoiseFilters_h
#define HcalTupleMaker_HcalNoiseFilters_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"

class HcalTupleMaker_HcalNoiseFilters : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalNoiseFilters(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );

  edm::EDGetTokenT<HcalNoiseSummary>     noiseSummaryInputToken;
  std::string     noiseResultInputTag;
  edm::EDGetTokenT<HBHERecHitCollection> recoInputToken;

  //edm::InputTag   noiseSummaryInputTag;

  // std::string     recoInputTag;
  //edm::EDGetTokenT<HBHERecHitCollection> recoInputToken;

  bool            isRAW;
  bool            isRECO;
  std::string     prefix,suffix;

  edm::EDGetTokenT<bool> defaultNoiseResultInputToken;
  edm::EDGetTokenT<bool> run1NoiseResultInputToken;
  edm::EDGetTokenT<bool> run2LooseNoiseResultInputToken;
  edm::EDGetTokenT<bool> run2TightNoiseResultInputToken;
  edm::EDGetTokenT<bool> isoNoiseResultInputToken;

};

#endif
