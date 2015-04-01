#include "HCALPFG/HcalTupleMaker/interface/HcalCosmicDigisProducer.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"


HcalCosmicDigisProducer::HcalCosmicDigisProducer(const edm::ParameterSet& iConfig):
  m_hbheDigisTag     (iConfig.getParameter<edm::InputTag>("HBHEDigis")),
  m_hoDigisTag       (iConfig.getParameter<edm::InputTag>("HODigis")),
  m_hfDigisTag       (iConfig.getParameter<edm::InputTag>("HFDigis")),
  m_recoTracksTag    (iConfig.getParameter<edm::InputTag>("RecoTracks")),
  m_trackParameterSet(iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters"))  
{
  produces<HBHEDigiCollection>();
  produces<HODigiCollection>  ();
  produces<HFDigiCollection>  ();
  edm::ConsumesCollector iC = consumesCollector();
  m_trackParameters.loadParameters( m_trackParameterSet, iC );
  m_trackAssociator.useDefaultPropagator();
}

HcalCosmicDigisProducer::~HcalCosmicDigisProducer(){}

void HcalCosmicDigisProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  edm::Handle<reco::TrackCollection> recoTracks;
  iEvent.getByLabel(m_recoTracksTag, recoTracks);  

  edm::Handle<HBHEDigiCollection> hbheInputDigis;
  edm::Handle<HODigiCollection>   hoInputDigis;
  edm::Handle<HFDigiCollection>   hfInputDigis;

  iEvent.getByLabel(m_hbheDigisTag, hbheInputDigis);
  iEvent.getByLabel(m_hoDigisTag  , hoInputDigis);
  iEvent.getByLabel(m_hfDigisTag  , hfInputDigis);

  std::auto_ptr<HBHEDigiCollection> hbheOutputDigis(new HBHEDigiCollection()); 
  std::auto_ptr<HODigiCollection>   hoOutputDigis  (new HODigiCollection());
  std::auto_ptr<HFDigiCollection>   hfOutputDigis  (new HFDigiCollection());

  reco::TrackCollection::const_iterator recoTrack     = recoTracks -> begin();
  reco::TrackCollection::const_iterator recoTrack_end = recoTracks -> end();
  
  std::map<int,std::vector<DetId> > trackDetIdMap;
  
  for(; recoTrack != recoTrack_end; ++recoTrack){

    TrackDetMatchInfo info = m_trackAssociator.associate(iEvent, iSetup, *recoTrack, m_trackParameters);
    
    std::vector<DetId> crossedHcalIds       = info.crossedHcalIds;
    std::vector<DetId>::iterator hcalId     = crossedHcalIds.begin();
    std::vector<DetId>::iterator hcalId_end = crossedHcalIds.end();

    for (; hcalId != hcalId_end; ++hcalId){
      HcalDetId hcalDetId ( *hcalId );
      if (hcalDetId.subdet() == HcalBarrel || hcalDetId.subdet() == HcalEndcap ){
	HBHEDigiCollection::const_iterator hbheDigi = hbheInputDigis -> find(hcalDetId);
	if ( hbheDigi != hbheInputDigis -> end()) hbheOutputDigis -> push_back ( *hbheDigi );
      }
      
      if (hcalDetId.subdet() == HcalForward){
	HFDigiCollection::const_iterator hfDigi = hfInputDigis -> find(hcalDetId);
	if ( hfDigi != hfInputDigis -> end()) hfOutputDigis -> push_back ( *hfDigi );
      }
    }
    
    std::vector<DetId> crossedHOIds       = info.crossedHOIds;
    std::vector<DetId>::iterator hoId     = crossedHOIds.begin();
    std::vector<DetId>::iterator hoId_end = crossedHOIds.end();
    
    for (; hoId != hoId_end; ++hoId){
      HcalDetId hcalDetId ( *hoId );
      HODigiCollection::const_iterator hoDigi = hoInputDigis -> find(hcalDetId);
      if ( hoDigi != hoInputDigis -> end() ) hoOutputDigis -> push_back ( *hoDigi );
    }
  }

  iEvent.put(hbheOutputDigis);
  iEvent.put(hoOutputDigis  );
  iEvent.put(hfOutputDigis  );
  
}

void HcalCosmicDigisProducer::beginJob(){}

void HcalCosmicDigisProducer::endJob() {}

void HcalCosmicDigisProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
