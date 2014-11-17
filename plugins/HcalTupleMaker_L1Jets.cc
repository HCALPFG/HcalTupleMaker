#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_L1Jets.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalPFGGeometry.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

HcalTupleMaker_L1Jets::HcalTupleMaker_L1Jets(const edm::ParameterSet& iConfig):
  inputTags  (iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("source")),
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

  int nL1JetCollections = inputTags.size();
  std::vector<edm::Handle<l1extra::L1JetParticleCollection> > l1Jets ( nL1JetCollections );

  edm::Handle<HcalTrigPrimDigiCollection> tps;
  iEvent.getByLabel(tpInputTag, tps);

  HcalTrigPrimDigiCollection::const_iterator itp;
  HcalTrigPrimDigiCollection::const_iterator first_tp = tps -> begin();
  
  std::vector<HcalTrigTowerDetId> detids;
  
  for (int iL1JetCollection = 0; iL1JetCollection < nL1JetCollections; ++iL1JetCollection){

    // Get jet collection from event
    iEvent.getByLabel(inputTags[iL1JetCollection], l1Jets[iL1JetCollection]);
    l1extra::L1JetParticleCollection::const_iterator l1Jet     = l1Jets[iL1JetCollection] -> begin();
    l1extra::L1JetParticleCollection::const_iterator l1Jet_end = l1Jets[iL1JetCollection] -> end();

    for(; l1Jet != l1Jet_end; ++l1Jet){
      pt      -> push_back ( l1Jet -> pt  ());
      eta     -> push_back ( l1Jet -> eta ());
      phi     -> push_back ( l1Jet -> phi ());
      bx      -> push_back ( l1Jet -> bx  ());
      type    -> push_back ( l1Jet -> type());
      tpIndex -> push_back ( std::vector<int>() );
    
      size_t last_entry = tpIndex -> size() - 1;
      
      L1CaloRegionDetId regionId = l1Jet -> gctJetCand() -> regionId();
      hcalpfg::getTriggerTowerIDs(regionId.ieta(), regionId.iphi(), detids);
      
      int ndetids = detids.size();
      for (int idetid = 0; idetid < ndetids; ++idetid){
	itp = tps -> find ( detids[idetid] );
	int index = itp - first_tp;
	if ( itp != tps -> end()){
	  (*tpIndex)[last_entry].push_back ( index );
	}
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
