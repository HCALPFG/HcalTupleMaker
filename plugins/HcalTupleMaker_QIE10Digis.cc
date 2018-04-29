#include <iostream>
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_QIE10Digis.h"
//#include "DataFormats/HcalDigi/interface/HcalLaserDigi.h"
//#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"

#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"


// NEEDS UPDATING
double adc2fC_QIE10[256]={
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

HcalTupleMaker_QIE10Digis::HcalTupleMaker_QIE10Digis(const edm::ParameterSet& iConfig):
  prefix          (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix          (iConfig.getUntrackedParameter<std::string>("Suffix")),
  m_qie10DigisTag (iConfig.getUntrackedParameter<edm::InputTag>("tagQIE10", edm::InputTag("hcalDigis")))
{ 

  qie10digisToken_ = consumes<QIE10DigiCollection>(m_qie10DigisTag);
    
  produces<std::vector<int>   >                  ( "QIE10DigiIEta"      );
  produces<std::vector<int>   >                  ( "QIE10DigiIPhi"      );
  produces<std::vector<int>   >                  ( "QIE10DigiSubdet"    );
  produces<std::vector<int>   >                  ( "QIE10DigiDepth"     );
  produces<std::vector<int>   >                  ( "QIE10DigiRawID"     );
  produces<std::vector<int>   >                  ( "QIE10DigiLinkError" );
  produces<std::vector<int>   >                  ( "QIE10DigiFlags"     );
  produces<std::vector<std::vector<int>   > >    ( "QIE10DigiSOI"       );
  produces<std::vector<std::vector<int>   > >    ( "QIE10DigiOK"        );
  produces<std::vector<std::vector<int>   > >    ( "QIE10DigiADC"       );
  produces<std::vector<std::vector<double>   > > ( "QIE10DigiFC"        );
  produces<std::vector<std::vector<int>   > >    ( "QIE10DigiLETDC"     );
  produces<std::vector<std::vector<int>   > >    ( "QIE10DigiTETDC"     );
  produces<std::vector<std::vector<int>   > >    ( "QIE10DigiCapID"     );
}

void HcalTupleMaker_QIE10Digis::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    
  std::unique_ptr<std::vector<int> >                    ieta   ( new std::vector<int>   ());
  std::unique_ptr<std::vector<int> >                    iphi   ( new std::vector<int>   ());
  std::unique_ptr<std::vector<int> >                    subdet ( new std::vector<int>   ());
  std::unique_ptr<std::vector<int> >                    depth  ( new std::vector<int>   ());
  std::unique_ptr<std::vector<int> >                    rawId  ( new std::vector<int>   ());
  std::unique_ptr<std::vector<int> >                    linkEr ( new std::vector<int>   ());
  std::unique_ptr<std::vector<int> >                    flags  ( new std::vector<int>   ());
  std::unique_ptr<std::vector<std::vector<int  > > >    soi    ( new std::vector<std::vector<int  > >   ());
  std::unique_ptr<std::vector<std::vector<int  > > >    ok     ( new std::vector<std::vector<int  > >   ());
  std::unique_ptr<std::vector<std::vector<int  > > >    adc    ( new std::vector<std::vector<int  > >    ());
  std::unique_ptr<std::vector<std::vector<double  > > > fc     ( new std::vector<std::vector<double  > > ());
  std::unique_ptr<std::vector<std::vector<int  > > >    le_tdc ( new std::vector<std::vector<int  > >    ());
  std::unique_ptr<std::vector<std::vector<int  > > >    te_tdc ( new std::vector<std::vector<int  > >    ());
  std::unique_ptr<std::vector<std::vector<int  > > >    capid  ( new std::vector<std::vector<int  > >    ());
    
  //

  edm::ESHandle<HcalDbService> conditions;
  iSetup.get<HcalDbRecord>().get(conditions);

  edm::Handle<QIE10DigiCollection>  qie10Digis;
  iEvent.getByToken(qie10digisToken_, qie10Digis);
   
  //
  for (uint32_t i=0; i<qie10Digis->size(); i++){

    // From: https://github.com/awhitbeck/HFcommissioningAnalysis/blob/b3456c9fe66ef9bcc6c54773d60f768c269a5c74/src/HFanalyzer.cc#L429
    QIE10DataFrame qie10df = static_cast<QIE10DataFrame>((*qie10Digis)[i]);

    //Extract info on detector location
    DetId detid = qie10df.detid();
    HcalDetId hcaldetid = HcalDetId(detid);

    // Protection against calibration channels which are not
    // in the database but can still come in the QIE10DataFrame
    // in the laser calibs, etc.
    if (hcaldetid.subdet() != HcalSubdetector::HcalForward)
      continue;        
    

    const HcalQIECoder* channelCoder = conditions -> getHcalCoder(hcaldetid);
    const HcalQIEShape* shape = conditions -> getHcalShape(channelCoder);
    HcalCoderDb coder(*channelCoder,*shape);
    CaloSamples cs; coder.adc2fC(qie10df,cs);
        
    ieta   -> push_back ( hcaldetid.ieta()        );
    iphi   -> push_back ( hcaldetid.iphi()        );
    subdet -> push_back ( 5/*hcaldetid.subdet()*/ );
    depth  -> push_back ( hcaldetid.depth()       );
    rawId  -> push_back ( hcaldetid.rawId()       );
    linkEr -> push_back ( qie10df.linkError()     );
    flags  -> push_back ( qie10df.flags()         );
    
    if (0){
      std::cout << "Printing raw dataframe" << std::endl;
      std::cout << qie10df << std::endl;

      std::cout << "Printing content of samples() method" << std::endl;
      std::cout << qie10df.samples() << std::endl;
    }

    soi             -> push_back ( std::vector<int  >   () ) ;
    ok              -> push_back ( std::vector<int  >   () ) ;
    adc             -> push_back ( std::vector<int  >   () ) ;
    fc              -> push_back ( std::vector<double  >() ) ;
    le_tdc          -> push_back ( std::vector<int  >   () ) ;
    te_tdc          -> push_back ( std::vector<int  >   () ) ;
    capid           -> push_back ( std::vector<int  >   () ) ;
    size_t last_entry = adc -> size() - 1;

    // TS
    int nTS = qie10df.samples();

    for(int its=0; its<nTS; ++its)
    { 
      //int adc = qie10df[its].adc();
      //int capid = qie10df[its].capid();

      (*soi      )[last_entry].push_back ( qie10df[its].soi()               ); // soi is a bool, but stored as an int
      (*ok       )[last_entry].push_back ( qie10df[its].ok()                ); // ok is a bool, but stored as an int
      (*adc      )[last_entry].push_back ( qie10df[its].adc()               );
      //(*fc       )[last_entry].push_back ( qie10df[its].nominal_fC() );
      (*fc       )[last_entry].push_back ( cs[its] );
      (*le_tdc   )[last_entry].push_back ( qie10df[its].le_tdc()            );
      (*te_tdc   )[last_entry].push_back ( qie10df[its].te_tdc()            );
      (*capid    )[last_entry].push_back ( qie10df[its].capid()             );
	  
    }

  }

  //  
  iEvent.put(move( ieta  )        , "QIE10DigiIEta"      );
  iEvent.put(move( iphi  )        , "QIE10DigiIPhi"      );
  iEvent.put(move( subdet)        , "QIE10DigiSubdet"    );
  iEvent.put(move( depth )        , "QIE10DigiDepth"     );
  iEvent.put(move( rawId )        , "QIE10DigiRawID"     );
  iEvent.put(move( linkEr)        , "QIE10DigiLinkError" );
  iEvent.put(move( flags )        , "QIE10DigiFlags"     );
  iEvent.put(move( soi   )        , "QIE10DigiSOI"       );
  iEvent.put(move( ok    )        , "QIE10DigiOK"        );
  iEvent.put(move( adc   )        , "QIE10DigiADC"       );
  iEvent.put(move( fc    )        , "QIE10DigiFC"        );
  iEvent.put(move( le_tdc)        , "QIE10DigiLETDC"     );
  iEvent.put(move( te_tdc)        , "QIE10DigiTETDC"     );
  iEvent.put(move( capid )        , "QIE10DigiCapID"     );

}
