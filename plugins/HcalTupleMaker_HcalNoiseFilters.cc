#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalNoiseFilters.h"
//#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
#include "RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseShapes.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
//#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDigi/interface/HBHEDataFrame.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"
#include "DataFormats/HcalDigi/interface/HFDataFrame.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include <iterator>
#include <vector>

#include <iostream>

using namespace std;

HcalTupleMaker_HcalNoiseFilters::HcalTupleMaker_HcalNoiseFilters(const edm::ParameterSet& iConfig):
    //noiseSummaryInputTag (iConfig.getUntrackedParameter<edm::InputTag>("noiseSummaryInputTag")),
    noiseSummaryInputToken (consumes<HcalNoiseSummary>(iConfig.getUntrackedParameter<edm::InputTag>("noiseSummaryInputTag"))),
    noiseResultInputTag    (iConfig.getUntrackedParameter<std::string>("noiseResultInputTag")), 
    //recoInputTag         (iConfig.getUntrackedParameter<std::string>("recoInputTag")),
    recoInputToken         (consumes<HBHERecHitCollection>(iConfig.getUntrackedParameter<std::string>("recoInputTag"))),
    recoHFInputToken       (consumes<HFRecHitCollection>(iConfig.getUntrackedParameter<std::string>("recoHFInputTag"))),
    recoVertexInputToken   (consumes<reco::VertexCollection>(iConfig.getUntrackedParameter<std::string>("recoVertexInputTag"))), 
    isRECO                 (iConfig.getUntrackedParameter<bool>("isRECO")),
    prefix                 (iConfig.getUntrackedParameter<std::string>("Prefix")),
    suffix                 (iConfig.getUntrackedParameter<std::string>("Suffix")),
    defaultNoiseResultInputToken   (consumes<bool>(edm::InputTag(noiseResultInputTag,"HBHENoiseFilterResult"))),
    run1NoiseResultInputToken      (consumes<bool>(edm::InputTag(noiseResultInputTag,"HBHENoiseFilterResultRun1"))),
    run2LooseNoiseResultInputToken (consumes<bool>(edm::InputTag(noiseResultInputTag,"HBHENoiseFilterResultRun2Loose"))),
    run2TightNoiseResultInputToken (consumes<bool>(edm::InputTag(noiseResultInputTag,"HBHENoiseFilterResultRun2Tight"))),
    isoNoiseResultInputToken       (consumes<bool>(edm::InputTag(noiseResultInputTag,"HBHEIsoNoiseFilterResult")))
{
    produces <std::vector<int> >                  (prefix + "OfficialDecision"         + suffix );
    produces <std::vector<int> >                  (prefix + "OfficialDecisionRun1"     + suffix );
    produces <std::vector<int> >                  (prefix + "OfficialDecisionRun2L"    + suffix );
    produces <std::vector<int> >                  (prefix + "OfficialDecisionRun2T"    + suffix );
    produces <std::vector<int> >                  (prefix + "IsoNoiseFilterDecision"   + suffix );
    produces <std::vector<int> >                  (prefix + "HPDHits"                  + suffix );
    produces <std::vector<int> >                  (prefix + "HPDNoOtherHits"           + suffix );
    produces <std::vector<int> >                  (prefix + "MaxZeros"                 + suffix );
    produces <std::vector<double> >               (prefix + "MinE2E10"                 + suffix );
    produces <std::vector<double> >               (prefix + "MaxE2E10"                 + suffix );
    produces <std::vector<int> >                  (prefix + "HasBadRBXR45"             + suffix );
    produces <std::vector<int> >                  (prefix + "HasBadRBXRechitR45Loose"  + suffix );
    produces <std::vector<int> >                  (prefix + "HasBadRBXRechitR45Tight"  + suffix );
    produces <std::vector<int> >                  (prefix + "NumIsolatedNoiseChannels" + suffix );
    produces <std::vector<double> >               (prefix + "IsolatedNoiseSumE"        + suffix );
    produces <std::vector<double> >               (prefix + "IsolatedNoiseSumEt"       + suffix );
    produces <std::vector<int> >                  (prefix + "NumNegativeNoiseChannels" + suffix );
    produces <std::vector<double> >               (prefix + "NegativeNoiseSumE"        + suffix );
    produces <std::vector<double> >               (prefix + "NegativeNoiseSumEt"       + suffix );
    produces <std::vector<int> >                  (prefix + "NumSpikeNoiseChannels"    + suffix );
    produces <std::vector<double> >               (prefix + "SpikeNoiseSumE"           + suffix );
    produces <std::vector<double> >               (prefix + "SpikeNoiseSumEt"          + suffix ); 
    produces <std::vector<bool> >                 (prefix + "goodJetFoundInLowBVRegion"+ suffix );
    //
    // Used to be from RAW, now from RECO-aux
    produces <std::vector<std::vector<double> > > (prefix + "RBXCharge"                + suffix );
    produces <std::vector<std::vector<double> > > (prefix + "RBXCharge15"              + suffix );
    produces <std::vector<double> >               (prefix + "RBXEnergy"                + suffix );
    produces <std::vector<double> >               (prefix + "RBXEnergy15"              + suffix );
    //
    // Method-0 rechit collection accessed via eraw()
    produces <std::vector<double> >               (prefix + "HBHERecHitEnergyRaw"      + suffix );
    produces <std::vector<double> >               (prefix + "HBHERecHitEnergyAux"      + suffix );
    produces <std::vector<float> >                (prefix + "HBHERecHitChi2"           + suffix );
    produces <std::vector<int> >                  (prefix + "HBHERecHitSevLvl"         + suffix );
    produces <std::vector<uint32_t> >             (prefix + "HBHERecHitAux"            + suffix );
    produces <std::vector<uint32_t> >             (prefix + "HBHERecHitAuxHBHE"        + suffix );
    produces <std::vector<uint32_t> >             (prefix + "HBHERecHitAuxPhase1"      + suffix );
    //
    produces <std::vector<std::vector<int> > >     (prefix + "HBHERecHitAuxCapID"       + suffix );
    produces <std::vector<std::vector<int> > >     (prefix + "HBHERecHitAuxADC"         + suffix );
    produces <std::vector<std::vector<double> > >  (prefix + "HBHERecHitAuxAllfC"       + suffix );
    produces <std::vector<std::vector<double> > >  (prefix + "HBHERecHitAuxPedFC"       + suffix );
    produces <std::vector<std::vector<double> > >  (prefix + "HBHERecHitAuxGain"        + suffix );
    produces <std::vector<std::vector<double> > >  (prefix + "HBHERecHitAuxRCGain"      + suffix );
    produces <std::vector<std::vector<double> > >  (prefix + "HBHERecHitAuxFC"          + suffix );
    produces <std::vector<std::vector<double> > >  (prefix + "HBHERecHitAuxEnergy"      + suffix );
    // 
    produces <std::vector<int> >                   (prefix + "HFRecHitSevLvl"           + suffix );
    produces <std::vector<uint32_t> >              (prefix + "HFRecHitAux"              + suffix );
    produces <std::vector<uint32_t> >              (prefix + "HFRecHitAuxHF"            + suffix );
    // 
    produces <std::vector<int> >                   (prefix + "Nvtx"                     + suffix );
}

void HcalTupleMaker_HcalNoiseFilters::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

    std::unique_ptr<std::vector<int> >                  officialdecision           ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  officialdecisionrun1       ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  officialdecisionrun2l      ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  officialdecisionrun2t      ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  isonoisefilterdecision     ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  hpdhits                    ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  hpdnootherhits             ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  maxzeros                   ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<double> >               mine2e10                   ( new std::vector<double>               ());
    std::unique_ptr<std::vector<double> >               maxe2e10                   ( new std::vector<double>               ());
    std::unique_ptr<std::vector<int> >                  hasbadrbxr45               ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  hasbadrbxrechitr45loose    ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  hasbadrbxrechitr45tight    ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<int> >                  numisolatednoisechannels   ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<double> >               isolatednoisesume          ( new std::vector<double>               ());
    std::unique_ptr<std::vector<double> >               isolatednoisesumet         ( new std::vector<double>               ());
    std::unique_ptr<std::vector<int> >                  numnegativenoisechannels   ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<double> >               negativenoisesume          ( new std::vector<double>               ());
    std::unique_ptr<std::vector<double> >               negativenoisesumet         ( new std::vector<double>               ());
    std::unique_ptr<std::vector<int> >                  numspikenoisechannels      ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<double> >               spikenoisesume             ( new std::vector<double>               ());
    std::unique_ptr<std::vector<double> >               spikenoisesumet            ( new std::vector<double>               ());
    std::unique_ptr<std::vector<bool> >                 goodjetinlowbv             ( new std::vector<bool>               ());
    //
    std::unique_ptr<std::vector<std::vector<double> > > rbxcharge                  ( new std::vector<std::vector<double> > ());
    std::unique_ptr<std::vector<std::vector<double> > > rbxcharge15                ( new std::vector<std::vector<double> > ());
    std::unique_ptr<std::vector<double> >               rbxenergy                  ( new std::vector<double>               ());
    std::unique_ptr<std::vector<double> >               rbxenergy15                ( new std::vector<double>               ());
    //
    std::unique_ptr<std::vector<double> >               hbherechitenergyraw        ( new std::vector<double>               ());
    std::unique_ptr<std::vector<double> >               hbherechitenergyaux        ( new std::vector<double>               ());
    std::unique_ptr<std::vector<float> >                hbherechitchi2             ( new std::vector<float>                ());
    std::unique_ptr<std::vector<int> >                  hbherechitsevlvl           ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<uint32_t> >             hbherechitaux              ( new std::vector<uint32_t>                  ());
    std::unique_ptr<std::vector<uint32_t> >             hbherechitauxhbhe          ( new std::vector<uint32_t>                  ());
    std::unique_ptr<std::vector<uint32_t> >             hbherechitauxphase1        ( new std::vector<uint32_t>                  ());
    //
    std::unique_ptr<std::vector<std::vector<int> > >    hbherechitauxcapid         ( new std::vector<std::vector<int> >    ());   
    std::unique_ptr<std::vector<std::vector<int> > >    hbherechitauxadc           ( new std::vector<std::vector<int> >    ());   
    std::unique_ptr<std::vector<std::vector<double> > > hbherechitauxallfc         ( new std::vector<std::vector<double> > ());   
    std::unique_ptr<std::vector<std::vector<double> > > hbherechitauxpedfc         ( new std::vector<std::vector<double> > ());
    std::unique_ptr<std::vector<std::vector<double> > > hbherechitauxgain          ( new std::vector<std::vector<double> > ());
    std::unique_ptr<std::vector<std::vector<double> > > hbherechitauxrcgain        ( new std::vector<std::vector<double> > ());
    std::unique_ptr<std::vector<std::vector<double> > > hbherechitauxfc            ( new std::vector<std::vector<double> > ());
    std::unique_ptr<std::vector<std::vector<double> > > hbherechitauxenergy        ( new std::vector<std::vector<double> > ());
    //
    std::unique_ptr<std::vector<int> >                  hfrechitsevlvl             ( new std::vector<int>                  ());
    std::unique_ptr<std::vector<uint32_t> >             hfrechitaux                ( new std::vector<uint32_t>                  ());
    std::unique_ptr<std::vector<uint32_t> >             hfrechitauxhf              ( new std::vector<uint32_t>                  ());
    std::unique_ptr<std::vector<int> >                  nvtx                       ( new std::vector<int>                  ());

    // HCAL noise results 
    edm::Handle<bool> hNoiseResult;
    iEvent.getByToken(defaultNoiseResultInputToken, hNoiseResult);

    edm::Handle<bool> hNoiseResult_Run1;
    iEvent.getByToken(run1NoiseResultInputToken, hNoiseResult_Run1);

    edm::Handle<bool> hNoiseResult_Run2Loose;
    iEvent.getByToken(run2LooseNoiseResultInputToken, hNoiseResult_Run2Loose);

    edm::Handle<bool> hNoiseResult_Run2Tight;
    iEvent.getByToken(run2TightNoiseResultInputToken, hNoiseResult_Run2Tight);

    edm::Handle<bool> hNoiseResult_IsoNoiseFilter;
    iEvent.getByToken(isoNoiseResultInputToken, hNoiseResult_IsoNoiseFilter);
    
    // HCAL Topology 
    edm::ESHandle<HcalTopology> topo;
    iSetup.get<HcalRecNumberingRecord>().get(topo);
    theHcalTopology = topo.product();

    // get the HCAL channel status map
    edm::ESHandle<HcalChannelQuality> hcalChStatus;
    iSetup.get<HcalChannelQualityRcd>().get( "withTopo", hcalChStatus );
    dbHcalChStatus = hcalChStatus.product();

    // HCAL Severity level 
    edm::ESHandle<HcalSeverityLevelComputer> hcalSevLvlComputerHndl;
    iSetup.get<HcalSeverityLevelComputerRcd>().get(hcalSevLvlComputerHndl);
    hcalSevLvlComputer = hcalSevLvlComputerHndl.product();

    // HCAL Filter Decision
    officialdecision        -> push_back( *hNoiseResult );
    officialdecisionrun1    -> push_back( *hNoiseResult_Run1 );
    officialdecisionrun2l   -> push_back( *hNoiseResult_Run2Loose );
    officialdecisionrun2t   -> push_back( *hNoiseResult_Run2Tight );
    isonoisefilterdecision  -> push_back( *hNoiseResult_IsoNoiseFilter );

    // Number of vertices 
    if(isRECO) 
    {
      edm::Handle<reco::VertexCollection> vtx;
      iEvent.getByToken(recoVertexInputToken, vtx); 
      nvtx->push_back(vtx->size());
    }

    // HCAL summary objects
    edm::Handle<HcalNoiseSummary> hSummary;
    iEvent.getByToken(noiseSummaryInputToken, hSummary);
    //
    hpdhits                  -> push_back ( hSummary->maxHPDHits() );
    hpdnootherhits           -> push_back ( hSummary->maxHPDNoOtherHits() );
    maxzeros                 -> push_back ( hSummary->maxZeros() );
    mine2e10                 -> push_back ( hSummary->minE2Over10TS() );
    maxe2e10                 -> push_back ( hSummary->maxE2Over10TS() );
    hasbadrbxr45             -> push_back ( hSummary->HasBadRBXTS4TS5() );
    hasbadrbxrechitr45loose  -> push_back ( hSummary->HasBadRBXRechitR45Loose() );
    hasbadrbxrechitr45tight  -> push_back ( hSummary->HasBadRBXRechitR45Tight() );
    numisolatednoisechannels -> push_back ( hSummary->numIsolatedNoiseChannels() );
    isolatednoisesume        -> push_back ( hSummary->isolatedNoiseSumE() );
    isolatednoisesumet       -> push_back ( hSummary->isolatedNoiseSumEt() );
    numnegativenoisechannels -> push_back ( hSummary->numNegativeNoiseChannels() );
    negativenoisesume        -> push_back ( hSummary->NegativeNoiseSumE() );
    negativenoisesumet       -> push_back ( hSummary->NegativeNoiseSumEt() );
    numspikenoisechannels    -> push_back ( hSummary->numSpikeNoiseChannels() );
    spikenoisesume           -> push_back ( hSummary->spikeNoiseSumE() );
    spikenoisesumet          -> push_back ( hSummary->spikeNoiseSumEt() );
    goodjetinlowbv           -> push_back ( hSummary->goodJetFoundInLowBVRegion() );

    // HCAL rechit collections
    edm::Handle<HBHERecHitCollection> hRecHits;
    iEvent.getByToken(recoInputToken, hRecHits);
    
    edm::Handle<HFRecHitCollection> hRecHitsHF;
    iEvent.getByToken(recoHFInputToken, hRecHitsHF);

    // HCAL DB service to extract conditions 
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
   /* // FIXME 
    // AUX charge, gain, etc. values..
    int    auxcapid[8];
    int    auxadc[8];
    double auxallfc[8];
    double auxpedfc[8];
    double auxrcgain[8];
    double auxfc[8];
    double auxenergy[8];
    double auxgain[8];
   */ 
    // Reset values to 0
    for(int i = 0; i < 72; i++){
        for(int j = 0; j < 10; j++){
            RBXCharge[i][j] = 0;
            RBXCharge15[i][j] = 0;
        }
        RBXEnergy[i] = 0;
        RBXEnergy15[i] = 0;
        /* // FIXME
        if( i < 8 ){
            auxcapid[i]=0;
            auxadc[i]=0;
            auxallfc[i]=0;
            auxpedfc[i]=0;
            auxrcgain[i]=0;
            auxfc[i]=0;
            auxenergy[i]=0;
            auxgain[i]=0;
        }
        */
    }

    // loop over HF rechits    
    for(HFRecHitCollection::const_iterator j = hRecHitsHF->begin(); j != hRecHitsHF->end(); j++){
        
        HcalDetId cell(j->id());
      
       // Severity level 
       int severityLevel = hcalSevLvl( (CaloRecHit*) &*j );
       hfrechitsevlvl->push_back( severityLevel );
       hfrechitaux->push_back(j->aux());
       hfrechitauxhf->push_back(j->getAuxHF());
    }

    // loop over HBHE rechits
    for(HBHERecHitCollection::const_iterator j = hRecHits->begin(); j != hRecHits->end(); j++){ 
        hbherechitenergyraw->push_back( j->eraw() ); //always method-0 rechit energy (raw energy).
        hbherechitenergyaux->push_back( j->eaux() );
        hbherechitchi2->push_back( j->chi2() );
  /* // FIXME     
        // Reset values to 0
        for(int j = 0; j < 8; j++){
            auxcapid[j]=0;
            auxadc[j]=0;
            auxallfc[j]=0;
            auxpedfc[j]=0;
            auxrcgain[j]=0;
            auxfc[j]=0;
            auxenergy[j]=0;
            auxgain[j]=0;
        }
        
        // Extracting corresponding digi/pulse-shape information for the rechit via the auxword
        // Code borrowed from S. Abdullin: /afs/cern.ch/user/a/abdullin/public/AUX_word_usage/HcalRecHitsValidation.cc
        HcalDetId cell(j->id());
        //const CaloCellGeometry* cellGeometry =  geometry->getSubdetectorGeometry (cell)->getGeometry (cell) ; //for eta-phi, but no need
      
        // http://cmslxr.fnal.gov/source/DataFormats/HcalRecHit/src/HBHERecHitAuxSetter.cc
        int auxwd1 = j->aux();        // TS = 0,1,2,3 info
        int auxwd2 = j->auxHBHE();    // TS = 4,5,6,7 info
        int auxwd3 = j->auxPhase1();  // capid info

        // Severity level 
        int severityLevel = hcalSevLvl( (CaloRecHit*) &*j );
        hbherechitsevlvl->push_back( severityLevel );
        hbherechitaux->push_back(auxwd1);
        hbherechitauxhbhe->push_back(auxwd2);
        hbherechitauxphase1->push_back(auxwd3);
        
        // FIXME : select particular channels 
        //if ((cell.subdet()==2 && cell.iphi()>=63 && cell.iphi()<=66 && cell.ieta()>0)) continue;  
       
        //
        CaloSamples tool;
        const HcalCalibrations calibrations = hConditions->getHcalCalibrations(cell);
        const HcalQIECoder *channelCoder = hConditions->getHcalCoder(cell);
        const HcalQIEShape *shape = hConditions->getHcalShape(channelCoder);
        HcalCoderDb coder(*channelCoder, *shape);

        int adc[8]; // raw (non-linearized) ADC counts
        int capid[8];

        adc[0] = (auxwd1)       & 0xff;
        adc[1] = (auxwd1 >> 8)  & 0xff;
        adc[2] = (auxwd1 >> 16) & 0xff;
        adc[3] = (auxwd1 >> 24) & 0xff;
        adc[4] = (auxwd2)       & 0xff;
        adc[5] = (auxwd2 >> 8)  & 0xff;
        adc[6] = (auxwd2 >> 16) & 0xff;
        adc[7] = (auxwd2 >> 24) & 0xff;
        // capid for soi
        int capid_for_soi = (auxwd3 >> 24) & 0x3;  // rotating through 4 values: 0,1,2,3
        capid[0] = capid_for_soi - 3; // calculate assuming soi is 3        
        if(capid[0]<0) capid[0] = capid[0] + 4;
        capid[1] = (capid[0] + 1 <= 3) ? capid[0] + 1 : 0;
        capid[2] = (capid[1] + 1 <= 3) ? capid[1] + 1 : 0;
        capid[3] = (capid[2] + 1 <= 3) ? capid[2] + 1 : 0;
        capid[4] = capid[0]; 
        capid[5] = capid[1];
        capid[6] = capid[2];
        capid[7] = capid[3];

        HBHEDataFrame digi(cell);
        digi.setSize(8);
        digi.setPresamples(3);
        //digi.setSize(10);
        //digi.setPresamples(4);
        for (int iTS = 0; iTS < 8; iTS++) {
            HcalQIESample s (adc[iTS], capid[iTS], 0, 0);
            digi.setSample(iTS,s);
        }    

        coder.adc2fC(digi, tool);

        for (int iTS = 0; iTS < 8; iTS++) {
            auxcapid[iTS]  = (int)(capid[iTS]); //just a naming change..
            auxadc[iTS]    = (int)(adc[iTS]);   //just a naming change..
            auxallfc[iTS]  = tool[iTS];
            auxpedfc[iTS]  = calibrations.pedestal     ( capid[iTS] );
            auxrcgain[iTS] = calibrations.respcorrgain ( capid[iTS] );
            auxfc[iTS]     = auxallfc[iTS] - auxpedfc[iTS];
            auxenergy[iTS] = auxfc[iTS] * auxrcgain[iTS];
            auxgain[iTS]   = calibrations.rawgain( capid[iTS] );
        }

        //converting arrays into vectors
        std::vector<int>    auxcapidvector(   std::begin(auxcapid), std::end(auxcapid)  );
        std::vector<int>    auxadcvector(     std::begin(auxadc),   std::end(auxadc)    );
        std::vector<double> auxallfcvector(   std::begin(auxallfc), std::end(auxallfc)  );
        std::vector<double> auxpedfcvector(   std::begin(auxpedfc), std::end(auxpedfc)  );
        std::vector<double> auxrcgainvector(  std::begin(auxrcgain),std::end(auxrcgain) );
        std::vector<double> auxfcvector(      std::begin(auxfc),    std::end(auxfc)     );
        std::vector<double> auxenergyvector(  std::begin(auxenergy),std::end(auxenergy) );
        std::vector<double> auxgainvector(    std::begin(auxgain),  std::end(auxgain)   );
        hbherechitauxcapid  -> push_back( auxcapidvector  );
        hbherechitauxadc    -> push_back( auxadcvector    );
        hbherechitauxallfc  -> push_back( auxallfcvector  );
        hbherechitauxpedfc  -> push_back( auxpedfcvector  );
        hbherechitauxrcgain -> push_back( auxrcgainvector );
        hbherechitauxfc     -> push_back( auxfcvector     );
        hbherechitauxenergy -> push_back( auxenergyvector );
        hbherechitauxgain   -> push_back( auxgainvector   );

        //debugging
        //if( j->eraw()> 2){
        //std::cout<<"Eraw: "<<j->eraw()<<"   ";
        //std::cout<<"Charge: ";
        //for( int iTS=0; iTS<8; iTS++){
        //if(iTS==4) std::cout<< auxfcvector.at(iTS)<<"*  ";
        //else std::cout<< auxfcvector.at(iTS) <<"  ";
        //}
        //std::cout<<std::endl;
        //}

        // Calculate RBX total charge, total energy, using rechit info..
        HcalDetId hcalDetId = HcalDetId(j -> detid());
        int RBXIndex = HcalHPDRBXMap::indexRBX(hcalDetId);
        //
        for(int iTS = 0; iTS<8; iTS++){// loop over TS's
            RBXCharge[RBXIndex][iTS] = RBXCharge[RBXIndex][iTS] + auxfc[iTS];// RBX pulse shape
            if( j->eraw() > 1.5 )
                RBXCharge15[RBXIndex][iTS] = RBXCharge15[RBXIndex][iTS] + auxfc[iTS];// RBX pulse shape for rechits > 1.5 GeV
        }
        //
        RBXEnergy[RBXIndex] = RBXEnergy[RBXIndex] + j->eraw();
        if( j->eraw() > 1.5 )
            RBXEnergy15[RBXIndex] = RBXEnergy15[RBXIndex] + j->eraw();
*/
    }//loop over rechits


    // RBX charge and energy vectors are filled in
    for(int irbx = 0; irbx < 72; irbx++){
        std::vector<double> RBXChargevector(   std::begin(RBXCharge[irbx]), std::end(RBXCharge[irbx]) );
        std::vector<double> RBXCharge15vector( std::begin(RBXCharge15[irbx]), std::end(RBXCharge15[irbx]) );
        rbxcharge   -> push_back( RBXChargevector   );
        rbxcharge15 -> push_back( RBXCharge15vector );
        rbxenergy   -> push_back( RBXEnergy[irbx]   );
        rbxenergy15 -> push_back( RBXEnergy15[irbx] );
    }

    iEvent.put ( move( officialdecision         ), prefix + "OfficialDecision"         + suffix );
    iEvent.put ( move( officialdecisionrun1     ), prefix + "OfficialDecisionRun1"     + suffix );
    iEvent.put ( move( officialdecisionrun2l    ), prefix + "OfficialDecisionRun2L"    + suffix );
    iEvent.put ( move( officialdecisionrun2t    ), prefix + "OfficialDecisionRun2T"    + suffix );
    iEvent.put ( move( isonoisefilterdecision   ), prefix + "IsoNoiseFilterDecision"   + suffix );
    iEvent.put ( move( hpdhits                  ), prefix + "HPDHits"                  + suffix );
    iEvent.put ( move( hpdnootherhits           ), prefix + "HPDNoOtherHits"           + suffix );
    iEvent.put ( move( maxzeros                 ), prefix + "MaxZeros"                 + suffix );
    iEvent.put ( move( mine2e10                 ), prefix + "MinE2E10"                 + suffix );
    iEvent.put ( move( maxe2e10                 ), prefix + "MaxE2E10"                 + suffix );
    iEvent.put ( move( hasbadrbxr45             ), prefix + "HasBadRBXR45"             + suffix );
    iEvent.put ( move( hasbadrbxrechitr45loose  ), prefix + "HasBadRBXRechitR45Loose"  + suffix );
    iEvent.put ( move( hasbadrbxrechitr45tight  ), prefix + "HasBadRBXRechitR45Tight"  + suffix );
    iEvent.put ( move( numisolatednoisechannels ), prefix + "NumIsolatedNoiseChannels" + suffix );
    iEvent.put ( move( isolatednoisesume        ), prefix + "IsolatedNoiseSumE"        + suffix );
    iEvent.put ( move( isolatednoisesumet       ), prefix + "IsolatedNoiseSumEt"       + suffix );
    iEvent.put ( move( numnegativenoisechannels ), prefix + "NumNegativeNoiseChannels" + suffix );
    iEvent.put ( move( negativenoisesume        ), prefix + "NegativeNoiseSumE"        + suffix );
    iEvent.put ( move( negativenoisesumet       ), prefix + "NegativeNoiseSumEt"       + suffix );
    iEvent.put ( move( numspikenoisechannels    ), prefix + "NumSpikeNoiseChannels"    + suffix );
    iEvent.put ( move( spikenoisesume           ), prefix + "SpikeNoiseSumE"           + suffix );
    iEvent.put ( move( spikenoisesumet          ), prefix + "SpikeNoiseSumEt"          + suffix );
    iEvent.put ( move( goodjetinlowbv          ), prefix + "goodJetFoundInLowBVRegion" + suffix );
    //
    iEvent.put ( move( rbxcharge                ), prefix + "RBXCharge"                + suffix );
    iEvent.put ( move( rbxcharge15              ), prefix + "RBXCharge15"              + suffix );
    iEvent.put ( move( rbxenergy                ), prefix + "RBXEnergy"                + suffix );
    iEvent.put ( move( rbxenergy15              ), prefix + "RBXEnergy15"              + suffix );
    //
    iEvent.put ( move( hbherechitsevlvl         ), prefix + "HBHERecHitSevLvl"         + suffix );
    iEvent.put ( move( hbherechitenergyraw      ), prefix + "HBHERecHitEnergyRaw"      + suffix );
    iEvent.put ( move( hbherechitenergyaux      ), prefix + "HBHERecHitEnergyAux"      + suffix );
    iEvent.put ( move( hbherechitchi2           ), prefix + "HBHERecHitChi2"           + suffix );
    iEvent.put ( move( hbherechitaux            ), prefix + "HBHERecHitAux"            + suffix );
    iEvent.put ( move( hbherechitauxhbhe        ), prefix + "HBHERecHitAuxHBHE"        + suffix );
    iEvent.put ( move( hbherechitauxphase1      ), prefix + "HBHERecHitAuxPhase1"      + suffix );
    //
    iEvent.put ( move( hbherechitauxcapid       ), prefix + "HBHERecHitAuxCapID"       + suffix ); 
    iEvent.put ( move( hbherechitauxadc         ), prefix + "HBHERecHitAuxADC"         + suffix ); 
    iEvent.put ( move( hbherechitauxallfc       ), prefix + "HBHERecHitAuxAllfC"       + suffix ); 
    iEvent.put ( move( hbherechitauxpedfc       ), prefix + "HBHERecHitAuxPedFC"       + suffix );
    iEvent.put ( move( hbherechitauxgain        ), prefix + "HBHERecHitAuxGain"        + suffix );
    iEvent.put ( move( hbherechitauxrcgain      ), prefix + "HBHERecHitAuxRCGain"      + suffix );
    iEvent.put ( move( hbherechitauxfc          ), prefix + "HBHERecHitAuxFC"          + suffix );  
    iEvent.put ( move( hbherechitauxenergy      ), prefix + "HBHERecHitAuxEnergy"      + suffix );
    //
    iEvent.put ( move( hfrechitsevlvl           ), prefix + "HFRecHitSevLvl"           + suffix );
    iEvent.put ( move( hfrechitaux              ), prefix + "HFRecHitAux"              + suffix );
    iEvent.put ( move( hfrechitauxhf            ), prefix + "HFRecHitAuxHF"            + suffix );
    //
    iEvent.put ( move( nvtx                     ), prefix + "Nvtx"                     + suffix );
}


int HcalTupleMaker_HcalNoiseFilters::hcalSevLvl(const CaloRecHit* hit)
{
   HcalDetId id = hit->detid();
   if (theHcalTopology->withSpecialRBXHBHE() && id.subdet() == HcalEndcap) 
   {
     id = theHcalTopology->idFront(id);
   }

   const uint32_t recHitFlag = hit->flags();
   const uint32_t dbStatusFlag = dbHcalChStatus->getValues(id)->getValue();

   int severityLevel = hcalSevLvlComputer->getSeverityLevel(id, recHitFlag, dbStatusFlag);

   return severityLevel;

} 
