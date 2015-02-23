#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalNoiseFilters.h"
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
#include "FWCore/Framework/interface/Event.h"
#include <iterator>
#include <vector>

HcalTupleMaker_HcalNoiseFilters::HcalTupleMaker_HcalNoiseFilters(const edm::ParameterSet& iConfig):
  noiseSummaryInputTag (iConfig.getUntrackedParameter<edm::InputTag>("noiseSummaryInputTag")),
  noiseResultInputTag  (iConfig.getUntrackedParameter<std::string>("noiseResultInputTag")), 
  recoInputTag         (iConfig.getUntrackedParameter<std::string>("recoInputTag")),
  prefix               (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix               (iConfig.getUntrackedParameter<std::string>("Suffix"))
{
  produces <std::vector<int> >                  (prefix + "OfficialDecision"         + suffix );
  produces <std::vector<int> >                  (prefix + "HPDHits"                  + suffix );
  produces <std::vector<int> >                  (prefix + "HPDNoOtherHits"           + suffix );
  produces <std::vector<int> >                  (prefix + "MaxZeros"                 + suffix );
  produces <std::vector<double> >               (prefix + "MinE2E10"                 + suffix );
  produces <std::vector<double> >               (prefix + "MaxE2E10"                 + suffix );
  produces <std::vector<int> >                  (prefix + "HasBadRBXR45"             + suffix );
  produces <std::vector<int> >                  (prefix + "HasBadRBXRechitR45Loose"  + suffix );
  produces <std::vector<int> >                  (prefix + "HasBadRBXRechitR45Tight"  + suffix );
  produces <std::vector<unsigned int> >         (prefix + "FlagWord"                 + suffix );
  produces <std::vector<unsigned int> >         (prefix + "AuxWord"                  + suffix );
  //
  // Perhaps these should be migrated to HcalTupleMaker_HcalRecHits and HcalTupleMaker_HcalDigis
  produces <std::vector<std::vector<double> > > (prefix + "RBXCharge"                + suffix );
  produces <std::vector<std::vector<double> > > (prefix + "RBXCharge15"              + suffix );
  produces <std::vector<double> >               (prefix + "RBXEnergy"                + suffix );
  produces <std::vector<double> >               (prefix + "RBXEnergy15"              + suffix );
}

void HcalTupleMaker_HcalNoiseFilters::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  std::auto_ptr<std::vector<int> >                  officialdecision           ( new std::vector<int>                  ());
  std::auto_ptr<std::vector<int> >                  hpdhits                    ( new std::vector<int>                  ());
  std::auto_ptr<std::vector<int> >                  hpdnootherhits             ( new std::vector<int>                  ());
  std::auto_ptr<std::vector<int> >                  maxzeros                   ( new std::vector<int>                  ());
  std::auto_ptr<std::vector<double> >               mine2e10                   ( new std::vector<double>               ());
  std::auto_ptr<std::vector<double> >               maxe2e10                   ( new std::vector<double>               ());
  std::auto_ptr<std::vector<int> >                  hasbadrbxr45               ( new std::vector<int>                  ());
  std::auto_ptr<std::vector<int> >                  hasbadrbxrechitr45loose    ( new std::vector<int>                  ());
  std::auto_ptr<std::vector<int> >                  hasbadrbxrechitr45tight    ( new std::vector<int>                  ());
  std::auto_ptr<std::vector<unsigned int> >         flagword                   ( new std::vector<unsigned int>         ());
  std::auto_ptr<std::vector<unsigned int> >         auxword                    ( new std::vector<unsigned int>         ());
  std::auto_ptr<std::vector<std::vector<double> > > rbxcharge                  ( new std::vector<std::vector<double> > ());
  std::auto_ptr<std::vector<std::vector<double> > > rbxcharge15                ( new std::vector<std::vector<double> > ());
  std::auto_ptr<std::vector<double> >               rbxenergy                  ( new std::vector<double>               ());
  std::auto_ptr<std::vector<double> >               rbxenergy15                ( new std::vector<double>               ());

  edm::Handle<bool> hNoiseResult;
  iEvent.getByLabel(noiseResultInputTag, "HBHENoiseFilterResult", hNoiseResult);

  // HCAL Filter Decision
  officialdecision        -> push_back( *hNoiseResult );


  edm::Handle<HcalNoiseSummary> hSummary;
  iEvent.getByLabel(noiseSummaryInputTag, hSummary);

  // HCAL summary objects
  std::cout<<"hSummary->maxHPDHits(): "<<hSummary->maxHPDHits()<<std::endl;
  hpdhits                 -> push_back ( hSummary->maxHPDHits() );
  hpdnootherhits          -> push_back ( hSummary->maxHPDNoOtherHits() );
  maxzeros                -> push_back ( hSummary->maxZeros() );
  mine2e10                -> push_back ( hSummary->minE2Over10TS() );
  maxe2e10                -> push_back ( hSummary->maxE2Over10TS() );
  hasbadrbxr45            -> push_back ( hSummary->HasBadRBXTS4TS5() );
  hasbadrbxrechitr45loose -> push_back ( hSummary->HasBadRBXRechitR45Loose() );
  hasbadrbxrechitr45tight -> push_back ( hSummary->HasBadRBXRechitR45Tight() );


  edm::Handle<HBHERecHitCollection> hRecHits;
  iEvent.getByLabel(recoInputTag, hRecHits);
  std::cout<<"(int)hRecHits->size(): "<<(int)hRecHits->size()<<std::endl;

  edm::Handle<HBHEDigiCollection> hHBHEDigis;
  iEvent.getByLabel("hcalDigis", hHBHEDigis);
  std::cout<<"(int)hHBHEDigis->size(): "<<(int)hHBHEDigis->size()<<std::endl;

  edm::ESHandle<HcalDbService> hConditions;
  iSetup.get<HcalDbRecord>().get(hConditions);

  std::map<HcalDetId, int> RecHitIndex;
  for(int i = 0; i < (int)hRecHits->size(); i++){
    HcalDetId id = (*hRecHits)[i].id();
    RecHitIndex.insert(std::pair<HcalDetId, int>(id, i));
  }
  
  // HBHE RBX energy and mega-pulse shape
  double RBXCharge[72][10];
  double RBXEnergy[72];
  double RBXCharge15[72][10];
  double RBXEnergy15[72];
  // Reset values to 0
  for(int i = 0; i < 72; i++){
    for(int j = 0; j < 10; j++){
      RBXCharge[i][j] = 0;
      RBXCharge15[i][j] = 0;
    }
    RBXEnergy[i] = 0;
    RBXEnergy15[i] = 0;
  }

  // loop over digis
  for(HBHEDigiCollection::const_iterator iter = hHBHEDigis->begin(); iter != hHBHEDigis->end(); iter++){
    //
    HcalDetId id = iter->id();
    int RBXIndex = HcalHPDRBXMap::indexRBX(id);
    //
    // HCAL rechit flagword, auxword
    flagword -> push_back ( (*hRecHits)[RecHitIndex[id]].flags() );
    auxword  -> push_back ( (*hRecHits)[RecHitIndex[id]].aux()   );
    //

    // First convert ADC to deposited charge
    const HcalCalibrations &Calibrations = hConditions->getHcalCalibrations(id);
    const HcalQIECoder *ChannelCoder = hConditions->getHcalCoder(id);
    const HcalQIEShape *Shape = hConditions->getHcalShape(ChannelCoder);
    HcalCoderDb Coder(*ChannelCoder, *Shape);
    CaloSamples Tool;
    Coder.adc2fC(*iter, Tool);

    // Calculate RBX total charge, total energy
    for(int i = 0; i < (int)iter->size(); i++){// loop over TS's
      const HcalQIESample &QIE = iter->sample(i);
      RBXCharge[RBXIndex][i] = RBXCharge[RBXIndex][i] + Tool[i] - Calibrations.pedestal(QIE.capid());
      if((*hRecHits)[RecHitIndex[id]].energy() > 1.5)
	RBXCharge15[RBXIndex][i] = RBXCharge15[RBXIndex][i] + Tool[i] - Calibrations.pedestal(QIE.capid());
    }
    RBXEnergy[RBXIndex] = RBXEnergy[RBXIndex] + (*hRecHits)[RecHitIndex[id]].energy();
    if((*hRecHits)[RecHitIndex[id]].energy() > 1.5)
      RBXEnergy15[RBXIndex] = RBXEnergy15[RBXIndex] + (*hRecHits)[RecHitIndex[id]].energy();
  }

  // RBX charge and energy vectors are filled in
  for(int irbx = 0; irbx < 72; irbx++){
    std::vector<double> RBXChargevector(   std::begin(RBXCharge[irbx]), std::end(RBXCharge[irbx]) );
    std::vector<double> RBXCharge15vector( std::begin(RBXCharge[irbx]), std::end(RBXCharge[irbx]) );
    //rbxcharge   -> push_back( RBXCharge[irbx]   );
    //rbxcharge15 -> push_back( RBXCharge15[irbx] );
    rbxcharge   -> push_back( RBXChargevector   );
    rbxcharge15 -> push_back( RBXCharge15vector );
    rbxenergy   -> push_back( RBXEnergy[irbx]   );
    rbxenergy15 -> push_back( RBXEnergy15[irbx] );
  }  

  iEvent.put( officialdecision         , prefix + "OfficialDecision"         + suffix );
  iEvent.put( hpdhits                  , prefix + "HPDHits"                  + suffix );
  iEvent.put( hpdnootherhits           , prefix + "HPDNoOtherHits"           + suffix );
  iEvent.put( maxzeros                 , prefix + "MaxZeros"                 + suffix );
  iEvent.put( mine2e10                 , prefix + "MinE2E10"                 + suffix );
  iEvent.put( maxe2e10                 , prefix + "MaxE2E10"                 + suffix );
  iEvent.put( hasbadrbxr45             , prefix + "HasBadRBXR45"             + suffix );
  iEvent.put( hasbadrbxrechitr45loose  , prefix + "HasBadRBXRechitR45Loose"  + suffix );
  iEvent.put( hasbadrbxrechitr45tight  , prefix + "HasBadRBXRechitR45Tight"  + suffix );
  iEvent.put( flagword                 , prefix + "FlagWord"                 + suffix );
  iEvent.put( auxword                  , prefix + "AuxWord"                  + suffix );
  iEvent.put( rbxcharge                , prefix + "RBXCharge"                + suffix );
  iEvent.put( rbxcharge15              , prefix + "RBXCharge15"              + suffix );
  iEvent.put( rbxenergy                , prefix + "RBXEnergy"                + suffix );
  iEvent.put( rbxenergy15              , prefix + "RBXEnergy15"              + suffix );
}
