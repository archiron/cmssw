
#include "DQMOffline/EGamma/interface/ElectronDqmCommonBase.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TTree.h"
#include <iostream>
#include <algorithm>
#include <sstream>

ElectronDqmCommonBase::ElectronDqmCommonBase(const edm::ParameterSet &conf)
    : bookPrefix_("ele"), bookIndex_(0), histoNamesReady(false) {
  verbosity_ = conf.getUntrackedParameter<int>("Verbosity");
  finalStep_ = conf.getParameter<std::string>("FinalStep");
  inputFile_ = conf.getParameter<std::string>("InputFile");
  outputFile_ = conf.getParameter<std::string>("OutputFile");
  inputInternalPath_ = conf.getParameter<std::string>("InputFolderName");
  outputInternalPath_ = conf.getParameter<std::string>("OutputFolderName");
}

ElectronDqmCommonBase::~ElectronDqmCommonBase() {}

void ElectronDqmCommonBase::setBookPrefix(const std::string &prefix) { bookPrefix_ = prefix; }
void ElectronDqmCommonBase::setBookIndex(short index) { bookIndex_ = index; }
void ElectronDqmCommonBase::setBookEfficiencyFlag(const bool &eff_flag) { bookEfficiencyFlag_ = eff_flag; }
void ElectronDqmCommonBase::setBookStatOverflowFlag(const bool &statOverflow_flag) {
  bookStatOverflowFlag_ = statOverflow_flag;
}

// Common features for ElectronDqmAnalyzerBase/ElectronDqmHarvesterBase
ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::bookH1(DQMStore::IBooker &iBooker,
                                                                         const std::string &name,
                                                                         const std::string &title,
                                                                         int nchX,
                                                                         double lowX,
                                                                         double highX,
                                                                         const std::string &titleX,
                                                                         const std::string &titleY,
                                                                         Option_t *option) {
  iBooker.setCurrentFolder(outputInternalPath_);
  MonitorElement *me = iBooker.book1D(newName(name), title, nchX, lowX, highX);
  if (!titleX.empty()) {
    me->setAxisTitle(titleX);
  }
  if (!titleY.empty()) {
    me->setAxisTitle(titleY, 2);
    //me->getTH1F()->GetYaxis()->SetTitle(titleY.c_str());
  }
  if (TString(option) != "") {
    me->setOption(option);
  }
  if (bookStatOverflowFlag_) {
    me->setStatOverflows(kTRUE);
  }
  return me;
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::bookH1withSumw2(DQMStore::IBooker &iBooker,
                                                                                  const std::string &name,
                                                                                  const std::string &title,
                                                                                  int nchX,
                                                                                  double lowX,
                                                                                  double highX,
                                                                                  const std::string &titleX,
                                                                                  const std::string &titleY,
                                                                                  Option_t *option) {
  iBooker.setCurrentFolder(outputInternalPath_);
  MonitorElement *me = iBooker.book1D(newName(name), title, nchX, lowX, highX);
  if (me->getTH1F()->GetSumw2N() == 0)
    me->enableSumw2();
  if (!titleX.empty()) {
    me->setAxisTitle(titleX);
  }
  if (!titleY.empty()) {
    me->setAxisTitle(titleY, 2);
    //me->getTH1F()->GetYaxis()->SetTitle(titleY.c_str());
  }
  if (TString(option) != "") {
    me->setOption(option);
  }
  if (bookStatOverflowFlag_) {
    me->setStatOverflows(kTRUE);
  }
  return me;
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::bookH2(DQMStore::IBooker &iBooker,
                                                                           const std::string &name,
                                                                           const std::string &title,
                                                                           int nchX,
                                                                           double lowX,
                                                                           double highX,
                                                                           int nchY,
                                                                           double lowY,
                                                                           double highY,
                                                                           const std::string &titleX,
                                                                           const std::string &titleY,
                                                                           Option_t *option) {
  iBooker.setCurrentFolder(outputInternalPath_);
  MonitorElement *me = iBooker.book2D(newName(name), title, nchX, lowX, highX, nchY, lowY, highY);
  if (!titleX.empty()) {
    me->setAxisTitle(titleX);
  }
  if (!titleY.empty()) {
    me->setAxisTitle(titleY, 2);
    //me->getTH2F()->GetYaxis()->SetTitle(titleY.c_str());
  }
  if (TString(option) != "") {
    me->setOption(option);
  }
  if (bookStatOverflowFlag_) {
    me->getTH1F()->StatOverflows(kTRUE);
  }
  return me;
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::bookH2withSumw2(DQMStore::IBooker &iBooker,
                                                                                    const std::string &name,
                                                                                    const std::string &title,
                                                                                    int nchX,
                                                                                    double lowX,
                                                                                    double highX,
                                                                                    int nchY,
                                                                                    double lowY,
                                                                                    double highY,
                                                                                    const std::string &titleX,
                                                                                    const std::string &titleY,
                                                                                    Option_t *option) {
  iBooker.setCurrentFolder(outputInternalPath_);
  MonitorElement *me = iBooker.book2D(newName(name), title, nchX, lowX, highX, nchY, lowY, highY);
  me->enableSumw2();
  if (!titleX.empty()) {
    me->setAxisTitle(titleX);
  }
  if (!titleY.empty()) {
    me->setAxisTitle(titleY, 2);
    //me->getTH2F()->GetYaxis()->SetTitle(titleY.c_str());
  }
  if (TString(option) != "") {
    me->setOption(option);
  }
  if (bookStatOverflowFlag_) {
    me->getTH1F()->StatOverflows(kTRUE);
  }
  return me;
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::bookP1(DQMStore::IBooker &iBooker,
                                                                           const std::string &name,
                                                                           const std::string &title,
                                                                           int nchX,
                                                                           double lowX,
                                                                           double highX,
                                                                           double lowY,
                                                                           double highY,
                                                                           const std::string &titleX,
                                                                           const std::string &titleY,
                                                                           Option_t *option) {
  iBooker.setCurrentFolder(outputInternalPath_);
  MonitorElement *me = iBooker.bookProfile(newName(name), title, nchX, lowX, highX, lowY, highY, " ");
  if (!titleX.empty()) {
    me->getTProfile()->GetXaxis()->SetTitle(titleX.c_str());
  }
  if (!titleY.empty()) {
    me->setAxisTitle(titleY, 2);
    //me->getTProfile()->GetYaxis()->SetTitle(titleY.c_str());
  }
  if (TString(option) != "") {
    me->getTProfile()->SetOption(option);
  }
  if (bookStatOverflowFlag_) {
    me->getTH1F()->StatOverflows(kTRUE);
  }
  return me;
}

std::string ElectronDqmCommonBase::newName(const std::string &name) {
  if (bookPrefix_.empty()) {
    return name;
  }
  std::ostringstream oss;
  oss << bookPrefix_;
  if (bookIndex_ >= 0) {
    oss << bookIndex_++;
  }
  oss << "_" << name;
  return oss.str();
}

// Features for ElectronDqmHarvesterBase
const std::string *ElectronDqmCommonBase::find(DQMStore::IGetter &iGetter, const std::string &name) {
  if (!histoNamesReady) {
    histoNamesReady = true;
    histoNames_ = iGetter.getMEs();
  }
  std::vector<const std::string *> res;
  std::size_t nsize = name.size();

  for (const auto &histoName : histoNames_) {
    std::size_t lsize = histoName.size();
    if ((lsize >= nsize) && (histoName.find(name) == (lsize - nsize))) {
      res.push_back(&histoName);
    }
  }
  if (res.empty()) {
    std::ostringstream oss;
    oss << "Histogram " << name << " not found in " << outputInternalPath_;
    char sep = ':';
    for (auto const &histoName : histoNames_) {
      oss << sep << ' ' << histoName;
      sep = ',';
    }
    oss << '.';
    edm::LogWarning("ElectronDqmCommonBase::find") << oss.str();
    return nullptr;
  } else if (res.size() > 1) {
    std::ostringstream oss;
    oss << "Ambiguous histograms for " << name << " in " << outputInternalPath_;
    char sep = ':';
    for (auto const resItr : res) {
      oss << sep << ' ' << *resItr;
      sep = ',';
    }
    oss << '.';
    edm::LogWarning("ElectronDqmCommonBase::find") << oss.str();
    return nullptr;
  }
  return res[0];
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::get(DQMStore::IGetter &iGetter,
                                                                        const std::string &name) {
  const std::string *fullName = find(iGetter, name);
  if (fullName) {
    return iGetter.get(inputInternalPath_ + "/" + *fullName);
  } else {
    return nullptr;
  }
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::bookH1andDivide(DQMStore::IBooker &iBooker,
                                                                                    DQMStore::IGetter &iGetter,
                                                                                    const std::string &name,
                                                                                    const std::string &num,
                                                                                    const std::string &denom,
                                                                                    const std::string &titleX,
                                                                                    const std::string &titleY,
                                                                                    const std::string &title) {
  return bookH1andDivide(iBooker, iGetter, name, get(iGetter, num), get(iGetter, denom), titleX, titleY, title);
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::bookH1andDivide(DQMStore::IBooker &iBooker,
                                                                                    DQMStore::IGetter &iGetter,
                                                                                    const std::string &name,
                                                                                    MonitorElement *num,
                                                                                    MonitorElement *denom,
                                                                                    const std::string &titleX,
                                                                                    const std::string &titleY,
                                                                                    const std::string &title) {
  if ((!num) || (!denom))
    return nullptr;
  iBooker.setCurrentFolder(outputInternalPath_);
  std::string name2 = newName(name);
  TH1F *h_temp = (TH1F *)num->getTH1F()->Clone(name2.c_str());
  h_temp->Reset();
  h_temp->Divide(num->getTH1(), denom->getTH1(), 1, 1, "b");
  h_temp->GetXaxis()->SetTitle(titleX.c_str());
  h_temp->GetYaxis()->SetTitle(titleY.c_str());
  if (!title.empty()) {
    h_temp->SetTitle(title.c_str());
  }
  //if (verbosity_ > 0) {
  if (verbosity() > 0) {
    h_temp->Print();
  }
  MonitorElement *me = iBooker.book1D(name2, h_temp);
  if (bookEfficiencyFlag_) {
  //if (bookEfficiencyFlag()) {
    me->setEfficiencyFlag();
  }
  delete h_temp;
  return me;
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::bookH2andDivide(DQMStore::IBooker &iBooker,
                                                                                    DQMStore::IGetter &iGetter,
                                                                                    const std::string &name,
                                                                                    const std::string &num,
                                                                                    const std::string &denom,
                                                                                    const std::string &titleX,
                                                                                    const std::string &titleY,
                                                                                    const std::string &title) {
  return bookH2andDivide(iBooker, iGetter, name, get(iGetter, num), get(iGetter, denom), titleX, titleY, title);
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::bookH2andDivide(DQMStore::IBooker &iBooker,
                                                                                    DQMStore::IGetter &iGetter,
                                                                                    const std::string &name,
                                                                                    MonitorElement *num,
                                                                                    MonitorElement *denom,
                                                                                    const std::string &titleX,
                                                                                    const std::string &titleY,
                                                                                    const std::string &title) {
  if ((!num) || (!denom))
    return nullptr;
  iBooker.setCurrentFolder(outputInternalPath_);
  std::string name2 = newName(name);
  TH2F *h_temp = (TH2F *)num->getTH2F()->Clone(name2.c_str());
  h_temp->Reset();
  h_temp->Divide(num->getTH1(), denom->getTH1(), 1, 1, "b");
  h_temp->GetXaxis()->SetTitle(titleX.c_str());
  h_temp->GetYaxis()->SetTitle(titleY.c_str());
  if (!title.empty()) {
    h_temp->SetTitle(title.c_str());
  }
  //if (verbosity_ > 0) {
  if (verbosity() > 0) {
    h_temp->Print();
  }
  MonitorElement *me = iBooker.book2D(name2, h_temp);
  if (bookEfficiencyFlag_) {
  //if (bookEfficiencyFlag()) {
    me->setEfficiencyFlag();
  }
  delete h_temp;
  return me;
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::cloneH1(DQMStore::IBooker &iBooker,
                                                                            DQMStore::IGetter &iGetter,
                                                                            const std::string &clone,
                                                                            const std::string &original,
                                                                            const std::string &title) {
  return cloneH1(iBooker, iGetter, clone, get(iGetter, original), title);
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::cloneH1(DQMStore::IBooker &iBooker,
                                                                            DQMStore::IGetter &iGetter,
                                                                            const std::string &name,
                                                                            MonitorElement *original,
                                                                            const std::string &title) {
  if (!original)
    return nullptr;
  iBooker.setCurrentFolder(outputInternalPath_);
  std::string name2 = newName(name);
  TH1F *h_temp = (TH1F *)original->getTH1F()->Clone(name2.c_str());
  h_temp->Reset();
  if (!title.empty()) {
    h_temp->SetTitle(title.c_str());
  }
  MonitorElement *me = iBooker.book1D(name2, h_temp);
  delete h_temp;
  return me;
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::profileX(DQMStore::IBooker &iBooker,
                                                                             DQMStore::IGetter &iGetter,
                                                                             const std::string &me2d,
                                                                             const std::string &title,
                                                                             const std::string &titleX,
                                                                             const std::string &titleY,
                                                                             Double_t minimum,
                                                                             Double_t maximum) {
  return profileX(iBooker, iGetter, get(iGetter, me2d), title, titleX, titleY, minimum, maximum);
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::profileX(DQMStore::IBooker &iBooker,
                                                                             DQMStore::IGetter &iGetter,
                                                                             MonitorElement *me2d,
                                                                             const std::string &title,
                                                                             const std::string &titleX,
                                                                             const std::string &titleY,
                                                                             Double_t minimum,
                                                                             Double_t maximum) {
  iBooker.setCurrentFolder(outputInternalPath_);
  std::string name2 = me2d->getName() + "_pfx";
  TProfile *p1_temp = me2d->getTH2F()->ProfileX();
  if (!title.empty()) {
    p1_temp->SetTitle(title.c_str());
  }
  if (!titleX.empty()) {
    p1_temp->GetXaxis()->SetTitle(titleX.c_str());
  }
  if (!titleY.empty()) {
    p1_temp->GetYaxis()->SetTitle(titleY.c_str());
  }
  if (minimum != -1111) {
    p1_temp->SetMinimum(minimum);
  }
  if (maximum != -1111) {
    p1_temp->SetMaximum(maximum);
  }
  MonitorElement *me = iBooker.bookProfile(name2, p1_temp);
  delete p1_temp;
  return me;
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::profileY(DQMStore::IBooker &iBooker,
                                                                             DQMStore::IGetter &iGetter,
                                                                             const std::string &me2d,
                                                                             const std::string &title,
                                                                             const std::string &titleX,
                                                                             const std::string &titleY,
                                                                             Double_t minimum,
                                                                             Double_t maximum) {
  return profileY(iBooker, iGetter, get(iGetter, me2d), title, titleX, titleY, minimum, maximum);
}

ElectronDqmCommonBase::MonitorElement *ElectronDqmCommonBase::profileY(DQMStore::IBooker &iBooker,
                                                                             DQMStore::IGetter &iGetter,
                                                                             MonitorElement *me2d,
                                                                             const std::string &title,
                                                                             const std::string &titleX,
                                                                             const std::string &titleY,
                                                                             Double_t minimum,
                                                                             Double_t maximum) {
  iBooker.setCurrentFolder(outputInternalPath_);
  std::string name2 = me2d->getName() + "_pfy";
  TProfile *p1_temp = me2d->getTH2F()->ProfileY();
  if (!title.empty()) {
    p1_temp->SetTitle(title.c_str());
  }
  if (!titleX.empty()) {
    p1_temp->GetXaxis()->SetTitle(titleX.c_str());
  }
  if (!titleY.empty()) {
    p1_temp->GetYaxis()->SetTitle(titleY.c_str());
  }
  if (minimum != -1111) {
    p1_temp->SetMinimum(minimum);
  }
  if (maximum != -1111) {
    p1_temp->SetMaximum(maximum);
  }
  MonitorElement *me = iBooker.bookProfile(name2, p1_temp);
  delete p1_temp;
  return me;
}


