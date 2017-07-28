#ifndef HcalTupleMaker_HcalNoiseFilters_h
#define HcalTupleMaker_HcalNoiseFilters_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HFRecHit.h"

#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"



class HcalTupleMaker_HcalNoiseFilters : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalNoiseFilters(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  int hcalSevLvl(const CaloRecHit* hit);

  edm::EDGetTokenT<HcalNoiseSummary>     noiseSummaryInputToken;
  std::string     noiseResultInputTag;
  edm::EDGetTokenT<HBHERecHitCollection> recoInputToken;
  edm::EDGetTokenT<HFRecHitCollection> recoHFInputToken;

  //edm::InputTag   noiseSummaryInputTag;

  // std::string     recoInputTag;
  //edm::EDGetTokenT<HBHERecHitCollection> recoInputToken;

  bool            isRAW;
  bool            isRECO;
  std::string     prefix,suffix;
    
  const HcalTopology* theHcalTopology;
  const HcalChannelQuality* dbHcalChStatus;
  const HcalSeverityLevelComputer* hcalSevLvlComputer;

  edm::EDGetTokenT<bool> defaultNoiseResultInputToken;
  edm::EDGetTokenT<bool> run1NoiseResultInputToken;
  edm::EDGetTokenT<bool> run2LooseNoiseResultInputToken;
  edm::EDGetTokenT<bool> run2TightNoiseResultInputToken;
  edm::EDGetTokenT<bool> isoNoiseResultInputToken;

};

#endif
