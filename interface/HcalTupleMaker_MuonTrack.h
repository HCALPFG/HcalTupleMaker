#ifndef HcalTupleMaker_MuonTrack_h
#define HcalTupleMaker_MuonTrack_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
//#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
//#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"

class HcalTupleMaker_MuonTrack : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_MuonTrack(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag   inputMuonCollection;
  edm::InputTag   inputTrackCollection;
  edm::InputTag   CSCSegmentsCollection;
  edm::InputTag   CSC2DRecHitsCollection;
  edm::InputTag   RPCRecHitsCollection;
  edm::InputTag   DT1DRecHitsCollection;
  edm::InputTag   DT1DCosmicRecHitsCollection;
  edm::InputTag   DTRecSegmentsCollection;
  edm::InputTag   DTRecCosmicSegmentsCollection;
  std::string     prefix,suffix;
  //
  edm::EDGetTokenT<reco::MuonCollection>     muonCollection_;
  edm::EDGetTokenT<reco::TrackCollection>    trackCollection_;
  edm::EDGetTokenT<CSCSegmentCollection>     CSCSegmentsCollection_;
  edm::EDGetTokenT<CSCRecHit2DCollection>    CSC2DRecHitsCollection_;
  edm::EDGetTokenT<RPCRecHitCollection>      RPCRecHitsCollection_;
  edm::EDGetTokenT<DTRecHitCollection>       DT1DRecHitsCollection_;
  edm::EDGetTokenT<DTRecHitCollection>       DT1DCosmicRecHitsCollection_;
  edm::EDGetTokenT<DTRecSegment4DCollection> DTRecSegmentsCollection_;
  edm::EDGetTokenT<DTRecSegment4DCollection> DTRecCosmicSegmentsCollection_;
};

#endif
