#ifndef HcalTupleMaker_HcalIsoNoiseFilterParameters_h
#define HcalTupleMaker_HcalIsoNoiseFilterParameters_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
#include "RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseShapes.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoLocalCalo/HcalRecProducers/src/HBHEIsolatedNoiseReflagger.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HBHEIsolatedNoiseAlgos.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"
#include "DataFormats/METReco/interface/HcalCaloFlagLabels.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "FWCore/Framework/interface/Event.h"
#include <iterator>
#include <vector>


class HcalTupleMaker_HcalIsoNoiseFilterParameters : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalIsoNoiseFilterParameters(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  //edm::InputTag   noiseSummaryInputTag;
  edm::EDGetTokenT<HcalNoiseSummary> noiseSummaryInputToken;
  //std::string     noiseResultInputTag;
  //std::string     recoInputTag;
  //bool            isRAW;
  std::string     prefix,suffix;
  edm::EDGetTokenT<HBHERecHitCollection> hbheprerecoToken;
  edm::EDGetTokenT<EcalRecHitCollection> EcalRecHitsEBToken;
  edm::EDGetTokenT<EcalRecHitCollection> EcalRecHitsEEToken;
  edm::EDGetTokenT<std::vector<reco::TrackExtrapolation> > trackExtrapolatorToken;

};

#endif
