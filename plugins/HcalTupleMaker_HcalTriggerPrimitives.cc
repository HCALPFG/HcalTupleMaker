#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalTriggerPrimitives.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"

HcalTupleMaker_HcalTriggerPrimitives::HcalTupleMaker_HcalTriggerPrimitives(const edm::ParameterSet& iConfig):
  inputTag    (iConfig.getUntrackedParameter<edm::InputTag>("source")),
  hbheInputTag(iConfig.getUntrackedParameter<edm::InputTag>("hbheDigis")),
  hfInputTag  (iConfig.getUntrackedParameter<edm::InputTag>("hfDigis")),
  prefix      (iConfig.getUntrackedParameter<std::string>  ("Prefix")),
  suffix      (iConfig.getUntrackedParameter<std::string>  ("Suffix"))
{
  produces <std::vector<int> >               (prefix + "IEta"            + suffix );
  produces <std::vector<int> >               (prefix + "IPhi"            + suffix );
  produces <std::vector<int> >               (prefix + "CompressedEtSOI" + suffix );
  produces <std::vector<int> >               (prefix + "FineGrainSOI"    + suffix );
  produces <std::vector<int> >               (prefix + "Size"            + suffix );
  produces <std::vector<int> >               (prefix + "Presamples"      + suffix );
  produces <std::vector<std::vector<int> > > (prefix + "CompressedEt"    + suffix );
  produces <std::vector<std::vector<int> > > (prefix + "FineGrain"       + suffix );
  produces <std::vector<std::vector<int> > > (prefix + "HBHEDigiIndex"   + suffix );
  produces <std::vector<std::vector<int> > > (prefix + "HFDigiIndex"     + suffix );

  tpToken_ = consumes<HcalTrigPrimDigiCollection>(inputTag);
  hbheToken_ = consumes<HBHEDigiCollection>(hbheInputTag);
  hfToken_ = consumes<HFDigiCollection>(hfInputTag);
}

void HcalTupleMaker_HcalTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

  std::unique_ptr<std::vector<int   > >            ieta              ( new std::vector<int>              ());
  std::unique_ptr<std::vector<int   > >            iphi              ( new std::vector<int>              ());
  std::unique_ptr<std::vector<int   > >            SOI_compressedEt  ( new std::vector<int>              ());
  std::unique_ptr<std::vector<int   > >            SOI_fineGrain     ( new std::vector<int>              ());
  std::unique_ptr<std::vector<int   > >            size              ( new std::vector<int>              ());
  std::unique_ptr<std::vector<int   > >            presamples        ( new std::vector<int>              ());
  std::unique_ptr<std::vector<std::vector<int> > > compressedEt      ( new std::vector<std::vector<int> >());
  std::unique_ptr<std::vector<std::vector<int> > > fineGrain         ( new std::vector<std::vector<int> >());
  std::unique_ptr<std::vector<std::vector<int> > > hbheDigiIndex     ( new std::vector<std::vector<int> >());
  std::unique_ptr<std::vector<std::vector<int> > > hfDigiIndex       ( new std::vector<std::vector<int> >());

  edm::ESHandle<HcalTrigTowerGeometry> geometry;
  iSetup.get<CaloGeometryRecord>().get(geometry);

  edm::Handle<HBHEDigiCollection> hbheDigis;
  iEvent.getByToken(hbheToken_, hbheDigis);

  HBHEDigiCollection::const_iterator iHBHE;
  HBHEDigiCollection::const_iterator first_HBHE = hbheDigis -> begin();
  
  edm::Handle<HFDigiCollection> hfDigis;
  iEvent.getByToken(hfToken_, hfDigis);
  
  HFDigiCollection::const_iterator iHF;
  HFDigiCollection::const_iterator first_HF = hfDigis -> begin();

  edm::Handle<HcalTrigPrimDigiCollection> tps;
  iEvent.getByToken(tpToken_, tps);
  
  HcalTrigPrimDigiCollection::const_iterator itp     = tps -> begin();
  HcalTrigPrimDigiCollection::const_iterator itp_end = tps -> end();

  std::vector<HcalDetId> detids;
  
  for(; itp != itp_end; ++itp){

    int nsamples = itp -> size ();
    HcalTrigTowerDetId id = itp -> id();
    
    ieta              -> push_back ( id.ieta() );
    iphi              -> push_back ( id.iphi() );
    SOI_compressedEt  -> push_back ( itp -> SOI_compressedEt() );
    SOI_fineGrain     -> push_back ( itp -> SOI_fineGrain   () );
    presamples        -> push_back ( itp -> presamples      () );
    size              -> push_back ( nsamples                  );
                     
    compressedEt      -> push_back ( std::vector<int> () );
    fineGrain         -> push_back ( std::vector<int> () );
    hbheDigiIndex     -> push_back ( std::vector<int> () );
    hfDigiIndex       -> push_back ( std::vector<int> () );

    size_t last_entry = compressedEt -> size() - 1;
    
    for (int i = 0; i < nsamples; ++i){
      HcalTriggerPrimitiveSample sample = itp -> sample (i);
      (*fineGrain   )[last_entry].push_back ( sample.fineGrain   () );
      (*compressedEt)[last_entry].push_back ( sample.compressedEt() );
    }

    detids.clear();
    detids = geometry -> detIds( id );
    int ndetids = detids.size();
    
    for (int i = 0; i < ndetids; ++i){
      int index; 
      HcalDetId       hcalId = detids[i];
      HcalSubdetector subdet = hcalId.subdet();
      
      if ( subdet == HcalBarrel || subdet == HcalEndcap ){
    	iHBHE = hbheDigis -> find ( hcalId );
    	if ( iHBHE != hbheDigis -> end() ){
    	  index = iHBHE - first_HBHE;
    	  (*hbheDigiIndex)[last_entry].push_back(index);
    	}
      }
      
      else if ( subdet == HcalForward ) {
    	iHF = hfDigis -> find ( hcalId );
    	if ( iHF != hfDigis -> end() ){
    	  index = iHF - first_HF;
    	  (*hfDigiIndex)[last_entry].push_back(index);
    	}
      }
    }
  }
  
  iEvent.put(move( ieta            )  , prefix + "IEta"            + suffix );
  iEvent.put(move( iphi            ) , prefix + "IPhi"            + suffix );
  iEvent.put(move( SOI_compressedEt) , prefix + "CompressedEtSOI" + suffix );
  iEvent.put(move( SOI_fineGrain   ) , prefix + "FineGrainSOI"    + suffix );
  iEvent.put(move( presamples      ) , prefix + "Size"            + suffix );
  iEvent.put(move( size            ) , prefix + "Presamples"      + suffix );
  
  iEvent.put(move( compressedEt    ) , prefix + "CompressedEt"    + suffix );
  iEvent.put(move( fineGrain       ) , prefix + "FineGrain"       + suffix );
  iEvent.put(move( hbheDigiIndex   ) , prefix + "HBHEDigiIndex"   + suffix );
  iEvent.put(move( hfDigiIndex     ) , prefix + "HFDigiIndex"     + suffix );
  
}
