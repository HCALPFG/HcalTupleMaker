#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_MuonTrack.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

HcalTupleMaker_MuonTrack::HcalTupleMaker_MuonTrack(const edm::ParameterSet& iConfig):
  //recoInputTag         (iConfig.getUntrackedParameter<std::string>("recoInputTag")),
  inputMuonCollection            (iConfig.getParameter<edm::InputTag>("inputMuonCollection")),
  inputTrackCollection           (iConfig.getParameter<edm::InputTag>("inputTrackCollection")),
  CSCSegmentsCollection          (iConfig.getParameter<edm::InputTag>("CSCSegmentsCollection")),
  CSC2DRecHitsCollection         (iConfig.getParameter<edm::InputTag>("CSC2DRecHitsCollection")),
  RPCRecHitsCollection           (iConfig.getParameter<edm::InputTag>("RPCRecHitsCollection")),
  DT1DRecHitsCollection          (iConfig.getParameter<edm::InputTag>("DT1DRecHitsCollection")),
  DT1DCosmicRecHitsCollection    (iConfig.getParameter<edm::InputTag>("DT1DCosmicRecHitsCollection")),
  DTRecSegmentsCollection        (iConfig.getParameter<edm::InputTag>("DTRecSegmentsCollection")),
  DTRecCosmicSegmentsCollection  (iConfig.getParameter<edm::InputTag>("DTRecCosmicSegmentsCollection")),
  prefix                         (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix                         (iConfig.getUntrackedParameter<std::string>("Suffix")),
  muonCollection_                (consumes<reco::MuonCollection>(inputMuonCollection)),
  trackCollection_               (consumes<reco::TrackCollection>(inputTrackCollection)),
  CSCSegmentsCollection_         (consumes<CSCSegmentCollection>(CSCSegmentsCollection)),
  CSC2DRecHitsCollection_        (consumes<CSCRecHit2DCollection>(CSC2DRecHitsCollection)),
  RPCRecHitsCollection_          (consumes<RPCRecHitCollection>(RPCRecHitsCollection)),
  DT1DRecHitsCollection_         (consumes<DTRecHitCollection>(DT1DRecHitsCollection)),
  DT1DCosmicRecHitsCollection_   (consumes<DTRecHitCollection>(DT1DCosmicRecHitsCollection)),
  DTRecSegmentsCollection_       (consumes<DTRecSegment4DCollection>(DTRecSegmentsCollection)),
  DTRecCosmicSegmentsCollection_ (consumes<DTRecSegment4DCollection>(DTRecCosmicSegmentsCollection))
  /*
  CSCSegmentsCollection_         (consumes<CSCSegmentCollection>(edm::InputTag{"cscSegments"})),
  CSC2DRecHitsCollection_        (consumes<CSCRecHit2DCollection>(edm::InputTag{"csc2DRecHits"})),
  RPCRecHitsCollection_          (consumes<RPCRecHitCollection>(edm::InputTag{"rpcRecHits"})),
  DT1DRecHitsCollection_         (consumes<DTRecHitCollection>(edm::InputTag{"dt1DRecHits"})),
  DT1DCosmicRecHitsCollection_   (consumes<DTRecHitCollection>(edm::InputTag{"dt1DCosmicRecHits"})),
  DTRecSegmentsCollection_       (consumes<DTRecSegment4DCollection>(edm::InputTag{"dt4DSegments"})),
  DTRecCosmicSegmentsCollection_ (consumes<DTRecSegment4DCollection>(edm::InputTag{"dt4DCosmicSegments"}))
  */
{
  produces <std::vector<double> >            (prefix + "MuonEta"                        + suffix );
  produces <std::vector<double> >            (prefix + "MuonPt"                         + suffix );
  produces <std::vector<double> >            (prefix + "MuonPhi"                        + suffix );
  produces <std::vector<double> >            (prefix + "MuonCalEnergyHadS9"             + suffix );
  produces <std::vector<double> >            (prefix + "MuonCalEnergyHad"               + suffix );
  produces <std::vector<double> >            (prefix + "MuonCalEnergyEmS25"             + suffix );
  produces <std::vector<double> >            (prefix + "MuonCalEnergyEm"                + suffix );
  produces <std::vector<int> >               (prefix + "MuonNumberOfChambers"           + suffix );
  produces <std::vector<int> >               (prefix + "MuonNumberOfMatchedStations"    + suffix );
  produces <std::vector<int> >               (prefix + "MuonNumberOfMatchedRPCLayers"   + suffix );
  //
  produces <std::vector<int> >               (prefix + "MuonCSC2DRecHitsSize"           + suffix );
  produces <std::vector<int> >               (prefix + "MuonRPCRecHitsSize"             + suffix );
  produces <std::vector<int> >               (prefix + "MuonDT1DRecHitsSize"            + suffix );
  produces <std::vector<int> >               (prefix + "MuonDT1DCosmicRecHitsSize"      + suffix );
  produces <std::vector<int> >               (prefix + "MuonCSCSegmentsSize"            + suffix );
  produces <std::vector<int> >               (prefix + "MuonDTRecSegmentsSize"          + suffix );
  produces <std::vector<int> >               (prefix + "MuonDTRecCosmicSegmentsSize"    + suffix );
}

void HcalTupleMaker_MuonTrack::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  std::auto_ptr<std::vector<double> >            muoneta       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            muonpt        ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            muonphi       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            muoncalenergyhads9     ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            muoncalenergyhad       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            muoncalenergyems25     ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            muoncalenergyem        ( new std::vector<double>           ());
  std::auto_ptr<std::vector<int> >               muonnumberofchambers         ( new std::vector<int>               ());
  std::auto_ptr<std::vector<int> >               muonnumberofmatchedstations  ( new std::vector<int>               ());
  std::auto_ptr<std::vector<int> >               muonnumberofmatchedrpclayers ( new std::vector<int>               ());
  //
  std::auto_ptr<std::vector<int> >               muoncsc2drechitssize         ( new std::vector<int>               ());
  std::auto_ptr<std::vector<int> >               muonrpcrechitssize           ( new std::vector<int>               ());
  std::auto_ptr<std::vector<int> >               muondt1drechitssize          ( new std::vector<int>               ());
  std::auto_ptr<std::vector<int> >               muondt1dcosmicrechitssize    ( new std::vector<int>               ());
  std::auto_ptr<std::vector<int> >               muoncscsegmentssize          ( new std::vector<int>               ());
  std::auto_ptr<std::vector<int> >               muondtrecsegmentssize        ( new std::vector<int>               ());
  std::auto_ptr<std::vector<int> >               muondtreccosmicsegmentssize  ( new std::vector<int>               ());

  //Get Muon DT, CSC, RPC rechits, segments:
  edm::Handle<CSCSegmentCollection> hCSCSegments;
  iEvent.getByToken(CSCSegmentsCollection_, hCSCSegments);
  edm::Handle<CSCRecHit2DCollection> hCSC2DRecHits;
  iEvent.getByToken(CSC2DRecHitsCollection_, hCSC2DRecHits);
  edm::Handle<RPCRecHitCollection> hRPCRecHits;
  iEvent.getByToken(RPCRecHitsCollection_, hRPCRecHits);
  edm::Handle<DTRecHitCollection> hDT1DRecHits;
  iEvent.getByToken(DT1DRecHitsCollection_, hDT1DRecHits);
  edm::Handle<DTRecHitCollection> hDT1DCosmicRecHits;
  iEvent.getByToken(DT1DCosmicRecHitsCollection_, hDT1DCosmicRecHits);
  edm::Handle<DTRecSegment4DCollection> hDTRecSegments;
  iEvent.getByToken(DTRecSegmentsCollection_, hDTRecSegments);
  edm::Handle<DTRecSegment4DCollection> hDTRecCosmicSegments;
  iEvent.getByToken(DTRecCosmicSegmentsCollection_, hDTRecCosmicSegments);
  //
  //  std::cout<<"        hCSC2DRecHits.size: "<<(*hCSC2DRecHits).size()<<std::endl;
  //    http://cmslxr.fnal.gov/lxr/source/DPGAnalysis/Skims/src/CSCSkim.cc?v=CMSSW_7_5_0_pre5#0380
  //    CSCRecHit2DCollection::const_iterator cscrh = hCSC2DRecHits->begin ();
  //    for (; cscrh!=hCSC2DRecHits->end (); cscrh++){}
  //  std::cout<<"          hRPCRecHits.size: "<<(*hRPCRecHits).size()<<std::endl;
  //  std::cout<<"         hDT1DRecHits.size: "<<(*hDT1DRecHits).size()<<std::endl;
  //  std::cout<<"   hDT1DCosmicRecHits.size: "<<(*hDT1DCosmicRecHits).size()<<std::endl;
  //  std::cout<<"         hCSCSegments.size: "<<(*hCSCSegments).size()<<std::endl;
  //  std::cout<<"       hDTRecSegments.size: "<<(*hDTRecSegments).size()<<std::endl;
  //  std::cout<<" hDTRecCosmicSegments.size: "<<(*hDTRecCosmicSegments).size()<<std::endl;
  //
  muoncsc2drechitssize        -> push_back( (*hCSC2DRecHits).size()        );
  muonrpcrechitssize          -> push_back( (*hRPCRecHits).size()          );
  muondt1drechitssize         -> push_back( (*hDT1DRecHits).size()         );
  muondt1dcosmicrechitssize   -> push_back( (*hDT1DCosmicRecHits).size()   );
  muoncscsegmentssize         -> push_back( (*hCSCSegments).size()         );
  muondtrecsegmentssize       -> push_back( (*hDTRecSegments).size()       );
  muondtreccosmicsegmentssize -> push_back( (*hDTRecCosmicSegments).size() );

  //// Tracks, for later if needed:
  //edm::Handle<reco::TrackCollection> hTracks;
  //iEvent.getByToken(trackCollection_, hTracks);
  //
  //if( fabs((*hCaloMET)[0].px())>50 || fabs((*hCaloMET)[0].py())>50 ){
  //std::cout<<"   hTracks.size: "<<(*hTracks).size()<<std::endl;
  //reco::TrackCollection::const_iterator tr = hTracks->begin ();
  //for (; tr !=hTracks->end (); tr++){
  //std::cout<<"   hTracks.eta: "<<(*tr).eta()<<std::endl;
  //std::cout<<"   hTracks.phi: "<<(*tr).phi()<<std::endl;
  //std::cout<<"    hTracks.pt: "<<(*tr).pt()<<std::endl;
  //}
  //std::cout<<std::endl;
  //}

  edm::Handle<reco::MuonCollection> hMuons;
  iEvent.getByToken(muonCollection_, hMuons);
  //std::cout<<"   hMuons.size: "<<(*hMuons).size()<<std::endl;

  reco::MuonCollection::const_iterator mu = hMuons->begin ();
  for (; mu !=hMuons->end (); mu++){
    muoneta -> push_back( (*mu).eta() );
    muonpt  -> push_back( (*mu).pt()  );
    muonphi -> push_back( (*mu).phi() );
    muoncalenergyhads9 -> push_back( (*mu).calEnergy().hadS9 );
    muoncalenergyhad   -> push_back( (*mu).calEnergy().had   );
    muoncalenergyems25 -> push_back( (*mu).calEnergy().emS25 );
    muoncalenergyem    -> push_back( (*mu).calEnergy().em    );
    muonnumberofchambers         -> push_back( (*mu).numberOfChambers()         );
    muonnumberofmatchedstations  -> push_back( (*mu).numberOfMatchedStations()  );
    muonnumberofmatchedrpclayers -> push_back( (*mu).numberOfMatchedRPCLayers() );
    //
    //std::cout<<"             hMuons.pt: "<<(*mu).pt()<<std::endl;
    //std::cout<<"            hMuons.eta: "<<(*mu).eta()<<std::endl;
    //std::cout<<"            hMuons.phi: "<<(*mu).phi()<<std::endl;
    //std::cout<<"hMuons.calEnergy.hadS9: "<<(*mu).calEnergy().hadS9 <<std::endl;
    //std::cout<<"hMuons.calEnergy.emS25: "<<(*mu).calEnergy().emS25 <<std::endl;
    //std::cout<<"  hMuons.calEnergy.had: "<<(*mu).calEnergy().had <<std::endl;
    //std::cout<<"   hMuons.calEnergy.em: "<<(*mu).calEnergy().em <<std::endl;
    //std::cout<<"        hMuons.numberOfChambers: "<<(*mu).numberOfChambers()<<std::endl;
    //std::cout<<" hMuons.numberOfMatchedStations: "<<(*mu).numberOfMatchedStations()<<std::endl;
    //std::cout<<"hMuons.numberOfMatchedRPCLayers: "<<(*mu).numberOfMatchedRPCLayers()<<std::endl;
    //std::cout<<"       Muons.isolationR05.hadEt: "<<(*mu).isolationR05().hadEt<<std::endl;
    //std::cout<<"       hMuons.isolationR05.emEt: "<<(*mu).isolationR05().emEt<<std::endl;
  }
  
  iEvent.put(muoneta    , prefix + "MuonEta"                        + suffix );
  iEvent.put(muonpt     , prefix + "MuonPt"                         + suffix );
  iEvent.put(muonphi    , prefix + "MuonPhi"                        + suffix );
  //
  iEvent.put(muoncalenergyhads9 ,           prefix + "MuonCalEnergyHadS9"             + suffix );
  iEvent.put(muoncalenergyhad   ,           prefix + "MuonCalEnergyHad"               + suffix );
  iEvent.put(muoncalenergyems25 ,           prefix + "MuonCalEnergyEmS25"             + suffix );
  iEvent.put(muoncalenergyem    ,           prefix + "MuonCalEnergyEm"                + suffix );
  iEvent.put(muonnumberofchambers         , prefix + "MuonNumberOfChambers"           + suffix );
  iEvent.put(muonnumberofmatchedstations  , prefix + "MuonNumberOfMatchedStations"    + suffix );
  iEvent.put(muonnumberofmatchedrpclayers , prefix + "MuonNumberOfMatchedRPCLayers"   + suffix );
  //
  iEvent.put(muoncsc2drechitssize,          prefix + "MuonCSC2DRecHitsSize"           + suffix );
  iEvent.put(muonrpcrechitssize  ,          prefix + "MuonRPCRecHitsSize"             + suffix );
  iEvent.put(muondt1drechitssize ,          prefix + "MuonDT1DRecHitsSize"            + suffix );
  iEvent.put(muondt1dcosmicrechitssize  ,   prefix + "MuonDT1DCosmicRecHitsSize"      + suffix );
  iEvent.put(muoncscsegmentssize        ,   prefix + "MuonCSCSegmentsSize"            + suffix );
  iEvent.put(muondtrecsegmentssize      ,   prefix + "MuonDTRecSegmentsSize"          + suffix );
  iEvent.put(muondtreccosmicsegmentssize,   prefix + "MuonDTRecCosmicSegmentsSize"    + suffix );
}
