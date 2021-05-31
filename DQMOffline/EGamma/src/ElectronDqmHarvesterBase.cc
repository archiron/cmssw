
#include "DQMOffline/EGamma/interface/ElectronDqmHarvesterBase.h"
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

ElectronDqmHarvesterBase::ElectronDqmHarvesterBase(const edm::ParameterSet &conf)
    : ElectronDqmCommonBase(conf), finalDone_(false) {}

ElectronDqmHarvesterBase::~ElectronDqmHarvesterBase() {}

void ElectronDqmHarvesterBase::beginJob() {}

void ElectronDqmHarvesterBase::dqmEndLuminosityBlock(DQMStore::IBooker &iBooker,
                                                     DQMStore::IGetter &iGetter,
                                                     edm::LuminosityBlock const &,
                                                     edm::EventSetup const &) {
  if (finalStep_ == "AtLumiEnd") {
    if (finalDone_) {
      edm::LogWarning("ElectronDqmHarvesterBase::endLuminosityBlock") << "finalize() already called";
    }
    iBooker.setCurrentFolder(outputInternalPath_);
    finalDone_ = true;
  }
}

void ElectronDqmHarvesterBase::dqmEndJob(DQMStore::IBooker &iBooker, DQMStore::IGetter &iGetter) {
  if (finalStep_ == "AtJobEnd") {
    if (finalDone_) {
      edm::LogWarning("ElectronDqmHarvesterBase::dqmEndJob") << "finalize() already called";
    }
    iBooker.setCurrentFolder(outputInternalPath_);
    finalDone_ = true;
  }
  iBooker.setCurrentFolder(outputInternalPath_);
  finalize(iBooker, iGetter);
}

void ElectronDqmHarvesterBase::remove(DQMStore::IBooker &iBooker, DQMStore::IGetter &iGetter, const std::string &name) {
  // TODO: remove no longer supported in DQMStore.
}
