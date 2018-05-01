#ifndef HcalTupleMaker_QIE11Digis_h
#define HcalTupleMaker_QIE11Digis_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalUMNioDigi.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"

class HcalTupleMaker_QIE11Digis : public edm::EDProducer {
public:
  explicit HcalTupleMaker_QIE11Digis(const edm::ParameterSet&);

private:
  void produce( edm::Event &, const edm::EventSetup & ); 
  std::string prefix,suffix; 
  bool storelaser;
  const edm::InputTag _taguMNio;
  const edm::InputTag m_qie11DigisTag;
  double chargeskim;
  
  edm::EDGetTokenT<HcalDataFrameContainer<QIE11DataFrame> > qie11digisToken_;
  edm::EDGetTokenT<HcalUMNioDigi> _tokuMNio;
};

#endif
