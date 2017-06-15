#ifndef HCALTUPLEMAKER_HFPHASE1HITALGORITHM_H
#define HCALTUPLEMAKER_HFPHASE1HITALGORITHM_H

#include <iostream>

// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

// Geometry
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

// HcalHPDRBXMap
#include "RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h"
#include "DataFormats/HcalRecHit/interface/HFQIE10Info.h"

class HcalTupleMaker_HcalPhase1RecHitAlgorithm { 

 public:

  HcalTupleMaker_HcalPhase1RecHitAlgorithm();

  void run ();
  
  std::unique_ptr<std::vector<int  > > ieta;           
  std::unique_ptr<std::vector<int  > > iphi;           
  std::unique_ptr<std::vector<float> > eta;           
  std::unique_ptr<std::vector<float> > phi;           
  std::unique_ptr<std::vector<int  > > depth;          
  std::unique_ptr<std::vector<float> > energy;    
  std::unique_ptr<std::vector<float> > charge;    
 
  std::unique_ptr<std::vector<std::vector<float> > > qie10time;    
  std::unique_ptr<std::vector<std::vector<float> > > qie10energy;    
  std::unique_ptr<std::vector<std::vector<float> > > qie10charge;           
  std::unique_ptr<std::vector<std::vector<int  > > > qie10soi;     
  
  float qie10timeVal[2];
  float qie10chargeVal[2];
  float qie10energyVal[2];
  int   qie10soiVal[2];

  template <class RecoCollection > 
    void run ( const RecoCollection & recos, const CaloGeometry & geometry, double energyThresholdHF ){
    
    //-----------------------------------------------------
    // Get iterators
    //-----------------------------------------------------
    
    typename RecoCollection::const_iterator reco     = recos.begin();
    typename RecoCollection::const_iterator reco_end = recos.end();

    HcalDetId hcalDetId;
    const HFQIE10Info *info;
    
    //-----------------------------------------------------
    // Loop through rechits
    //-----------------------------------------------------
    
    //std::cout << "  recos.size() = " << recos.size() << std::endl;

    for ( ; reco != reco_end ; ++reco ) {

      for (unsigned int i = 0; i < 2; i++) {
        qie10timeVal[i] = 0;   
        qie10chargeVal[i] = 0; 
        qie10energyVal[i] = 0; 
        qie10soiVal[i] = 0;    
      }

      //-----------------------------------------------------
      // "Custom Zero Suppression" - To suppress rechits coming from HF 
      //-----------------------------------------------------
      
      if( reco->energy() < energyThresholdHF ) continue;

      //-----------------------------------------------------
      // Save the detector id, no matter what
      //-----------------------------------------------------
      
      hcalDetId = HcalDetId(reco -> id());

      //-----------------------------------------------------
      // Get the position
      //-----------------------------------------------------
      
      const GlobalPoint& position = geometry.getPosition(hcalDetId);
      
      //-----------------------------------------------------
      // Get rechit-specific values
      //-----------------------------------------------------
 
      //std::cout << " HFPreRecHit:  ieta = " << hcalDetId.ieta() << "  iphi = " << hcalDetId.iphi() 
      //          << "  depth = " << hcalDetId.depth() << "  charge = " << reco->charge () 
      //          << "  energy = " << reco->energy() << std::endl;

      eta    -> push_back ( position.eta     () );
      phi    -> push_back ( position.phi     () );
      ieta   -> push_back ( hcalDetId.ieta   () );
      iphi   -> push_back ( hcalDetId.iphi   () );
      depth  -> push_back ( hcalDetId.depth  () );
      energy -> push_back ( reco   -> energy () );
      charge -> push_back ( reco   -> charge () );

      for (unsigned int i = 0; i < 2; i++) {


        info = reco->getHFQIE10Info(i);
        if (!info) continue;
        qie10timeVal[i] = info->timeRising();
        qie10chargeVal[i] = info->charge();
        qie10energyVal[i] = info->energy();
        qie10soiVal[i] = info->soi();
      
        //std::cout << "(" << hcalDetId.ieta() 
        //          << ", " << hcalDetId.ieta() 
        //          << ", " << hcalDetId.depth() << ") :: " 
        //          << " Anode" << i << ":  time = " << qie10timeVal[i] << "  charge = " 
        //          << qie10chargeVal[i] << "  energy = " << qie10energyVal[i] << std::endl;
      }

      std::vector<float> qie10timevector(std::begin(qie10timeVal), std::end(qie10timeVal));
      std::vector<float> qie10chargevector(std::begin(qie10chargeVal), std::end(qie10chargeVal));
      std::vector<float> qie10energyvector(std::begin(qie10energyVal), std::end(qie10energyVal));
      std::vector<int> qie10soivector(std::begin(qie10soiVal), std::end(qie10soiVal));

      qie10time -> push_back(qie10timevector);
      qie10charge -> push_back(qie10chargevector);
      qie10energy -> push_back(qie10energyvector);
      qie10soi -> push_back(qie10soivector);
      
    } // end of loop over rechits
  }
};

#endif
