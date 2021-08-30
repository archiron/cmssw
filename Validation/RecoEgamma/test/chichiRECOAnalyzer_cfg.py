from __future__ import print_function

import sys
import os
import FWCore.ParameterSet.Config as cms

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))
# first arg : cmsRun
# second arg : name of the _cfg file
# third arg : sample name (ex. ZEE_14)

## Launch cmsRun chichiRECOAnalyzer_cfg.py ZEE_14

if ('DD_SAMPLE' not in os.environ) or (os.environ['DD_SAMPLE'] == ''):
    if (len(sys.argv) > 2):  # no else part since if sample does not exist, we had quit previously
        sampleName = str(sys.argv[2])
        os.environ['DD_SAMPLE'] = 'RelVal' + sampleName
        print('Sample name:', sampleName, ' - ', os.environ['DD_SAMPLE'])
    else:
        print('====================')
        print('no sample name, quit')
        print('====================')
        quit()

# RECO 12_0_0_pre1 : DQM_V0001_R000000001__RelValZEE_14__CMSSW_12_0_0_pre1-113X_mcRun4_realistic_v7_2026D76noPU-v1__DQMIO.root
# miniAOD 12_0_0_pre1 : DQM_V0001_R000000001__RelValZEE_14__CMSSW_12_0_0_pre1-113X_mcRun4_realistic_v7_2026D76noPU-v1__DQMIO.root

beginTag = 'Phase2'
# beginTag = 'Run2_2017'
os.environ['beginTag'] = 'Phase2'
# os.environ['beginTag'] = 'Run2_2017'
dd_tier = 'GEN-SIM-RECO'
#dd_tier = 'MINIAODSIM'
tag_startup = '113X_mcRun4_realistic_v7_2026D76noPU'
global_tag = '113X_mcRun4_realistic_v7'
data_version = 'v1'
dd_cond = tag_startup + '-' + data_version  # noPU

if ('DD_TIER' not in os.environ) or (os.environ['DD_TIER'] == ''):
    os.environ['DD_TIER'] = dd_tier
if 'TAG_STARTUP' not in os.environ:  # TAG_STARTUP from OvalFile
    os.environ['TAG_STARTUP'] = tag_startup  # '93X_upgrade2023_realistic_v0_D17PU200'
if 'DATA_VERSION' not in os.environ:  # DATA_VERSION from OvalFile
    os.environ['DATA_VERSION'] = data_version  # 'v1'
if 'TEST_GLOBAL_TAG' not in os.environ:  # TEST_GLOBAL_TAG from OvalFile
    os.environ['TEST_GLOBAL_TAG'] = tag_startup
if ('DD_COND' not in os.environ) or (os.environ['DD_COND'] == ''):
    os.environ['DD_COND'] = dd_cond  # noPU  # 'PU25ns_' + os.environ['TEST_GLOBAL_TAG'] + '-' + os.environ['DATA_VERSION']

os.environ['DD_RELEASE'] = os.environ['CMSSW_VERSION']
# os.environ['DD_RELEASE'] = "CMSSW_11_3_0_pre3"

print('=====')
if ('DD_SAMPLE_OUT' not in os.environ) or (os.environ['DD_SAMPLE_OUT'] == ''):
    os.environ['DD_SAMPLE_OUT'] = os.environ['DD_SAMPLE'].replace("RelVal", "ValFull")
print('=====')

os.environ['DD_SOURCE'] = '/eos/cms/store/relval/' + os.environ['DD_RELEASE'] + '/' + os.environ[
    'DD_SAMPLE'] + '/' + os.environ['DD_TIER'] + '/' + os.environ['DD_COND']
os.environ['data'] = '/' + os.environ['DD_SAMPLE'] + '/' + os.environ['DD_RELEASE'] + '-' + os.environ[
    'DD_COND'] + '/' + os.environ['DD_TIER']
os.environ['outputFile'] = 'electronHistos.' + os.environ['DD_SAMPLE_OUT'] + '_gedGsfE.root'
if ('inputPostFile' not in os.environ) or (os.environ['inputPostFile'] == ''):
    print('inputPostFile : %s' % os.environ['outputFile'])
    os.environ['inputPostFile'] = os.environ['outputFile']

print('DD_RELEASE', os.environ['DD_RELEASE'])
print('DD_SAMPLE', os.environ['DD_SAMPLE'])
print('DD_SAMPLE_OUT', os.environ['DD_SAMPLE_OUT'])
print('DD_COND', os.environ['DD_COND'])
print('DD_TIER', os.environ['DD_TIER'])
print('DD_SOURCE', os.environ['DD_SOURCE'])
print('data', os.environ['data'])
print('outputFile    :', os.environ['outputFile'])
print('inputPostFile :', os.environ['inputPostFile'])
print('beginTag : ', beginTag)

import DQMOffline.EGamma.electronDataDiscovery as dd

if beginTag == 'Run2_2017':
    from Configuration.Eras.Era_Run2_2017_cff import Run2_2017
    process = cms.Process("electronValidation", Run2_2017)
else:
    from Configuration.Eras.Era_Phase2_cff import Phase2
    process = cms.Process('electronValidation', Phase2)

process.DQMStore = cms.Service("DQMStore")
process.load("DQMServices.Components.DQMStoreStats_cfi")
from DQMServices.Components.DQMStoreStats_cfi import *
dqmStoreStats.runOnEndJob = cms.untracked.bool(True)

print("reading files ...")
max_number = -1 # -1 or 10 number of events
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(max_number))

data = os.environ['data']
flist = dd.getCMSdata(data)
print(flist)
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(*flist))
print("reading files done")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load("Configuration.StandardSequences.EDMtoMEAtJobEnd_cff")  # new
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = global_tag

# FOR DATA REDONE FROM RAW, ONE MUST HIDE IsoFromDeps
# CONFIGURATION
process.load("Validation.RecoEgamma.electronIsoFromDeps_cff")
process.load("Validation.RecoEgamma.chichiRECOAnalyzer_cfi")

# load DQM
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

process.chichiRECOAnalyzer.InputFolderName = cms.string("EgammaV/chichiRECOAnalyzer")
process.chichiRECOAnalyzer.OutputFolderName = cms.string("EgammaV/chichiRECOAnalyzer")

process.p = cms.Path(process.chichiRECOAnalyzer * process.MEtoEDMConverter)
