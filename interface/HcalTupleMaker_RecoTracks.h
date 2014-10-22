#ifndef HcalTupleMaker_RecoTracks_h
#define HcalTupleMaker_RecoTracks_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_RecoTracks : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_RecoTracks(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
};

#endif
