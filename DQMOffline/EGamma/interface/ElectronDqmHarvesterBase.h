
#ifndef ElectronDqmHarvesterBase_h
#define ElectronDqmHarvesterBase_h

#include "DQMOffline/EGamma/interface/ElectronDqmCommonBase.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <Rtypes.h>
#include <string>
#include <vector>

//DQM
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMEDHarvester.h"
#include "DQMServices/Core/interface/DQMStore.h"

class ElectronDqmHarvesterBase : public DQMEDHarvester, public ElectronDqmCommonBase {
public:
  typedef ElectronDqmCommonBase::DQMStore DQMStore;
  typedef ElectronDqmCommonBase::MonitorElement MonitorElement;

protected:
  explicit ElectronDqmHarvesterBase(const edm::ParameterSet &conf);
  ~ElectronDqmHarvesterBase() override;

  // specific implementation of EDAnalyzer
  void beginJob() override;  // prepare DQM, open input field if declared, and call book() below
  void dqmEndLuminosityBlock(DQMStore::IBooker &,
                             DQMStore::IGetter &,
                             edm::LuminosityBlock const &,
                             edm::EventSetup const &) override;       //performed in the endLumi
  void dqmEndJob(DQMStore::IBooker &, DQMStore::IGetter &) override;  //performed in the endJob

  // interface to implement in derived classes
  virtual void finalize(DQMStore::IBooker &iBooker,
                        DQMStore::IGetter &iGetter){};  //  override ;, const edm::Event& e, const edm::EventSetup & c

  // utility methods
  bool finalStepDone() { return finalDone_; }
  void remove(DQMStore::IBooker &iBooker, DQMStore::IGetter &iGetter, const std::string &name);

private:
  bool finalDone_;
};

#endif
