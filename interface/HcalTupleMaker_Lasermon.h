#ifndef HcalTupleMaker_Lasermon_h
#define HcalTupleMaker_Lasermon_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

class HcalTupleMaker_Lasermon : public edm::EDProducer {
public:
  explicit HcalTupleMaker_Lasermon(const edm::ParameterSet&);

private:
  void produce( edm::Event &, const edm::EventSetup & ); 
  std::string prefix,suffix; 
  const edm::InputTag m_lasermonDigisTag;
  edm::EDGetTokenT<QIE10DigiCollection> lasermonDigisToken_;

  std::vector<int> laserMonIPhiList_;
  std::vector<int> laserMonIEtaList_;
  std::vector<int> laserMonCBoxList_;

  int laserMonitorTSStart_;
  int laserMonitorTSEnd_;


};

#endif
