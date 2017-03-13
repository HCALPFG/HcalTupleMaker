#ifndef HCALTUPLEMAKER_RECHITALGORITHM_H
#define HCALTUPLEMAKER_RECHITALGORITHM_H

#include <iostream>

// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

// Geometry
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

// HcalHPDRBXMap
#include "RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h"

class HcalTupleMaker_HcalRecHitAlgorithm { 

 public:

  HcalTupleMaker_HcalRecHitAlgorithm();

  void run ();
  
  std::unique_ptr<std::vector<int  > > ieta;
  std::unique_ptr<std::vector<int  > > iphi;
  std::unique_ptr<std::vector<float> > eta;
  std::unique_ptr<std::vector<float> > phi;
  std::unique_ptr<std::vector<int  > > depth;
  std::unique_ptr<std::vector<int  > > rbxid;
  std::unique_ptr<std::vector<int  > > hpdid;
  std::unique_ptr<std::vector<float> > energy;
  std::unique_ptr<std::vector<float> > time;
  std::unique_ptr<std::vector<int  > > flags;
  std::unique_ptr<std::vector<int  > > aux;
  
  template <class RecoCollection > 
    void run ( const RecoCollection & recos, const CaloGeometry & geometry ){
    
    //-----------------------------------------------------
    // Get iterators
    //-----------------------------------------------------
    
    typename RecoCollection::const_iterator reco     = recos.begin();
    typename RecoCollection::const_iterator reco_end = recos.end();

    HcalDetId hcalDetId;

    //-----------------------------------------------------
    // Loop through rechits
    //-----------------------------------------------------
    //for (; reco != reco_end ; ++reco ) {
      //hcalDetId = HcalDetId(reco -> detid());
      //std::cout << "Found HcalDetId = " << hcalDetId << std::endl;
    //}
    for (; reco != reco_end ; ++reco ) {

      //-----------------------------------------------------
      // Save the detector id, no matter what
      //-----------------------------------------------------
      
      hcalDetId = HcalDetId(reco -> detid());
      //std::cout << "Looking at HcalDetId = " << hcalDetId << std::endl;

      //-----------------------------------------------------
      // Get HPD and RBX IDs
      //-----------------------------------------------------
      //   http://cmslxr.fnal.gov/lxr/source/RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h
      //   description: Algorithm which isomorphically maps HPD/RBX locations to
      //                integers ranging from 0 to NUM_HPDS-1/NUM_RBXS-1.  The HPDs/RBXs
      //                are ordered from lowest to highest: HB+, HB-, HE+, HE-.
      // "magic numbers"
      // total number of HPDs in the HB and HE: 288
      // total number of HPDs per subdetector (HB+, HB-, HE+, HE-): 72
      // number of HPDs per RBX: 4
      // total number of RBXs in the HB and HE: 72
      // total number of RBXs per subdetector (e.g. HB+, HB-, HE+, HE-): 18

      if (hcalDetId.subdet() == HcalForward) {
        rbxid->push_back(-1);
        hpdid->push_back(-1);
      } else {
        int RBXIndex = HcalHPDRBXMap::indexRBX(hcalDetId);
        int HPDIndex = HcalHPDRBXMap::indexHPD(hcalDetId);
        rbxid -> push_back ( RBXIndex );
        hpdid -> push_back ( HPDIndex );
      }

      //-----------------------------------------------------
      // Get the position
      //-----------------------------------------------------
      
      const GlobalPoint& position = geometry.getPosition(hcalDetId);
      
      //-----------------------------------------------------
      // Get rechit-specific values
      //-----------------------------------------------------
      
      eta    -> push_back ( position.eta     () );
      phi    -> push_back ( position.phi     () );
      ieta   -> push_back ( hcalDetId.ieta   () );
      iphi   -> push_back ( hcalDetId.iphi   () );
      depth  -> push_back ( hcalDetId.depth  () );
      energy -> push_back ( reco   -> energy () );
      time   -> push_back ( reco   -> time   () );
      flags  -> push_back ( reco   -> flags  () );
      aux    -> push_back ( reco   -> aux    () );
      
    } // end of loop over rechits
  }
};

#endif
