
#ifndef ElectronDqmCommonBase_h
#define ElectronDqmCommonBase_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <Rtypes.h>
#include <string>
#include <vector>

//DQM
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMEDHarvester.h"
#include "DQMServices/Core/interface/DQMStore.h"

class ElectronDqmCommonBase {
public:
  typedef dqm::legacy::DQMStore DQMStore;
  typedef dqm::legacy::MonitorElement MonitorElement;

protected:
  explicit ElectronDqmCommonBase(const edm::ParameterSet &conf);
  ~ElectronDqmCommonBase();  // override;

  // utility methods
  void setBookPrefix(const std::string &);
  void setBookIndex(short);
  void setBookEfficiencyFlag(const bool &);
  void setBookStatOverflowFlag(const bool &);
  std::string newName(const std::string &name);
  int verbosity() { return verbosity_; }

  //bool histoNamesReady;
  std::string finalStep_;
  std::string inputFile_;
  std::string outputFile_;
  std::string inputInternalPath_;
  std::string outputInternalPath_;

  MonitorElement *get(DQMStore::IGetter &iGetter, const std::string &name);
  //const std::string *find(DQMStore::IGetter &iGetter, const std::string &name);

  // Common features for ElectronDqmAnalyzerBase/ElectronDqmHarvesterBase
  MonitorElement *bookH1(DQMStore::IBooker &,
                         const std::string &name,
                         const std::string &title,
                         int nchX,
                         double lowX,
                         double highX,
                         const std::string &titleX = "",
                         const std::string &titleY = "Events",
                         Option_t *option = "E1 P"); /**/

  MonitorElement *bookH1withSumw2(DQMStore::IBooker &,
                                  const std::string &name,
                                  const std::string &title,
                                  int nchX,
                                  double lowX,
                                  double highX,
                                  const std::string &titleX = "",
                                  const std::string &titleY = "Events",
                                  Option_t *option = "E1 P");

  MonitorElement *bookH2(DQMStore::IBooker &,
                         const std::string &name,
                         const std::string &title,
                         int nchX,
                         double lowX,
                         double highX,
                         int nchY,
                         double lowY,
                         double highY,
                         const std::string &titleX = "",
                         const std::string &titleY = "",
                         Option_t *option = "COLZ");

  MonitorElement *bookH2withSumw2(DQMStore::IBooker &,
                                  const std::string &name,
                                  const std::string &title,
                                  int nchX,
                                  double lowX,
                                  double highX,
                                  int nchY,
                                  double lowY,
                                  double highY,
                                  const std::string &titleX = "",
                                  const std::string &titleY = "",
                                  Option_t *option = "COLZ");

  MonitorElement *bookP1(DQMStore::IBooker &,
                         const std::string &name,
                         const std::string &title,
                         int nchX,
                         double lowX,
                         double highX,
                         double lowY,
                         double highY,
                         const std::string &titleX = "",
                         const std::string &titleY = "",
                         Option_t *option = "E1 P");

  // Features for ElectronDqmHarvesterBase
  MonitorElement *bookH1andDivide(DQMStore::IBooker &iBooker,
                                  DQMStore::IGetter &,
                                  const std::string &name,
                                  MonitorElement *num,
                                  MonitorElement *denom,
                                  const std::string &titleX,
                                  const std::string &titleY,
                                  const std::string &title = "");

  MonitorElement *bookH1andDivide(DQMStore::IBooker &iBooker,
                                  DQMStore::IGetter &iGetter,
                                  const std::string &name,
                                  const std::string &num,
                                  const std::string &denom,
                                  const std::string &titleX,
                                  const std::string &titleY,
                                  const std::string &title = "");

  MonitorElement *bookH2andDivide(DQMStore::IBooker &iBooker,
                                  DQMStore::IGetter &,
                                  const std::string &name,
                                  MonitorElement *num,
                                  MonitorElement *denom,
                                  const std::string &titleX,
                                  const std::string &titleY,
                                  const std::string &title = "");

  MonitorElement *bookH2andDivide(DQMStore::IBooker &iBooker,
                                  DQMStore::IGetter &,
                                  const std::string &name,
                                  const std::string &num,
                                  const std::string &denom,
                                  const std::string &titleX,
                                  const std::string &titleY,
                                  const std::string &title = "");

  MonitorElement *profileX(DQMStore::IBooker &iBooker,
                           DQMStore::IGetter &iGetter,
                           MonitorElement *me2d,
                           const std::string &title = "",
                           const std::string &titleX = "",
                           const std::string &titleY = "",
                           Double_t minimum = -1111,
                           Double_t maximum = -1111);

  MonitorElement *profileX(DQMStore::IBooker &iBooker,
                           DQMStore::IGetter &iGetter,
                           const std::string &me2d,
                           const std::string &title = "",
                           const std::string &titleX = "",
                           const std::string &titleY = "",
                           Double_t minimum = -1111,
                           Double_t maximum = -1111);

  MonitorElement *profileY(DQMStore::IBooker &iBooker,
                           DQMStore::IGetter &iGetter,
                           MonitorElement *me2d,
                           const std::string &title = "",
                           const std::string &titleX = "",
                           const std::string &titleY = "",
                           Double_t minimum = -1111,
                           Double_t maximum = -1111);

  MonitorElement *profileY(DQMStore::IBooker &iBooker,
                           DQMStore::IGetter &iGetter,
                           const std::string &me2d,
                           const std::string &title = "",
                           const std::string &titleX = "",
                           const std::string &titleY = "",
                           Double_t minimum = -1111,
                           Double_t maximum = -1111);

  MonitorElement *cloneH1(DQMStore::IBooker &iBooker,
                          DQMStore::IGetter &,
                          const std::string &name,
                          MonitorElement *original,
                          const std::string &title = "");

  MonitorElement *cloneH1(DQMStore::IBooker &iBooker,
                          DQMStore::IGetter &,
                          const std::string &name,
                          const std::string &original,
                          const std::string &title = "");

private:
  int verbosity_;
  std::string bookPrefix_;
  short bookIndex_;
  bool bookEfficiencyFlag_ = false;
  bool bookStatOverflowFlag_ = false;
  bool histoNamesReady;
  std::vector<std::string> histoNames_;

  const std::string *find(DQMStore::IGetter &iGetter, const std::string &name);
};

#endif
