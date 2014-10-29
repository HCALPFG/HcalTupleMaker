#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_L1Jets.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

//-----------------------------------------------------------------------------
// Beware: no official mapping from L1CaloRegions to HcalTrigTowerDetId exists
// This "roll your own" mapping is based on this pre-upgrade RCT map:
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/RCTMap
//-----------------------------------------------------------------------------

void getTriggerTowerIEtas( int gctEta, std::vector<int> & ttIEtas ) {
  ttIEtas.clear();
  if       (gctEta <= 3 ) ttIEtas.push_back(gctEta - 32);
  else if  (gctEta >= 18) ttIEtas.push_back(gctEta + 11);
  else {
    int sign, start;
    if (gctEta <= 10){
      sign  = -1;
      start = -1 - (4*(10 - gctEta));
    }
    else {
      sign  = 1;
      start = 1 + (4*(gctEta - 11));
    }
    for (int i = 0; i < 4; ++i) ttIEtas.push_back ( start + (i*sign));
  }
}

void getTriggerTowerIPhis( int rctPhi, std::vector<int> & ttIPhis ){
  ttIPhis.clear();
  int iphi;
  int start = (((rctPhi * 4) - 2) % 72) + 1;
  for (int i = 0; i < 4; ++i){
    iphi = start + i;
    if (iphi > 72) iphi -= 72;
    ttIPhis.push_back( iphi );
  }
}

void getTriggerTowerIDs( int gctEta,int rctPhi, std::vector<HcalTrigTowerDetId> & ids ){
  ids.clear();
  std::vector<int> ttIEtas, ttIPhis;
  getTriggerTowerIPhis( rctPhi, ttIPhis );
  getTriggerTowerIEtas( gctEta, ttIEtas );
  int nIPhis = ttIPhis.size();
  int nIEtas = ttIEtas.size();
  for (int iiphi = 0; iiphi < nIPhis; ++iiphi){
    for (int iieta = 0; iieta < nIEtas; ++iieta){
      ids.push_back(HcalTrigTowerDetId(ttIEtas[iieta], ttIPhis[iiphi]));
    }
  }
}

HcalTupleMaker_L1Jets::HcalTupleMaker_L1Jets(const edm::ParameterSet& iConfig):
  inputTag   (iConfig.getUntrackedParameter<edm::InputTag>("source")),
  tpInputTag (iConfig.getUntrackedParameter<edm::InputTag>("hcalTPs")),
  prefix     (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix     (iConfig.getUntrackedParameter<std::string>("Suffix"))
{
  produces <std::vector<double> >            (prefix + "Pt"            + suffix );
  produces <std::vector<double> >            (prefix + "Eta"           + suffix );
  produces <std::vector<double> >            (prefix + "Phi"           + suffix );
  produces <std::vector<int> >               (prefix + "Type"          + suffix );
  produces <std::vector<int> >               (prefix + "BX"            + suffix );
  produces <std::vector<std::vector<int> > > (prefix + "TrigPrimIndex" + suffix );
}

void HcalTupleMaker_L1Jets::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  std::auto_ptr<std::vector<double> >            pt      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            eta     ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            phi     ( new std::vector<double>           ());
  std::auto_ptr<std::vector<int   > >            type    ( new std::vector<int>              ());
  std::auto_ptr<std::vector<int   > >            bx      ( new std::vector<int>              ());
  std::auto_ptr<std::vector<std::vector<int> > > tpIndex ( new std::vector<std::vector<int> >());
  
  edm::Handle<l1extra::L1JetParticleCollection> l1Jets;
  iEvent.getByLabel(inputTag, l1Jets);

  l1extra::L1JetParticleCollection::const_iterator l1Jet     = l1Jets -> begin();
  l1extra::L1JetParticleCollection::const_iterator l1Jet_end = l1Jets -> end();
  
  edm::Handle<HcalTrigPrimDigiCollection> tps;
  iEvent.getByLabel(tpInputTag, tps);

  HcalTrigPrimDigiCollection::const_iterator itp;
  HcalTrigPrimDigiCollection::const_iterator first_tp = tps -> begin();
  
  std::vector<HcalTrigTowerDetId> detids;
  
  for(; l1Jet != l1Jet_end; ++l1Jet){
    pt      -> push_back ( l1Jet -> pt  ());
    eta     -> push_back ( l1Jet -> eta ());
    phi     -> push_back ( l1Jet -> phi ());
    bx      -> push_back ( l1Jet -> bx  ());
    type    -> push_back ( l1Jet -> type());
    tpIndex -> push_back ( std::vector<int>() );
    
    size_t last_entry = tpIndex -> size();
    
    L1CaloRegionDetId regionId = l1Jet -> gctJetCand() -> regionId();
    getTriggerTowerIDs(regionId.ieta(), regionId.iphi(), detids);

    int ndetids = detids.size();
    for (int idetid = 0; idetid < ndetids; ++idetid){
      itp = tps -> find ( detids[idetid] );
      int index = itp - first_tp;
      if ( itp != tps -> end()){
	(*tpIndex)[last_entry].push_back ( index );
      }
    }
  }

  iEvent.put(pt     , prefix + "Pt"            + suffix );
  iEvent.put(eta    , prefix + "Eta"           + suffix );
  iEvent.put(phi    , prefix + "Phi"           + suffix );
  iEvent.put(bx     , prefix + "BX"            + suffix );
  iEvent.put(type   , prefix + "Type"           + suffix );
  iEvent.put(tpIndex, prefix + "TrigPrimIndex" + suffix );
  
}
