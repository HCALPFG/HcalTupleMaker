#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_RecoTracks.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

HcalTupleMaker_RecoTracks::HcalTupleMaker_RecoTracks(const edm::ParameterSet& iConfig):
  inputTag   (iConfig.getUntrackedParameter<edm::InputTag>("source")),
  prefix     (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix     (iConfig.getUntrackedParameter<std::string>("Suffix"))
{
  produces <std::vector<double> > (prefix + "Pt"  + suffix );
  produces <std::vector<double> > (prefix + "Eta" + suffix );
  produces <std::vector<double> > (prefix + "Phi" + suffix );
  produces <std::vector<double> > (prefix + "D0"  + suffix );
  produces <std::vector<double> > (prefix + "DZ"  + suffix );
  produces <std::vector<int   > > (prefix + "NValidHits"  + suffix );
}

void HcalTupleMaker_RecoTracks::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  std::auto_ptr<std::vector<double> > pt  ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > eta ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > phi ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > d0  ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > dz  ( new std::vector<double> () );
  std::auto_ptr<std::vector<int> > nValidHits  ( new std::vector<int> () );
  
  edm::Handle<reco::TrackCollection> recoTracks;
  iEvent.getByLabel(inputTag, recoTracks);  
  
  reco::TrackCollection::const_iterator recoTrack     = recoTracks -> begin();
  reco::TrackCollection::const_iterator recoTrack_end = recoTracks -> end();
  
  for(; recoTrack != recoTrack_end; ++recoTrack){
    pt  -> push_back ( recoTrack -> pt ());
    eta -> push_back ( recoTrack -> eta());
    phi -> push_back ( recoTrack -> phi());
    d0  -> push_back ( recoTrack -> d0 ());
    dz  -> push_back ( recoTrack -> dz ());
    nValidHits  -> push_back ( recoTrack -> numberOfValidHits ());
  }

  iEvent.put(pt  , prefix + "Pt"  + suffix );
  iEvent.put(eta , prefix + "Eta" + suffix );
  iEvent.put(phi , prefix + "Phi" + suffix );
  iEvent.put(d0  , prefix + "D0"  + suffix );
  iEvent.put(dz  , prefix + "DZ"  + suffix );
  iEvent.put(nValidHits , prefix + "NValidHits" + suffix );
  
}
