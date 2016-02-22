#ifndef HcalTupleMaker_Tree_h
#define HcalTupleMaker_Tree_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <string>
#include <vector>
#include <TTree.h>
#include <TH1I.h>

// --------------------------------------------------------------------------------------------------------
// Updated using the example here:
//    https://github.com/cms-sw/cmssw/blob/CMSSW_8_1_X/CalibTracker/SiStripCommon/plugins/ShallowTree.cc
//    https://github.com/cms-sw/cmssw/blob/CMSSW_8_1_X/CalibTracker/SiStripCommon/interface/ShallowTree.h
// --------------------------------------------------------------------------------------------------------

class HcalTupleMaker_Tree : public edm::EDAnalyzer {
 private:
  virtual void beginJob();
  //virtual void beginJob(const edm::ParameterSet&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob(){}
  
  
     template <class T> 
       void eat(edm::BranchDescription const* desc) {
       consumes<T>(edm::InputTag(desc->moduleLabel(), desc->productInstanceName()));
     }
  
     
  class BranchConnector {
   public:
    virtual ~BranchConnector() {};
    virtual void connect(const edm::Event&) = 0;
  };
  
  template <class T>
  class TypedBranchConnector : public BranchConnector {
   private:
    std::string ml;   //module label
    std::string pin;  //product instance name
    T object_;
    T* object_ptr_;
   public:
    TypedBranchConnector(edm::BranchDescription const*, std::string, TTree*);
    void connect(const edm::Event&);
  };
  
  edm::Service<TFileService> fs;
  TTree * tree;
  
  std::vector<BranchConnector*> connectors;
  //edm::ParameterSet pset;
  
 public:
  explicit HcalTupleMaker_Tree(const edm::ParameterSet& iConfig);// : pset(iConfig) {}
  
  enum LEAFTYPE {BOOL=1,  BOOL_V,
                 SHORT,   SHORT_V,           U_SHORT, U_SHORT_V,
                 INT,     INT_V,             U_INT,   U_INT_V,
                 FLOAT,   FLOAT_V,           DOUBLE,  DOUBLE_V,
                 LONG,    LONG_V,            U_LONG,  U_LONG_V,
                 STRING,  STRING_V,
		 STRING_BOOL_M, STRING_INT_M, STRING_STRING_M,
		 STRING_FLOAT_V_M,
		 FLOAT_V_V,
		 DOUBLE_V_V,
		 INT_V_V
                 };
};

#endif
