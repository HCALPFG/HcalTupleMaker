#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalIsoNoiseFilterParameters.h"
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
#include "RecoLocalCalo/HcalRecProducers/src/HBHEIsolatedNoiseReflagger.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HBHEIsolatedNoiseAlgos.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"
#include "DataFormats/METReco/interface/HcalCaloFlagLabels.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
#include "FWCore/Framework/interface/Event.h"
#include <iterator>
#include <vector>

HcalTupleMaker_HcalIsoNoiseFilterParameters::HcalTupleMaker_HcalIsoNoiseFilterParameters(const edm::ParameterSet& iConfig):
  noiseSummaryInputTag (iConfig.getUntrackedParameter<edm::InputTag>("noiseSummaryInputTag")),
  //noiseResultInputTag  (iConfig.getUntrackedParameter<std::string>("noiseResultInputTag")), 
  //recoInputTag         (iConfig.getUntrackedParameter<std::string>("recoInputTag")),
  //isRAW                (iConfig.getUntrackedParameter<bool>("isRAW")),
  prefix               (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix               (iConfig.getUntrackedParameter<std::string>("Suffix"))
{
  // Iso noise filter params - these are already stored via the HcalNoiseFilters module.
  //produces <std::vector<int> >    (prefix + "NumIsolatedNoiseChannels" + suffix );
  //produces <std::vector<double> > (prefix + "IsolatedNoiseSumE"        + suffix );
  //produces <std::vector<double> > (prefix + "IsolatedNoiseSumEt"       + suffix );
  // 
  // RBX clusters
  produces <std::vector<int> >    (prefix + "CRBXId"              + suffix );//Id of the corresponding RBX
  produces <std::vector<int> >    (prefix + "CRBXIsTagged"        + suffix );//1=Loose Cluster,     2=Tight Cluster,     0=None
  produces <std::vector<int> >    (prefix + "CRBXIsIso"           + suffix );//1=Loose Cluster Iso, 2=Tight Cluster Iso, 0=None
  produces <std::vector<int> >    (prefix + "CRBXNHits"           + suffix );//No of rechits in the cluster (HCAL)
  produces <std::vector<double> > (prefix + "CRBXIsolHcalE"       + suffix );//Hcal Isolation Energy
  produces <std::vector<double> > (prefix + "CRBXIsolEcalE"       + suffix );//Ecal Isolation Energy
  produces <std::vector<double> > (prefix + "CRBXIsolTrkE"        + suffix );//Tracker Isolation Energy
  produces <std::vector<double> > (prefix + "CRBXTrkFidE"         + suffix );//Fiducial Cluster Energy (HCAL)
  produces <std::vector<double> > (prefix + "CRBXCluE"            + suffix );//Cluster Energy (HCAL)
  // HPD clusters
  produces <std::vector<int> >    (prefix + "CHPDId"              + suffix );//Id of the corresponding HPD
  produces <std::vector<int> >    (prefix + "CHPDRBXId"           + suffix );//Id of the corresponding RBX
  produces <std::vector<int> >    (prefix + "CHPDIsTagged"        + suffix );
  produces <std::vector<int> >    (prefix + "CHPDIsIso"           + suffix );
  produces <std::vector<int> >    (prefix + "CHPDNHits"           + suffix );
  produces <std::vector<double> > (prefix + "CHPDIsolHcalE"       + suffix );
  produces <std::vector<double> > (prefix + "CHPDIsolEcalE"       + suffix );
  produces <std::vector<double> > (prefix + "CHPDIsolTrkE"        + suffix );
  produces <std::vector<double> > (prefix + "CHPDTrkFidE"         + suffix );
  produces <std::vector<double> > (prefix + "CHPDCluE"            + suffix );
  // Dihit clusters
  produces <std::vector<int> >    (prefix + "CDihitIsTagged"      + suffix );
  produces <std::vector<int> >    (prefix + "CDihitIsIso"         + suffix );
  produces <std::vector<double> > (prefix + "CDihitIsolHcalE"     + suffix );
  produces <std::vector<double> > (prefix + "CDihitIsolEcalE"     + suffix );
  produces <std::vector<double> > (prefix + "CDihitIsolTrkE"      + suffix );
  produces <std::vector<double> > (prefix + "CDihitTrkFidE"       + suffix );
  produces <std::vector<double> > (prefix + "CDihitCluE"          + suffix );
  // Monohit clusters
  produces <std::vector<int> >    (prefix + "CMonohitIsTagged"    + suffix );
  produces <std::vector<int> >    (prefix + "CMonohitIsIso"       + suffix );
  produces <std::vector<double> > (prefix + "CMonohitIsolHcalE"   + suffix );
  produces <std::vector<double> > (prefix + "CMonohitIsolEcalE"   + suffix );
  produces <std::vector<double> > (prefix + "CMonohitIsolTrkE"    + suffix );
  produces <std::vector<double> > (prefix + "CMonohitTrkFidE"     + suffix );
  produces <std::vector<double> > (prefix + "CMonohitCluE"        + suffix );
}

void HcalTupleMaker_HcalIsoNoiseFilterParameters::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  // iso noise filter params
  //std::auto_ptr<std::vector<int> >     numisolatednoisechannels  ( new std::vector<int>    ());
  //std::auto_ptr<std::vector<double> >  isolatednoisesume         ( new std::vector<double> ());
  //std::auto_ptr<std::vector<double> >  isolatednoisesumet        ( new std::vector<double> ());
  // rbx clusters
  std::auto_ptr<std::vector<int> >     crbxid           ( new std::vector<int> ());
  std::auto_ptr<std::vector<int> >     crbxnhits        ( new std::vector<int> ());
  std::auto_ptr<std::vector<int> >     crbxistagged     ( new std::vector<int> ());
  std::auto_ptr<std::vector<int> >     crbxisiso        ( new std::vector<int> ());
  std::auto_ptr<std::vector<double> >  crbxisolhcale    ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  crbxisolecale    ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  crbxisoltrke     ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  crbxtrkfide      ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  crbxclue         ( new std::vector<double> ());
  // hpd clusters
  std::auto_ptr<std::vector<int> >     chpdid           ( new std::vector<int> ());
  std::auto_ptr<std::vector<int> >     chpdrbxid        ( new std::vector<int> ());
  std::auto_ptr<std::vector<int> >     chpdistagged     ( new std::vector<int> ());
  std::auto_ptr<std::vector<int> >     chpdnhits        ( new std::vector<int> ());
  std::auto_ptr<std::vector<int> >     chpdisiso        ( new std::vector<int> ());
  std::auto_ptr<std::vector<double> >  chpdisolhcale    ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  chpdisolecale    ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  chpdisoltrke     ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  chpdtrkfide      ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  chpdclue         ( new std::vector<double> ());
  // dihit clusters
  std::auto_ptr<std::vector<int> >     cdihitistagged   ( new std::vector<int> ());
  std::auto_ptr<std::vector<int> >     cdihitisiso      ( new std::vector<int> ());
  std::auto_ptr<std::vector<double> >  cdihitisolhcale  ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  cdihitisolecale  ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  cdihitisoltrke   ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  cdihittrkfide    ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  cdihitclue       ( new std::vector<double> ());
  // monohit clusters
  std::auto_ptr<std::vector<int> >     cmonohitistagged  ( new std::vector<int> ());
  std::auto_ptr<std::vector<int> >     cmonohitisiso     ( new std::vector<int> ());
  std::auto_ptr<std::vector<double> >  cmonohitisolhcale ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  cmonohitisolecale ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  cmonohitisoltrke  ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  cmonohittrkfide   ( new std::vector<double> ());
  std::auto_ptr<std::vector<double> >  cmonohitclue      ( new std::vector<double> ());

  // Isolation Filter code
  // Reproduced from: http://cmslxr.fnal.gov/lxr/source/RecoLocalCalo/HcalRecProducers/src/HBHEIsolatedNoiseReflagger.cc?v=CMSSW_7_5_0_pre5
  bool isodebug = false;
  
  //ObjectValidator objvalidator_;
  ObjectValidator objvalidator_ = ObjectValidator( 0.7, 0.8, 0.8, 0.07, 0.3, 9, 3, true, false, 0.3, 0.9, 5);
  //0074  ObjectValidator(double HBThreshold, double HESThreshold, double HEDThreshold, double EBThreshold, double EEThreshold,
  //0075           uint32_t HcalAcceptSeverityLevel, uint32_t EcalAcceptSeverityLevel, bool UseHcalRecoveredHits, bool UseEcalRecoveredHits,
  //0076           double MinValidTrackPt, double MinValidTrackPtBarrel, int MinValidTrackNHits)
  //
    
  // get the ECAL channel status map
  edm::ESHandle<EcalChannelStatus> ecalChStatus;
  iSetup.get<EcalChannelStatusRcd>().get( ecalChStatus );
  const EcalChannelStatus* dbEcalChStatus = ecalChStatus.product();
  
  // get the HCAL channel status map  
  edm::ESHandle<HcalChannelQuality> hcalChStatus;    
  iSetup.get<HcalChannelQualityRcd>().get( "withTopo", hcalChStatus );
  const HcalChannelQuality* dbHcalChStatus = hcalChStatus.product();
 
  edm::ESHandle<HcalSeverityLevelComputer> hcalSevLvlComputerHndl;
  iSetup.get<HcalSeverityLevelComputerRcd>().get(hcalSevLvlComputerHndl);
  const HcalSeverityLevelComputer* hcalSevLvlComputer = hcalSevLvlComputerHndl.product();
  
  edm::ESHandle<EcalSeverityLevelAlgo> ecalSevLvlAlgoHndl;
  iSetup.get<EcalSeverityLevelAlgoRcd>().get(ecalSevLvlAlgoHndl);
  const EcalSeverityLevelAlgo* ecalSevLvlAlgo = ecalSevLvlAlgoHndl.product();
  
  // get the calotower mappings
  edm::ESHandle<CaloTowerConstituentsMap> ctcm;
  iSetup.get<IdealGeometryRecord>().get(ctcm);
  
  // get the HB/HE hits
  edm::Handle<HBHERecHitCollection> hbhehits_h;
  iEvent.getByLabel("hbheprereco", hbhehits_h);
  
   // get the ECAL hits
  edm::Handle<EcalRecHitCollection> ebhits_h;
  iEvent.getByLabel("ecalRecHit","EcalRecHitsEB", ebhits_h);
  edm::Handle<EcalRecHitCollection> eehits_h;
  iEvent.getByLabel("ecalRecHit","EcalRecHitsEE", eehits_h);
  
  // get the tracks
  edm::Handle<std::vector<reco::TrackExtrapolation> > trackextraps_h;
  iEvent.getByLabel("trackExtrapolator", trackextraps_h);
  
  objvalidator_.setHcalChannelQuality(dbHcalChStatus);
  objvalidator_.setEcalChannelStatus(dbEcalChStatus);
  objvalidator_.setHcalSeverityLevelComputer(hcalSevLvlComputer);
  objvalidator_.setEcalSeverityLevelAlgo(ecalSevLvlAlgo);
  objvalidator_.setEBRecHitCollection(&(*ebhits_h));
  objvalidator_.setEERecHitCollection(&(*eehits_h));

  // Parameters to study
  double     LooseHcalIsol_  = 0.08;
  double     LooseEcalIsol_  = 0.08;
  double     LooseTrackIsol_ = 0.10;
  double     TightHcalIsol_  = 0.04;
  double     TightEcalIsol_  = 0.04;
  double     TightTrackIsol_ = 0.05;

  double     LooseRBXEne1_  = 10.0;
  double     LooseRBXEne2_  = 300.0;
  int        LooseRBXHits1_ = 14;
  int        LooseRBXHits2_ = 10;
  double     TightRBXEne1_  = 10.0;
  double     TightRBXEne2_  = 50.0;
  int        TightRBXHits1_ = 12;
  int        TightRBXHits2_ = 10;

  double     LooseHPDEne1_  = 10.0;
  double     LooseHPDEne2_  = 80.0;
  int        LooseHPDHits1_ = 6;
  int        LooseHPDHits2_ = 3;
  double     TightHPDEne1_  = 10.0;
  double     TightHPDEne2_  = 25.0;
  int        TightHPDHits1_ = 5;
  int        TightHPDHits2_ = 3;

  double     LooseDiHitEne_   = 50.0;
  double     TightDiHitEne_   = 15.0;
  double     LooseMonoHitEne_ = 35.0;
  double     TightMonoHitEne_ = 15.0;
  
  //isolation filter
  std::vector<HBHEHitMap> rbxs;
  std::vector<HBHEHitMap> hpds;
  std::vector<HBHEHitMap> dihits;
  std::vector<HBHEHitMap> monohits;

  
  // organizer the hits
  PhysicsTowerOrganizer pto(iEvent, iSetup, hbhehits_h, ebhits_h, eehits_h, trackextraps_h, objvalidator_, *(ctcm.product()));
  HBHEHitMapOrganizer organizer(hbhehits_h, objvalidator_, pto);

  organizer.getRBXs(rbxs, LooseRBXEne1_<TightRBXEne1_ ? LooseRBXEne1_ : TightRBXEne1_);
  organizer.getHPDs(hpds, LooseHPDEne1_<TightHPDEne1_ ? LooseHPDEne1_ : TightHPDEne1_);
  organizer.getDiHits(dihits, LooseDiHitEne_<TightDiHitEne_ ? LooseDiHitEne_ : TightDiHitEne_);
  organizer.getMonoHits(monohits, LooseMonoHitEne_<TightMonoHitEne_ ? LooseMonoHitEne_ : TightMonoHitEne_);
  
  if(isodebug) std::cout<<"     rbxs.size(): "<<rbxs.size()<<std::endl;
  if(isodebug) std::cout<<"     hpds.size(): "<<hpds.size()<<std::endl;
  if(isodebug) std::cout<<"   dihits.size(): "<<dihits.size()<<std::endl;
  if(isodebug) std::cout<<" monohits.size(): "<<monohits.size()<<std::endl;

  HcalDetId hcalDetId__;

  // RBX CLUSTERS
  if(isodebug) std::cout<<"Loop over rbx clusters: "<<std::endl;
  for(int i=0; i<static_cast<int>(rbxs.size()); i++) {
    int nhits=rbxs[i].nHits();
    double ene=rbxs[i].hitEnergy();
    double trkfide=rbxs[i].hitEnergyTrackFiducial();
    double isolhcale=rbxs[i].hcalEnergySameTowers()+rbxs[i].hcalEnergyNeighborTowers();
    double isolecale=rbxs[i].ecalEnergySameTowers();
    double isoltrke=rbxs[i].trackEnergySameTowers()+rbxs[i].trackEnergyNeighborTowers();
    //
    crbxisolhcale->push_back( isolhcale );
    crbxisolecale->push_back( isolecale );
    crbxisoltrke->push_back( isoltrke );
    crbxtrkfide->push_back( trkfide );
    crbxclue->push_back( ene );
    crbxnhits->push_back( nhits );
    //
    if(isodebug) std::cout<<" rbxs["<<i<<"].nHits()/hitEnergy(): "<<rbxs[i].nHits()<<" / "<<rbxs[i].hitEnergy();
    for(HBHEHitMap::hitmap_const_iterator it=rbxs[i].beginHits(); it!=rbxs[i].endHits(); ++it){
      hcalDetId__ = HcalDetId((it->first) -> detid());
      int RBXIndex = HcalHPDRBXMap::indexRBX(hcalDetId__);
      crbxid->push_back( RBXIndex );
      if(isodebug) std::cout<<" RBXid: "<<RBXIndex<<std::endl;
      break;
    }
    //

    int crbxtag_=0;
    if( isolhcale/ene<LooseHcalIsol_ && isolecale/ene<LooseEcalIsol_ && isoltrke/ene<LooseTrackIsol_ ){
      crbxtag_+=1;
      if(isodebug) std::cout<<"   Loose Cluster";
    }
    if( isolhcale/ene<TightHcalIsol_ && isolecale/ene<TightEcalIsol_ && isoltrke/ene<TightTrackIsol_ ){ 
      crbxtag_+=2;
      if(isodebug) std::cout<<"   Tight Cluster";
    }
    crbxistagged->push_back(crbxtag_);
    //
    int crbxiso_=0;
    if( (isolhcale/ene<LooseHcalIsol_ && isolecale/ene<LooseEcalIsol_ && isoltrke/ene<LooseTrackIsol_ 
	 && ((trkfide>LooseRBXEne1_ && nhits>=LooseRBXHits1_) || (trkfide>LooseRBXEne2_ && nhits>=LooseRBXHits2_))) ){
      crbxiso_+=1;
      if(isodebug) std::cout<<"   Loose Iso-Noise TAG";
    }
    //
    if( (isolhcale/ene<TightHcalIsol_ && isolecale/ene<TightEcalIsol_ && isoltrke/ene<TightTrackIsol_ 
	 && ((trkfide>TightRBXEne1_ && nhits>=TightRBXHits1_) || (trkfide>TightRBXEne2_ && nhits>=TightRBXHits2_))) ){
      crbxiso_+=2;
      if(isodebug) std::cout<<"   Tight Iso-Noise TAG";
    }
    //
    if(isodebug) std::cout<<std::endl;
  }
  if(isodebug) std::cout<<std::endl;  

  // HPD CLUSTERS
  if(isodebug) std::cout<<"Loop over hpd clusters: "<<std::endl;
  for(int i=0; i<static_cast<int>(hpds.size()); i++) {
    int nhits=hpds[i].nHits();
    double ene=hpds[i].hitEnergy();
    double trkfide=hpds[i].hitEnergyTrackFiducial();
    double isolhcale=hpds[i].hcalEnergySameTowers()+hpds[i].hcalEnergyNeighborTowers();
    double isolecale=hpds[i].ecalEnergySameTowers();
    double isoltrke=hpds[i].trackEnergySameTowers()+hpds[i].trackEnergyNeighborTowers();
    //
    chpdisolhcale->push_back( isolhcale );
    chpdisolecale->push_back( isolecale );
    chpdisoltrke->push_back( isoltrke );
    chpdtrkfide->push_back( trkfide );
    chpdclue->push_back( ene );
    chpdnhits->push_back( nhits );
    //
    if(isodebug) std::cout<<" hpds["<<i<<"].nHits()/hitEnergy(): "<<hpds[i].nHits()<<" / "<<hpds[i].hitEnergy();
    for(HBHEHitMap::hitmap_const_iterator it=hpds[i].beginHits(); it!=hpds[i].endHits(); ++it){
      hcalDetId__ = HcalDetId((it->first) -> detid());
      int RBXIndex = HcalHPDRBXMap::indexRBX(hcalDetId__);
      int HPDIndex = HcalHPDRBXMap::indexHPD(hcalDetId__);
      chpdrbxid->push_back( RBXIndex );
      chpdid->push_back( HPDIndex );
      if(isodebug) std::cout<<" RBX/HPDid: "<<RBXIndex<<" / "<<HPDIndex<<std::endl;
      break;
    }

    int chpdtag_=0;
    if( isolhcale/ene<LooseHcalIsol_ && isolecale/ene<LooseEcalIsol_ && isoltrke/ene<LooseTrackIsol_ ){
      chpdtag_+=1;
      if(isodebug) std::cout<<"   Loose Cluster";
    }
    if( isolhcale/ene<TightHcalIsol_ && isolecale/ene<TightEcalIsol_ && isoltrke/ene<TightTrackIsol_ ){
      chpdtag_+=2;
      if(isodebug) std::cout<<"   Tight Cluster";      
    }
    chpdistagged->push_back(chpdtag_);

    //
    int chpdiso_=0;
    if( (isolhcale/ene<LooseHcalIsol_ && isolecale/ene<LooseEcalIsol_ && isoltrke/ene<LooseTrackIsol_ 
	 && ((trkfide>LooseHPDEne1_ && nhits>=LooseHPDHits1_) || (trkfide>LooseHPDEne2_ && nhits>=LooseHPDHits2_))) ){
      chpdiso_+=1;
      if(isodebug) std::cout<<"   Loose Iso-Noise TAG";
    }
    if( (isolhcale/ene<TightHcalIsol_ && isolecale/ene<TightEcalIsol_ && isoltrke/ene<TightTrackIsol_ 
	 && ((trkfide>TightHPDEne1_ && nhits>=TightHPDHits1_) || (trkfide>TightHPDEne2_ && nhits>=TightHPDHits2_))) ){
      chpdiso_+=2;
      if(isodebug) std::cout<<"   Tight Iso-Noise TAG";
    }
    chpdisiso->push_back(chpdiso_);
    if(isodebug) std::cout<<std::endl;
  }
  if(isodebug) std::cout<<std::endl;

  // DIHIT CLUSTERS
  if(isodebug) std::cout<<"Loop over dihit clusters: "<<std::endl;
  for(int i=0; i<static_cast<int>(dihits.size()); i++) {
    double ene=dihits[i].hitEnergy();
    double trkfide=dihits[i].hitEnergyTrackFiducial();
    double isolhcale=dihits[i].hcalEnergySameTowers()+dihits[i].hcalEnergyNeighborTowers();
    double isolecale=dihits[i].ecalEnergySameTowers();
    double isoltrke=dihits[i].trackEnergySameTowers()+dihits[i].trackEnergyNeighborTowers();
    //
    cdihitisolhcale->push_back( isolhcale );
    cdihitisolecale->push_back( isolecale );
    cdihitisoltrke->push_back( isoltrke );
    cdihittrkfide->push_back( trkfide );
    cdihitclue->push_back( ene );
    //
    if(isodebug) std::cout<<" dihits["<<i<<"].nHits()/hitEnergy(): "<<dihits[i].nHits()<<" / "<<dihits[i].hitEnergy();
    //
    int cdihittag_=0;
    if( isolhcale/ene<LooseHcalIsol_ && isolecale/ene<LooseEcalIsol_ && isoltrke/ene<LooseTrackIsol_ ){
      cdihittag_+=1;
      if(isodebug) std::cout<<"   Loose Cluster";
    }
    if( isolhcale/ene<TightHcalIsol_ && isolecale/ene<TightEcalIsol_ && isoltrke/ene<TightTrackIsol_ ){
      cdihittag_+=2;
      if(isodebug) std::cout<<"   Tight Cluster";      
    }
    cdihitistagged->push_back(cdihittag_);

    //
    int cdihitiso_=0;
    if( isolhcale/ene<LooseHcalIsol_ && isolecale/ene<LooseEcalIsol_ && isoltrke/ene<LooseTrackIsol_
	&& trkfide>0.99*ene && trkfide>LooseDiHitEne_ ){
      cdihitiso_+=1;
      if(isodebug) std::cout<<"   Loose Iso-Noise TAG";
    }
    if( isolhcale/ene<TightHcalIsol_ && isolecale/ene<TightEcalIsol_ && isoltrke/ene<TightTrackIsol_ 
	&& ene>TightDiHitEne_ ){
      cdihitiso_+=2;
      if(isodebug) std::cout<<"   Tight Iso-Noise TAG";
    }
    cdihitisiso->push_back(cdihitiso_);
    if(isodebug) std::cout<<std::endl;
  }
  if(isodebug) std::cout<<std::endl;

  // MONOHIT CLUSTERS
  if(isodebug) std::cout<<"Loop over monohit clusters: "<<std::endl;
  for(int i=0; i<static_cast<int>(monohits.size()); i++) {
    double ene=monohits[i].hitEnergy();
    double trkfide=monohits[i].hitEnergyTrackFiducial();
    double isolhcale=monohits[i].hcalEnergySameTowers()+monohits[i].hcalEnergyNeighborTowers();
    double isolecale=monohits[i].ecalEnergySameTowers();
    double isoltrke=monohits[i].trackEnergySameTowers()+monohits[i].trackEnergyNeighborTowers();
    //
    cmonohitisolhcale->push_back( isolhcale );
    cmonohitisolecale->push_back( isolecale );
    cmonohitisoltrke->push_back( isoltrke );
    cmonohittrkfide->push_back( trkfide );
    cmonohitclue->push_back( ene );
    //
    if(isodebug) std::cout<<" monohits["<<i<<"].nHits()/hitEnergy(): "<<monohits[i].nHits()<<" / "<<monohits[i].hitEnergy();
    //
    int cmonohittag_=0;
    if( isolhcale/ene<LooseHcalIsol_ && isolecale/ene<LooseEcalIsol_ && isoltrke/ene<LooseTrackIsol_ ){
      cmonohittag_+=1;
      if(isodebug) std::cout<<"   Loose Cluster";
    }
    if( isolhcale/ene<TightHcalIsol_ && isolecale/ene<TightEcalIsol_ && isoltrke/ene<TightTrackIsol_ ){
      cmonohittag_+=2;
      if(isodebug) std::cout<<"   Tight Cluster";      
    }
    cmonohitistagged->push_back(cmonohittag_);
    //
    int cmonohitiso_=0;
    if( isolhcale/ene<LooseHcalIsol_ && isolecale/ene<LooseEcalIsol_ && isoltrke/ene<LooseTrackIsol_ 
	&& trkfide>0.99*ene && trkfide>LooseMonoHitEne_ ){
      cmonohitiso_+=1;
      if(isodebug) std::cout<<"   Loose Iso-Noise TAG";
    }
    if( isolhcale/ene<TightHcalIsol_ && isolecale/ene<TightEcalIsol_ && isoltrke/ene<TightTrackIsol_ 
	&& ene>TightMonoHitEne_ ){
      cmonohitiso_+=2;
      if(isodebug) std::cout<<"   Tight Iso-Noise TAG";
    }
    cmonohitisiso->push_back(cmonohitiso_);
    //
    if(isodebug) std::cout<<std::endl;
  }
  if(isodebug) std::cout<<std::endl;


  edm::Handle<HcalNoiseSummary> hSummary;
  iEvent.getByLabel(noiseSummaryInputTag, hSummary);

  if(isodebug) std::cout<<suffix<<"  hSummary->numIsolatedNoiseChannels(): "<<hSummary->numIsolatedNoiseChannels()<<std::endl;
  if(isodebug) std::cout<<suffix<<"  hSummary->isolatedNoiseSumE()       : "<<hSummary->isolatedNoiseSumE()<<std::endl;
  if(isodebug) std::cout<<suffix<<"  hSummary->isolatedNoiseSumEt()      : "<<hSummary->isolatedNoiseSumEt()<<std::endl;

  //
  //numisolatednoisechannels -> push_back ( hSummary->numIsolatedNoiseChannels() );
  //isolatednoisesume        -> push_back ( hSummary->isolatedNoiseSumE() );
  //isolatednoisesumet       -> push_back ( hSummary->isolatedNoiseSumEt() );


  /*
  edm::Handle<HBHERecHitCollection> hRecHits;
  iEvent.getByLabel(recoInputTag, hRecHits);

  edm::Handle<HBHEDigiCollection> hHBHEDigis;
  if( isRAW ) iEvent.getByLabel("hcalDigis", hHBHEDigis);

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

  if( isRAW ){
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
      //debugging
      //if( (*hRecHits)[RecHitIndex[id]].energy()>50 ){
      //std::cout<<suffix<<"  RechitEnergy/iEta/iPhi/iDepth: "<< (*hRecHits)[RecHitIndex[id]].energy() <<" / "<<id.ieta()<<" / "<< id.iphi()<<" / "<< id.depth();
      //std::cout<<suffix<<"                          Bit11: "<< (( (*hRecHits)[RecHitIndex[id]].flags() >> 11 ) & 1) <<std::endl;
      //std::cout<< <<std::endl;
      //}
      
      // First convert ADC to deposited charge
      const HcalCalibrations &Calibrations = hConditions->getHcalCalibrations(id);
      const HcalQIECoder *ChannelCoder = hConditions->getHcalCoder(id);
      const HcalQIEShape *Shape = hConditions->getHcalShape(id);
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
    
    //debugging
    // for(int i = 0; i < 72; i++){
    //if( RBXEnergy[i]>0 )
    //std::cout<<"RBX "<<i<<"  : "<<RBXEnergy[i]<<std::endl;
    //}
    //

    // RBX charge and energy vectors are filled in
    for(int irbx = 0; irbx < 72; irbx++){
      std::vector<double> RBXChargevector(   std::begin(RBXCharge[irbx]), std::end(RBXCharge[irbx]) );
      std::vector<double> RBXCharge15vector( std::begin(RBXCharge15[irbx]), std::end(RBXCharge15[irbx]) );
      //rbxcharge   -> push_back( RBXCharge[irbx]   );
      //rbxcharge15 -> push_back( RBXCharge15[irbx] );
      rbxcharge   -> push_back( RBXChargevector   );
      rbxcharge15 -> push_back( RBXCharge15vector );
      rbxenergy   -> push_back( RBXEnergy[irbx]   );
      rbxenergy15 -> push_back( RBXEnergy15[irbx] );
    }  
  }
  */

  // iso noise filter params
  //iEvent.put( numisolatednoisechannels , prefix + "NumIsolatedNoiseChannels" + suffix );
  //iEvent.put( isolatednoisesume        , prefix + "IsolatedNoiseSumE"        + suffix );
  //iEvent.put( isolatednoisesumet       , prefix + "IsolatedNoiseSumEt"       + suffix );
  //
  // rbx clusters
  iEvent.put(  crbxid           , prefix + "CRBXId"           + suffix );
  iEvent.put(  crbxnhits        , prefix + "CRBXNHits"        + suffix );
  iEvent.put(  crbxistagged     , prefix + "CRBXIsTagged"     + suffix );
  iEvent.put(  crbxisiso        , prefix + "CRBXIsIso"        + suffix );
  iEvent.put(  crbxisolhcale    , prefix + "CRBXIsolHcalE"    + suffix );
  iEvent.put(  crbxisolecale    , prefix + "CRBXIsolEcalE"    + suffix );
  iEvent.put(  crbxisoltrke     , prefix + "CRBXIsolTrkE"     + suffix );
  iEvent.put(  crbxtrkfide      , prefix + "CRBXTrkFidE"      + suffix );
  iEvent.put(  crbxclue         , prefix + "CRBXCluE"         + suffix );
  // hpd clusters
  iEvent.put(  chpdid           , prefix + "CHPDId"           + suffix );
  iEvent.put(  chpdrbxid        , prefix + "CHPDRBXId"        + suffix );
  iEvent.put(  chpdistagged     , prefix + "CHPDIsTagged"     + suffix );
  iEvent.put(  chpdisiso        , prefix + "CHPDIsIso"        + suffix );
  iEvent.put(  chpdnhits        , prefix + "CHPDNHits"        + suffix );
  iEvent.put(  chpdisolhcale    , prefix + "CHPDIsolHcalE"    + suffix );
  iEvent.put(  chpdisolecale    , prefix + "CHPDIsolEcalE"    + suffix );
  iEvent.put(  chpdisoltrke     , prefix + "CHPDIsolTrkE"     + suffix );
  iEvent.put(  chpdtrkfide      , prefix + "CHPDTrkFidE"      + suffix );
  iEvent.put(  chpdclue         , prefix + "CHPDCluE"         + suffix );
  // dihit clusters
  iEvent.put(  cdihitistagged   , prefix + "CDihitIsTagged"   + suffix );
  iEvent.put(  cdihitisiso      , prefix + "CDihitIsIso"      + suffix );
  iEvent.put(  cdihitisolhcale  , prefix + "CDihitIsolHcalE"  + suffix );
  iEvent.put(  cdihitisolecale  , prefix + "CDihitIsolEcalE"  + suffix );
  iEvent.put(  cdihitisoltrke   , prefix + "CDihitIsolTrkE"   + suffix );
  iEvent.put(  cdihittrkfide    , prefix + "CDihitTrkFidE"    + suffix );
  iEvent.put(  cdihitclue       , prefix + "CDihitCluE"       + suffix );
  // monohit clusters
  iEvent.put(  cmonohitistagged , prefix + "CMonohitIsTagged" + suffix );
  iEvent.put(  cmonohitisiso    , prefix + "CMonohitIsIso"    + suffix );
  iEvent.put(  cmonohitisolhcale, prefix + "CMonohitIsolHcalE"+ suffix );
  iEvent.put(  cmonohitisolecale, prefix + "CMonohitIsolEcalE"+ suffix );
  iEvent.put(  cmonohitisoltrke , prefix + "CMonohitIsolTrkE" + suffix );
  iEvent.put(  cmonohittrkfide  , prefix + "CMonohitTrkFidE"  + suffix );
  iEvent.put(  cmonohitclue     , prefix + "CMonohitCluE"     + suffix );
}
