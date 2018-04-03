#include <iostream>
#include <memory>
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Lasermon.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"

// NEEDS UPDATING
double adc2fC_hf[256]={
  // - - - - - - - range 0 - - - - - - - -
  //subrange0
  1.58, 4.73, 7.88, 11.0, 14.2, 17.3, 20.5, 23.6,
  26.8, 29.9, 33.1, 36.2, 39.4, 42.5, 45.7, 48.8,
  //subrange1
  53.6, 60.1, 66.6, 73.0, 79.5, 86.0, 92.5, 98.9,
  105, 112, 118, 125, 131, 138, 144, 151,
  //subrange2
  157, 164, 170, 177, 186, 199, 212, 225,
  238, 251, 264, 277, 289, 302, 315, 328,
  //subrange3
  341, 354, 367, 380, 393, 406, 418, 431,
  444, 464, 490, 516, 542, 568, 594, 620,

  // - - - - - - - range 1 - - - - - - - -
  //subrange0
  569, 594, 619, 645, 670, 695, 720, 745,
  771, 796, 821, 846, 871, 897, 922, 947,
  //subrange1
  960, 1010, 1060, 1120, 1170, 1220, 1270, 1320,
  1370, 1430, 1480, 1530, 1580, 1630, 1690, 1740,
  //subrange2
  1790, 1840, 1890, 1940,  2020, 2120, 2230, 2330,
  2430, 2540, 2640, 2740, 2850, 2950, 3050, 3150,
  //subrange3
  3260, 3360, 3460, 3570, 3670, 3770, 3880, 3980,
  4080, 4240, 4450, 4650, 4860, 5070, 5280, 5490,

  // - - - - - - - range 2 - - - - - - - -
  //subrange0
  5080, 5280, 5480, 5680, 5880, 6080, 6280, 6480,
  6680, 6890, 7090, 7290, 7490, 7690, 7890, 8090,
  //subrange1
  8400, 8810, 9220, 9630, 10000, 10400, 10900, 11300,
  11700, 12100, 12500, 12900, 13300, 13700, 14100, 14500,
  //subrange2
  15000, 15400, 15800, 16200, 16800, 17600, 18400, 19300,
  20100, 20900, 21700, 22500, 23400, 24200, 25000, 25800,
  //subrange3
  26600, 27500, 28300, 29100, 29900, 30700, 31600, 32400,
  33200, 34400, 36100, 37700, 39400, 41000, 42700, 44300,

  // - - - - - - - range 3 - - - - - - - - -
  //subrange0
  41100, 42700, 44300, 45900, 47600, 49200, 50800, 52500,
  54100, 55700, 57400, 59000, 60600, 62200, 63900, 65500,
  //subrange1
  68000, 71300, 74700, 78000, 81400, 84700, 88000, 91400,
  94700, 98100, 101000, 105000, 108000, 111000, 115000, 118000,
  //subrange2
  121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
  168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
  //subrange3
  230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
  291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000

};

HcalTupleMaker_Lasermon::HcalTupleMaker_Lasermon(const edm::ParameterSet& iConfig):
  m_lasermonDigisTag (iConfig.getUntrackedParameter<edm::InputTag>("tagLasermon", edm::InputTag("hcalDigis")))
{ 

  lasermonDigisToken_ = consumes<QIE10DigiCollection>(m_lasermonDigisTag);

  laserMonCBoxList_ = iConfig.getUntrackedParameter<std::vector<int> >("laserMonCBoxList");
  laserMonIPhiList_ = iConfig.getUntrackedParameter<std::vector<int> >("laserMonIPhiList");
  laserMonIEtaList_ = iConfig.getUntrackedParameter<std::vector<int> >("laserMonIEtaList");

  laserMonitorTSStart_ = 0;
  laserMonitorTSEnd_ = -1;
    
  produces<std::vector<int>    >    ( "LasermonDigiADC"       );
  produces<std::vector<double>    > ( "LasermonDigiFC"        );
}

void HcalTupleMaker_Lasermon::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    
  std::unique_ptr<std::vector<int  >  >    lasmon_adc    ( new std::vector<int  >     ());
  std::unique_ptr<std::vector<double   > > lasmon_fc     ( new std::vector<double   > ());
    
  //
  edm::Handle<QIE10DigiCollection>  lasermonDigis;
  iEvent.getByToken(lasermonDigisToken_, lasermonDigis);
    
  //
  std::vector<std::vector<int> > lasmon_adcs(laserMonCBoxList_.size(), std::vector<int>());
  std::vector<std::vector<int> > lasmon_capids(laserMonCBoxList_.size(), std::vector<int>());
  unsigned max_nsamples = 0;
  for(QIE10DigiCollection::const_iterator digi = lasermonDigis->begin(); digi != lasermonDigis->end(); digi++) {

    QIE10DataFrame df = static_cast<QIE10DataFrame>(*digi);

    HcalCalibDetId calibId( digi->id() );

    // Fill the lasermonitor channels
    int cboxch  = calibId.cboxChannel( );
    int iphi    = calibId.iphi();
    int ieta    = calibId.ieta();

    // find the index of this channel by matching cBox, iEta, iPhi
    for( unsigned idx = 0; idx < laserMonCBoxList_.size(); ++idx ) {
      if( cboxch == laserMonCBoxList_[idx] &&
        iphi  == laserMonIPhiList_[idx] && 
        ieta  == laserMonIEtaList_[idx] ) {

        // now get the digis
        unsigned ts_size = df.samples();
        if( ts_size > max_nsamples ) max_nsamples = ts_size;
        for(unsigned i = 0; i < ts_size; i++) {
          bool ok = df[i].ok();
          if( !ok ) { // protection against QIE reset
            lasmon_adcs[idx].push_back( -1 );
            lasmon_capids[idx].push_back( -1 );
          } else {
            lasmon_adcs[idx].push_back( df[i].adc() );
            lasmon_capids[idx].push_back( df[i].capid() );
          }
        } // end digi loop
      } // end matching channel if
    } // end fiber order loop
  }// end loop over digis
 
  // now match the laser monitor data by fiber (in time) 
  // check for any fibers without data and fill
  // them so we dont run into problems later
  for( unsigned idx = 0; idx < laserMonCBoxList_.size(); ++idx ) {
      if( lasmon_adcs[idx].empty() ) {
          lasmon_adcs[idx] = std::vector<int>(max_nsamples, -1);
      }
      if( lasmon_capids[idx].empty() ) {
          lasmon_capids[idx] = std::vector<int>(max_nsamples, -1);
      }
  }
  unsigned nFibers = laserMonIEtaList_.size();
  // for each fiber we need to find the index at with the 
  // data from the next fiber matches in order to stitch them together.
  // When there is an overlap, the data from the end of the
  // earlier fiber is removed.  There is no removal of the last fiber
  std::vector<unsigned> matching_idx; 
  // we assume that the list of fibers was given in time order
  // (if this was not the case, then we just end up using 
  // all data from all fibers )
  for( unsigned fidx = 0; nFibers > 0 && (fidx < (nFibers - 1)); ++fidx ) {

    unsigned nts = lasmon_capids[fidx].size();  // number of time slices

    // start by checking just the last TS of the earlier fiber
    // against the first TS of the later fiber
    // on each iteration, check one additional TS
    // moving back in time on the earlier fiber and
    // forward in time in the later fiber
    
    int start_ts = nts - 1; // start_ts will be decrimented on each loop where a match is not found

    // in the case that our stringent check below doesn't work 
    // store the latest capID that has a match
    int latest_cap_match = -1;

    // loop over the number of checks to make
    for( unsigned ncheck = 1; ncheck <= nts ; ncheck++ ) {
      bool cap_match = true; //will be set to false if at least one check fails below
      bool adc_match = true; //will be set to false if at least one check fails below

      // loop over the channel TS, this is for the later fiber in time
      for( unsigned lidx = 0; lidx < ncheck; lidx++) {
        // we are looping over the TS of the later fiber in time
        // the TS of the earlier fiber starts from the end
        unsigned eidx = nts-ncheck+lidx;
        // if we get an invald value, this fiber has no data
        // the check and match will fail, so the start_ts will 
        // be decrimented
        if( lasmon_capids[fidx][eidx] == -1 || lasmon_capids[fidx+1][lidx] == -1 ) {
          cap_match = false;
          adc_match = false;
          break;
        }

        if( lasmon_capids[fidx][eidx] != lasmon_capids[fidx+1][lidx] ) {
          cap_match = false;
        }
        // check the data values as well
        if( lasmon_adcs[fidx][eidx] != lasmon_adcs[fidx+1][lidx] ) {
          adc_match = false;
        }
      }
      if( cap_match && (start_ts > latest_cap_match) ) {
        latest_cap_match = start_ts;
      }
      if( cap_match && adc_match ) {
        // end the loop and we'll take the current start_ts
        // as the end of the data for this fiber
        break;
      }
      else {
        // if we don't have a match, then decrement the 
        // starting TS and check again
        start_ts--;
      }
    }

    // now make some sanity checks on the determined overlap index
    if( start_ts == -1 ) {
      // if we didn't find any match, use the capID only to compare
      if( latest_cap_match < 0 ) {
        //this shouldn't happen, in this case use all the data from the fiber
        start_ts = nts;
      }
      else {
        // its possible that the timing of the fibers
        // is shifted such that they do not overlap
        // and we just want to stitch the fibers
        // together with no removal.
        // In this case the capIDs will match at the
        // N-4 spot (and the ADCs will not)
        // if this is not the case, then we just take
        // the value of latest match
        if( latest_cap_match == int(nts - 4) ) {
          start_ts = nts;
        } else {
          start_ts = latest_cap_match;
        }
      }
    }

    // now store as the matching index
    matching_idx.push_back(start_ts);
  }

  // for the last fiber we always use all of the data
  matching_idx.push_back(max_nsamples);

  // now loop over the time slices of each fiber and make the sum
  int icombts = -1;
  for( unsigned fidx = 0 ; fidx < nFibers; ++fidx ) {
    for( unsigned its = 0; its < matching_idx[fidx]; ++its ) {
      icombts++;

      // apply integration limits
      if( icombts < laserMonitorTSStart_ ) continue;
      if( laserMonitorTSEnd_ > 0 && icombts > laserMonitorTSEnd_ ) continue;

      int adc = lasmon_adcs[fidx][its];

      if( adc >= 0 ) { // skip invalid data
        float fc = adc2fC_hf[adc];

        lasmon_adc->push_back( adc );
        lasmon_fc -> push_back( fc );
        
      }
    } 
  }
  iEvent.put(move( lasmon_adc   )        , "LasermonDigiADC"       );
  iEvent.put(move( lasmon_fc    )        , "LasermonDigiFC"        );

}
