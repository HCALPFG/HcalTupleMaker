#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_CaloJetMet.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HFRecHit.h"
#include "DataFormats/HcalRecHit/interface/HORecHit.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"

HcalTupleMaker_CaloJetMet::HcalTupleMaker_CaloJetMet(const edm::ParameterSet& iConfig):
  recoInputTag   (iConfig.getUntrackedParameter<std::string>("recoInputTag")),
  prefix         (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix         (iConfig.getUntrackedParameter<std::string>("Suffix"))
{
  produces <std::vector<double> >            (prefix + "EBET"           + suffix );
  produces <std::vector<double> >            (prefix + "EEET"           + suffix );
  produces <std::vector<double> >            (prefix + "HBET"           + suffix );
  produces <std::vector<double> >            (prefix + "HEET"           + suffix );
  produces <std::vector<double> >            (prefix + "HFET"           + suffix );
  //produces <std::vector<double> >            (prefix + "NominalMET"     + suffix );
  produces <std::vector<double> >            (prefix + "EBSumE"         + suffix );
  produces <std::vector<double> >            (prefix + "EESumE"         + suffix );
  produces <std::vector<double> >            (prefix + "HBSumE"         + suffix );
  produces <std::vector<double> >            (prefix + "HESumE"         + suffix );
  produces <std::vector<double> >            (prefix + "EBSumET"        + suffix );
  produces <std::vector<double> >            (prefix + "EESumET"        + suffix );
  produces <std::vector<double> >            (prefix + "HBSumET"        + suffix );
  produces <std::vector<double> >            (prefix + "HESumET"        + suffix );
  //produces <std::vector<double> >            (prefix + "JetEta"         + suffix );
  //produces <std::vector<double> >            (prefix + "JetPt"          + suffix );
  //produces <std::vector<double> >            (prefix + "JetPhi"         + suffix );
  //produces <std::vector<double> >            (prefix + "JetHad"         + suffix );
  //produces <std::vector<double> >            (prefix + "JetEM"          + suffix );
}

void HcalTupleMaker_CaloJetMet::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  std::auto_ptr<std::vector<double> >            ebet         ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            eeet         ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hbet         ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            heet         ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hfet         ( new std::vector<double>           ());
  //std::auto_ptr<std::vector<double> >            nominalmet   ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            ebsume       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            eesume       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hbsume       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hesume       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            ebsumet      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            eesumet      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hbsumet      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hesumet      ( new std::vector<double>           ());
  //std::auto_ptr<std::vector<double> >            jeteta       ( new std::vector<double>           ());
  //std::auto_ptr<std::vector<double> >            jetpt        ( new std::vector<double>           ());
  //std::auto_ptr<std::vector<double> >            jetphi       ( new std::vector<double>           ());
  //std::auto_ptr<std::vector<double> >            jethad       ( new std::vector<double>           ());
  //std::auto_ptr<std::vector<double> >            jetem        ( new std::vector<double>           ());
 
  edm::Handle<HBHERecHitCollection> hRecHits;
  iEvent.getByLabel(recoInputTag, hRecHits);

  edm::Handle<EcalRecHitCollection> hEBRecHits;
  iEvent.getByLabel("ecalRecHit","EcalRecHitsEB", hEBRecHits);

  edm::Handle<EcalRecHitCollection> hEERecHits;
  iEvent.getByLabel("ecalRecHit","EcalRecHitsEE", hEERecHits);

  edm::ESHandle<CaloGeometry> hGeometry;
  iSetup.get<CaloGeometryRecord>().get(hGeometry);
  Geometry = hGeometry.product();

  //edm::Handle<reco::CaloJetCollection> hCaloJets;
  //iEvent.getByLabel("ak4CaloJets", hCaloJets);

  //edm::Handle<reco::CaloMETCollection> hCaloMET;
  //iEvent.getByLabel("caloMet", hCaloMET);

  /*
  std::map<double, int, std::greater<double> > JetPTMap;
  std::map<double, int, std::greater<double> >::iterator iter = JetPTMap.begin();
  if(JetPTMap.size() > 0)
    {
      if(iter->second < (int)hCaloJets->size())
	{
	  jeteta -> push_back( (*hCaloJets)[iter->second].eta() );
	  jetphi -> push_back( (*hCaloJets)[iter->second].phi() );
	  jetpt  -> push_back( (*hCaloJets)[iter->second].pt()  );
	  jethad -> push_back( (*hCaloJets)[iter->second].hadEnergyInHB()
			       + (*hCaloJets)[iter->second].hadEnergyInHE() + (*hCaloJets)[iter->second].hadEnergyInHF() );
	  jetem  -> push_back( (*hCaloJets)[iter->second].emEnergyInEB()
			       + (*hCaloJets)[iter->second].emEnergyInEE() + (*hCaloJets)[iter->second].emEnergyInHF()   );
	}
    }
  if(JetPTMap.size() > 1)
    {
      iter++;
      if(iter->second < (int)hCaloJets->size())
	{
	  jeteta -> push_back( (*hCaloJets)[iter->second].eta() );
	  jetphi -> push_back( (*hCaloJets)[iter->second].phi() );
	  jetpt  -> push_back( (*hCaloJets)[iter->second].pt()  );
	  jethad -> push_back( (*hCaloJets)[iter->second].hadEnergyInHB()
			       + (*hCaloJets)[iter->second].hadEnergyInHE() + (*hCaloJets)[iter->second].hadEnergyInHF() );
	  jetem  -> push_back( (*hCaloJets)[iter->second].emEnergyInEB()
			       + (*hCaloJets)[iter->second].emEnergyInEE() + (*hCaloJets)[iter->second].emEnergyInHF()   );
	}
    }
  */

  ClearVariables();
  CalculateTotalEnergiesHBHE( *hRecHits );
  CalculateTotalEnergiesEB( *hEBRecHits );
  CalculateTotalEnergiesEE( *hEERecHits );

  //if(hCaloMET->size() > 0)
  //  {
  //    NominalMET[0] = (*hCaloMET)[0].px();
  //    NominalMET[1] = (*hCaloMET)[0].py();
  //  }


  //Vector x and y components for these variables a la HcalNoiseAnalyzer
  ebet -> push_back( EBET[0] );              ebet -> push_back( EBET[1] );
  eeet -> push_back( EEET[0] );              eeet -> push_back( EEET[1] );
  hbet -> push_back( HBET[0] );              hbet -> push_back( HBET[1] );
  heet -> push_back( HEET[0] );              heet -> push_back( HEET[1] );
  //nominalmet -> push_back( NominalMET[0] );  nominalmet -> push_back( NominalMET[1] );
  //
  ebsume  -> push_back( EBSumE  );
  eesume  -> push_back( EESumE  );
  hbsume  -> push_back( HBSumE  );
  hesume  -> push_back( HESumE  );
  ebsumet -> push_back( EBSumET );
  eesumet -> push_back( EESumET );
  hbsumet -> push_back( HBSumET );
  hesumet -> push_back( HESumET );

  iEvent.put(ebet       , prefix + "EBET"            + suffix );
  iEvent.put(eeet       , prefix + "EEET"            + suffix );
  iEvent.put(hbet       , prefix + "HBET"            + suffix );
  iEvent.put(heet       , prefix + "HEET"            + suffix );
  iEvent.put(hfet       , prefix + "HFET"            + suffix );
  //iEvent.put(nominalmet , prefix + "NominalMET"      + suffix );  
  iEvent.put(ebsume     , prefix + "EBSumE"          + suffix );  
  iEvent.put(eesume     , prefix + "EESumE"          + suffix );  
  iEvent.put(hbsume     , prefix + "HBSumE"          + suffix );  
  iEvent.put(hesume     , prefix + "HESumE"          + suffix );  
  iEvent.put(ebsumet    , prefix + "EBSumET"         + suffix );  
  iEvent.put(eesumet    , prefix + "EESumET"         + suffix );  
  iEvent.put(hbsumet    , prefix + "HBSumET"         + suffix );  
  iEvent.put(hesumet    , prefix + "HESumET"         + suffix );  
  //iEvent.put(jeteta     , prefix + "JetEta"         + suffix );  
  //iEvent.put(jetpt      , prefix + "JetPt"          + suffix );  
  //iEvent.put(jetphi     , prefix + "JetPhi"         + suffix );  
  //iEvent.put(jethad     , prefix + "JetHad"         + suffix );  
  //iEvent.put(jetem      , prefix + "JetEM"          + suffix );  
}

void HcalTupleMaker_CaloJetMet::CalculateTotalEnergiesHBHE(const HBHERecHitCollection &RecHits){
  for(int i = 0; i < (int)RecHits.size(); i++)
    {
      bool IsHB = true;
      if(RecHits[i].id().subdet() == HcalEndcap)
	IsHB = false;
      double eta = Geometry->getPosition(RecHits[i].id()).eta();
      double phi = Geometry->getPosition(RecHits[i].id()).phi();
      double energy = RecHits[i].energy();
      double et = energy / cosh(eta);
      if(IsHB == true)
	{
	  HBET[0] = HBET[0] + et * cos(phi);
	  HBET[1] = HBET[1] + et * sin(phi);
	  HBSumE = HBSumE + energy;
	  HBSumET = HBSumET + et;
	}
      else // is HE
	{
	  HEET[0] = HEET[0] + et * cos(phi);
	  HEET[1] = HEET[1] + et * sin(phi);
	  HESumE = HESumE + energy;
	  HESumET = HESumET + et;
	}
    }
}


void HcalTupleMaker_CaloJetMet::CalculateTotalEnergiesEB(const EcalRecHitCollection &RecHits)
{
  for(int i = 0; i < (int)RecHits.size(); i++)
    {
      double eta = Geometry->getPosition(RecHits[i].id()).eta();
      double phi = Geometry->getPosition(RecHits[i].id()).phi();
      double energy = RecHits[i].energy();
      double et = energy / cosh(eta);
      EBET[0] = EBET[0] + et * cos(phi);
      EBET[1] = EBET[1] + et * sin(phi);
      EBSumE = EBSumE + energy;
      EBSumET = EBSumET + et;
    }
}


void HcalTupleMaker_CaloJetMet::CalculateTotalEnergiesEE(const EcalRecHitCollection &RecHits)
{
  for(int i = 0; i < (int)RecHits.size(); i++)
    {
      double eta = Geometry->getPosition(RecHits[i].id()).eta();
      double phi = Geometry->getPosition(RecHits[i].id()).phi();
      double energy = RecHits[i].energy();
      double et = energy / cosh(eta);
      EEET[0] = EEET[0] + et * cos(phi);
      EEET[1] = EEET[1] + et * sin(phi);
      EESumE = EESumE + energy;
      EESumET = EESumET + et;
    }
}


void HcalTupleMaker_CaloJetMet::ClearVariables(){
  EBET[0] = 0; EBET[1] = 0;
  EEET[0] = 0; EEET[1] = 0;
  HBET[0] = 0; HBET[1] = 0;
  HEET[0] = 0; HEET[1] = 0;
  HFET[0] = 0; HFET[1] = 0;
  NominalMET[0] = 0; NominalMET[1] = 0;
  EBSumE = 0;
  EESumE = 0;
  HBSumE = 0;
  HESumE = 0;
  HFSumE = 0;
  EBSumET = 0;
  EESumET = 0;
  HBSumET = 0;
  HESumET = 0;
  HFSumET = 0;
}
