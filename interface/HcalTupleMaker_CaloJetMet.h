#ifndef HcalTupleMaker_CaloJetMet_h
#define HcalTupleMaker_CaloJetMet_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

class HcalTupleMaker_CaloJetMet : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_CaloJetMet(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  std::string     noiseResultInputTag;
  std::string     recoInputTag;
  std::string     prefix,suffix;

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
  void CalculateTotalEnergiesHBHE( const edm::SortedCollection<HBHERecHit>& );
  //void CalculateTotalEnergiesEB(const EcalRecHitCollection &RecHits);
  void CalculateTotalEnergiesEB(const edm::SortedCollection<EcalRecHit> &);
  void CalculateTotalEnergiesEE(const edm::SortedCollection<EcalRecHit> &);
  const CaloGeometry *Geometry;

};

#endif
