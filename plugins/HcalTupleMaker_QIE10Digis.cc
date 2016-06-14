#include <iostream>
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_QIE10Digis.h"
//#include "DataFormats/HcalDigi/interface/HcalLaserDigi.h"
//#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"



HcalTupleMaker_QIE10Digis::HcalTupleMaker_QIE10Digis(const edm::ParameterSet& iConfig):
  prefix     (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix     (iConfig.getUntrackedParameter<std::string>("Suffix")),
  m_qie10DigisTag (iConfig.getUntrackedParameter<edm::InputTag>("tagQIE10", edm::InputTag("hcalDigis")))
{ 

    qie10digisToken_     = consumes<QIE10DigiCollection>(m_qie10DigisTag);
    
    produces<std::vector<int>   >               ( "QIE10DigiIEta"    );
    produces<std::vector<int>   >               ( "QIE10DigiIPhi"    );
    produces<std::vector<int>   >               ( "QIE10DigiSubdet"  );
    produces<std::vector<int>   >               ( "QIE10DigiDepth"   );
    produces<std::vector<int>   >               ( "QIE10DigiRawID"   );
    produces<std::vector<std::vector<int>   > > ( "QIE10DigiADC"     );
    produces<std::vector<std::vector<int>   > > ( "QIE10DigiCapID"   );
}

void HcalTupleMaker_QIE10Digis::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
    
    std::auto_ptr<std::vector<int> >                    ieta   ( new std::vector<int>   ());
    std::auto_ptr<std::vector<int> >                    iphi   ( new std::vector<int>   ());
    std::auto_ptr<std::vector<int> >                    subdet ( new std::vector<int>   ());
    std::auto_ptr<std::vector<int> >                    depth  ( new std::vector<int>   ());
    std::auto_ptr<std::vector<int> >                    rawId  ( new std::vector<int>   ());
    std::auto_ptr<std::vector<std::vector<int  > > >    adc    ( new std::vector<std::vector<int  > > ());
    std::auto_ptr<std::vector<std::vector<int  > > >    capid  ( new std::vector<std::vector<int  > > ());
    
    //
    edm::Handle<QIE10DigiCollection>  qie10Digis;
    iEvent.getByToken(qie10digisToken_, qie10Digis);
    
    //
    for (uint32_t i=0; i<qie10Digis->size(); i++){

        // From: https://github.com/awhitbeck/HFcommissioningAnalysis/blob/b3456c9fe66ef9bcc6c54773d60f768c269a5c74/src/HFanalyzer.cc#L429
        QIE10DataFrame qie10df = static_cast<QIE10DataFrame>((*qie10Digis)[i]);

        //Extract info on detector location
        DetId detid = qie10df.detid();
        HcalDetId hcaldetid = HcalDetId(detid);
        
        ieta -> push_back ( hcaldetid.ieta());
        iphi -> push_back ( hcaldetid.iphi());
        subdet -> push_back ( 5/*hcaldetid.subdet()*/);
        depth -> push_back ( hcaldetid.depth());
        rawId -> push_back ( hcaldetid.rawId());

        if (0){
            std::cout << "Printing raw dataframe" << std::endl;
            std::cout << qie10df << std::endl;

            std::cout << "Printing content of samples() method" << std::endl;
            std::cout << qie10df.samples() << std::endl;
        }
    
        adc             -> push_back ( std::vector<int  >() ) ;
        capid           -> push_back ( std::vector<int  >() ) ;
        size_t last_entry = adc -> size() - 1;

        // TS
        int nTS = qie10df.samples();

        for(int its=0; its<nTS; ++its)
        { 
            //int adc = qie10df[its].adc();
            //int capid = qie10df[its].capid();
       
            (*adc      )[last_entry].push_back (       qie10df[its].adc()        ); 
            (*capid      )[last_entry].push_back (     qie10df[its].capid()        ); 

        }

    }

    //  
    iEvent.put( ieta          , "QIE10DigiIEta"     ); 
    iEvent.put( iphi          , "QIE10DigiIPhi"     ); 
    iEvent.put( subdet        , "QIE10DigiSubdet"   ); 
    iEvent.put( depth         , "QIE10DigiDepth"    ); 
    iEvent.put( rawId         , "QIE10DigiRawID"    ); 
    iEvent.put( adc           , "QIE10DigiADC"      ); 
    iEvent.put( capid         , "QIE10DigiCapID"    ); 

}
