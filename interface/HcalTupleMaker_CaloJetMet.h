#ifndef HcalTupleMaker_CaloJetMet_h
#define HcalTupleMaker_CaloJetMet_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HFRecHit.h"
#include "DataFormats/HcalRecHit/interface/HORecHit.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"



class HcalTupleMaker_CaloJetMet : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_CaloJetMet(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  //std::string     noiseResultInputTag;
  edm::EDGetTokenT<HBHERecHitCollection>    recoInputToken;
  std::string                               prefix,suffix;
  edm::EDGetTokenT<EcalRecHitCollection>    EcalRecHitsEBToken;
  edm::EDGetTokenT<EcalRecHitCollection>    EcalRecHitsEEToken;
  edm::EDGetTokenT<reco::CaloJetCollection> ak4CaloJetsToken;
  edm::EDGetTokenT<reco::CaloMETCollection> caloMetToken;

  //std::string     recoInputTag;

 private:
  // Sum \vec{ET}
  double EBET[2];
  double EEET[2];
  double HBET[2];
  double HEET[2];
  double HFET[2];
  double NominalMET[2];
  // Sum |E|
  double EBSumE;
  double EESumE;
  double HBSumE;
  double HESumE;
  double HFSumE;
  // Sum |ET|
  double EBSumET;
  double EESumET;
  double HBSumET;
  double HESumET;
  double HFSumET;

 private:
  void ClearVariables();
  //void CalculateTotalEnergiesHBHE( const edm::Handle<HBHERecHitCollection> &Rechits );
  //void CalculateTotalEnergiesEB(const EcalRecHitCollection &RecHits);
  void CalculateTotalEnergiesHBHE( const edm::SortedCollection<HBHERecHit>& );
  void CalculateTotalEnergiesEB(const edm::SortedCollection<EcalRecHit> &);
  void CalculateTotalEnergiesEE(const edm::SortedCollection<EcalRecHit> &);
  const CaloGeometry *Geometry;

};

#endif
