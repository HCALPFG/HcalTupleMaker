#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_CaloJetMet.h"

HcalTupleMaker_CaloJetMet::HcalTupleMaker_CaloJetMet(const edm::ParameterSet& iConfig):
  //recoInputTag     (iConfig.getUntrackedParameter<std::string>("recoInputTag")),
  recoInputToken     (consumes<HBHERecHitCollection>((iConfig.getUntrackedParameter<std::string>("recoInputTag")))),
  prefix             (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix             (iConfig.getUntrackedParameter<std::string>("Suffix")),
  EcalRecHitsEBToken (consumes<EcalRecHitCollection>(edm::InputTag("ecalRecHit","EcalRecHitsEB"))),
  EcalRecHitsEEToken (consumes<EcalRecHitCollection>(edm::InputTag("ecalRecHit","EcalRecHitsEE"))),
  ak4CaloJetsToken   (consumes<reco::CaloJetCollection>(edm::InputTag("ak4CaloJets"))),
  caloMetToken       (consumes<reco::CaloMETCollection>(edm::InputTag("caloMet")))
{
  produces <std::vector<double> >            (prefix + "EBET"           + suffix );
  produces <std::vector<double> >            (prefix + "EEET"           + suffix );
  produces <std::vector<double> >            (prefix + "HBET"           + suffix );
  produces <std::vector<double> >            (prefix + "HEET"           + suffix );
  produces <std::vector<double> >            (prefix + "HFET"           + suffix );
  produces <std::vector<double> >            (prefix + "EBSumE"         + suffix );
  produces <std::vector<double> >            (prefix + "EESumE"         + suffix );
  produces <std::vector<double> >            (prefix + "HBSumE"         + suffix );
  produces <std::vector<double> >            (prefix + "HESumE"         + suffix );
  produces <std::vector<double> >            (prefix + "EBSumET"        + suffix );
  produces <std::vector<double> >            (prefix + "EESumET"        + suffix );
  produces <std::vector<double> >            (prefix + "HBSumET"        + suffix );
  produces <std::vector<double> >            (prefix + "HESumET"        + suffix );
  //
  //Beware, the quantites below are not computed from "hRecHits", so they will not follow changes in reco-method in dual-reco python cfg files.
  produces <std::vector<double> >            (prefix + "NominalMET"     + suffix );
  //
  produces <std::vector<double> >            (prefix + "JetEta"         + suffix );
  produces <std::vector<double> >            (prefix + "JetPt"          + suffix );
  produces <std::vector<double> >            (prefix + "JetPhi"         + suffix );
  produces <std::vector<double> >            (prefix + "JetHadHB"       + suffix );
  produces <std::vector<double> >            (prefix + "JetHadHE"       + suffix );
  produces <std::vector<double> >            (prefix + "JetHadHF"       + suffix );
  produces <std::vector<double> >            (prefix + "JetEMEB"        + suffix );
  produces <std::vector<double> >            (prefix + "JetEMEE"        + suffix );
  produces <std::vector<double> >            (prefix + "JetEMHF"        + suffix );
  produces <std::vector<double> >            (prefix + "JetEMFrac"      + suffix );
  produces <std::vector<double> >            (prefix + "JetHadFrac"     + suffix );
  produces <std::vector<int> >               (prefix + "JetN90"         + suffix );
  produces <std::vector<int> >               (prefix + "JetN60"         + suffix );
}

void HcalTupleMaker_CaloJetMet::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  std::auto_ptr<std::vector<double> >            ebet         ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            eeet         ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hbet         ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            heet         ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hfet         ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            ebsume       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            eesume       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hbsume       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hesume       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            ebsumet      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            eesumet      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hbsumet      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            hesumet      ( new std::vector<double>           ());
  //
  std::auto_ptr<std::vector<double> >            nominalmet   ( new std::vector<double>           ());
  //
  std::auto_ptr<std::vector<double> >            jeteta       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jetpt        ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jetphi       ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jethadhb     ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jethadhe     ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jethadhf     ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jetemeb      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jetemee      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jetemhf      ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jetemfrac    ( new std::vector<double>           ());
  std::auto_ptr<std::vector<double> >            jethadfrac   ( new std::vector<double>           ());
  std::auto_ptr<std::vector<int> >               jetn90       ( new std::vector<int>              ());
  std::auto_ptr<std::vector<int> >               jetn60       ( new std::vector<int>              ());
 
  edm::Handle<HBHERecHitCollection> hRecHits;
  //iEvent.getByLabel(recoInputTag, hRecHits);
  iEvent.getByToken(recoInputToken, hRecHits);

  edm::Handle<EcalRecHitCollection> hEBRecHits;
  //iEvent.getByLabel("ecalRecHit","EcalRecHitsEB", hEBRecHits);
  iEvent.getByToken(EcalRecHitsEBToken, hEBRecHits);

  edm::Handle<EcalRecHitCollection> hEERecHits;
  //iEvent.getByLabel("ecalRecHit","EcalRecHitsEE", hEERecHits);
  iEvent.getByToken(EcalRecHitsEEToken, hEERecHits);

  edm::ESHandle<CaloGeometry> hGeometry;
  iSetup.get<CaloGeometryRecord>().get(hGeometry);
  Geometry = hGeometry.product();

  edm::Handle<reco::CaloJetCollection> hCaloJets;
  //iEvent.getByLabel("ak4CaloJets", hCaloJets);
  iEvent.getByToken(ak4CaloJetsToken, hCaloJets);

  edm::Handle<reco::CaloMETCollection> hCaloMET;
  //iEvent.getByLabel("caloMet", hCaloMET);
  iEvent.getByToken(caloMetToken, hCaloMET);
  
  //std::cout<<" Jet size "<<(*hCaloJets).size()<<std::endl; 
  reco::CaloJetCollection::const_iterator jet = hCaloJets->begin ();
  for (; jet != hCaloJets->end (); jet++)
    {
      jeteta  -> push_back( (*jet).eta() );
      jetphi  -> push_back( (*jet).phi() );
      jetpt   -> push_back( (*jet).et() );
      jethadhb-> push_back( (*jet).hadEnergyInHB() );
      jethadhe-> push_back( (*jet).hadEnergyInHE() );
      jethadhf-> push_back( (*jet).hadEnergyInHF() );
      jetemeb -> push_back( (*jet).emEnergyInEB() );
      jetemee -> push_back( (*jet).emEnergyInEE() );
      jetemhf -> push_back( (*jet).emEnergyInHF() );
      jetemfrac  -> push_back( (*jet).emEnergyFraction() );
      jethadfrac -> push_back( (*jet).energyFractionHadronic() );
      jetn90 -> push_back( (*jet).n90() );//the number of constituents carrying a 90% of the total Jet energy
      jetn60 -> push_back( (*jet).n60() );//the number of constituents carrying a 90% of the total Jet energy
      //
      //std::cout<<"Et/Eta/Phi jet: "<<(*jet).et()<<" / "<<(*jet).eta()<<" / "<<(*jet).phi()<<std::endl;
      //std::cout<<"    EMfrac jet: "<<(*jet).emEnergyFraction()<<std::endl; 
      //std::cout<<"   HADfrac jet: "<<(*jet).energyFractionHadronic() <<std::endl;
      //std::cout<<"       n90 jet: "<<(*jet).n90()<<std::endl;
      //std::cout<<"       n60 jet: "<<(*jet).n60()<<std::endl;
    }  
  
  
  ClearVariables();
  CalculateTotalEnergiesHBHE( *hRecHits );
  CalculateTotalEnergiesEB( *hEBRecHits );
  CalculateTotalEnergiesEE( *hEERecHits );

  if(hCaloMET->size() > 0)
    {
      NominalMET[0] = (*hCaloMET)[0].px();
      NominalMET[1] = (*hCaloMET)[0].py();
    }


  //Vector x and y components for these variables a la HcalNoiseAnalyzer
  ebet -> push_back( EBET[0] );              ebet -> push_back( EBET[1] );
  eeet -> push_back( EEET[0] );              eeet -> push_back( EEET[1] );
  hbet -> push_back( HBET[0] );              hbet -> push_back( HBET[1] );
  heet -> push_back( HEET[0] );              heet -> push_back( HEET[1] );
  //
  nominalmet -> push_back( NominalMET[0] );  nominalmet -> push_back( NominalMET[1] );
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
  iEvent.put(ebsume     , prefix + "EBSumE"          + suffix );  
  iEvent.put(eesume     , prefix + "EESumE"          + suffix );  
  iEvent.put(hbsume     , prefix + "HBSumE"          + suffix );  
  iEvent.put(hesume     , prefix + "HESumE"          + suffix );  
  iEvent.put(ebsumet    , prefix + "EBSumET"         + suffix );  
  iEvent.put(eesumet    , prefix + "EESumET"         + suffix );  
  iEvent.put(hbsumet    , prefix + "HBSumET"         + suffix );  
  iEvent.put(hesumet    , prefix + "HESumET"         + suffix );  
  //
  iEvent.put(nominalmet , prefix + "NominalMET"      + suffix );  
  //
  iEvent.put(jeteta     , prefix + "JetEta"         + suffix );  
  iEvent.put(jetpt      , prefix + "JetPt"          + suffix );  
  iEvent.put(jetphi     , prefix + "JetPhi"         + suffix );  
  iEvent.put(jethadhb   , prefix + "JetHadHB"       + suffix );  
  iEvent.put(jethadhe   , prefix + "JetHadHE"       + suffix );  
  iEvent.put(jethadhf   , prefix + "JetHadHF"       + suffix );  
  iEvent.put(jetemeb    , prefix + "JetEMEB"        + suffix );  
  iEvent.put(jetemee    , prefix + "JetEMEE"        + suffix );  
  iEvent.put(jetemhf    , prefix + "JetEMHF"        + suffix );  
  iEvent.put(jetemfrac  , prefix + "JetEMFrac"      + suffix );
  iEvent.put(jethadfrac , prefix + "JetHadFrac"     + suffix );
  iEvent.put(jetn90     , prefix + "JetN90"         + suffix );
  iEvent.put(jetn60     , prefix + "JetN60"         + suffix );
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
