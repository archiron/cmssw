
#ifndef ElectronDqmAnalyzerBase_h
#define ElectronDqmAnalyzerBase_h

#include "DQMOffline/EGamma/interface/ElectronDqmCommonBase.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <Rtypes.h>
#include <string>
#include <vector>

//DQM
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"

namespace edab {
  struct Empty {};
}  // namespace edab
class ElectronDqmAnalyzerBase : public DQMEDAnalyzer, public ElectronDqmCommonBase {
public:
  typedef ElectronDqmCommonBase::DQMStore DQMStore;
  typedef ElectronDqmCommonBase::MonitorElement MonitorElement;

protected:
  explicit ElectronDqmAnalyzerBase(const edm::ParameterSet &conf);
  ~ElectronDqmAnalyzerBase() override;

  // specific implementation of EDAnalyzer
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;

  // interface to implement in derived classes
  void analyze(const edm::Event &e, const edm::EventSetup &c) override {}

  // utility methods

private:
};

#endif
