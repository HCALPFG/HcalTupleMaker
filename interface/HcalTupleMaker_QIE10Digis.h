#ifndef HcalTupleMaker_QIE10Digis_h
#define HcalTupleMaker_QIE10Digis_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
//#include "DataFormats/HcalDetId/interface/HcalDetId.h"

class HcalTupleMaker_QIE10Digis : public edm::EDProducer {
public:
  explicit HcalTupleMaker_QIE10Digis(const edm::ParameterSet&);

private:
  void produce( edm::Event &, const edm::EventSetup & ); 
  std::string prefix,suffix; 
  const edm::InputTag m_qie10DigisTag;
  edm::EDGetTokenT<QIE10DigiCollection> qie10digisToken_;
};

#endif
