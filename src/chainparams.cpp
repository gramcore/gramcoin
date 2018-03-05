// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2018 The GRAM Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>

#include "chainparamsseeds.h"

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 504365055 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}
/*
static void MineGenesis(CBlockHeader& genesisBlock, const uint256& powLimit, bool noProduction)
{

    genesisBlock.nTime = 1514764800; // 01/01/18
    genesisBlock.nNonce = 0;

    printf("NOTE: Genesis nTime = %u \n", genesisBlock.nTime);
    printf("WARN: Genesis nNonce = %u \n", genesisBlock.nNonce);

    arith_uint256 besthash;
    memset(&besthash,0xFF,32);
    arith_uint256 hashTarget = UintToArith256(powLimit);
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    arith_uint256 newhash = UintToArith256(genesisBlock.GetHash());
    while (newhash > hashTarget) {
        genesisBlock.nNonce++;
        if (genesisBlock.nNonce == 0) {
            ++genesisBlock.nTime;
        }
        if ((genesisBlock.nNonce & 0xfff) == 0)
            printf("time %u: nonce %u: hash = %s\n", genesisBlock.nTime, 
                   genesisBlock.nNonce, newhash.ToString().c_str());

        if(newhash < besthash) {
            besthash = newhash;
            printf("Best: %s\n", newhash.GetHex().c_str());
        }
        newhash = UintToArith256(genesisBlock.GetHash());
    }
    printf("Genesis nTime = %u \n", genesisBlock.nTime);
    printf("Genesis nNonce = %u \n", genesisBlock.nNonce);
    printf("Genesis nBits: %08x\n", genesisBlock.nBits);
    printf("Genesis Hash = %s\n", newhash.ToString().c_str());
    printf("Genesis Hash Merkle Root = %s\n", genesisBlock.hashMerkleRoot.ToString().c_str());
}
*/
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Success is often achieved by those who don't know that failure is inevitable";
    const CScript genesisOutputScript = CScript() << ParseHex("0475c0cb8843f83363b11fba9f6d6f2e793e7fbc1c606f1541ffc7741a305783913a0b455e37164eb8b90250e7254d691f7b5f277c4acb78cc91a02b94ec8fbad8") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
/**

Genesis nTime = 1514764800 
Genesis nNonce = 285477 
Genesis nBits: 1e0fffff
Genesis Hash = 000000ec74514a010f69552d45a6880dcc13635f687339525517e77521bb1cbe
Genesis Hash Merkle Root = ffa1771723ffaea20ba1339e9ae0d5c0552dc3e74a83f8c5035f34b5f445d795

**/
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 144000;

        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x000000ec74514a010f69552d45a6880dcc13635f687339525517e77521bb1cbe");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;

        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nPowTargetTimespan = 24 * 60 * 60; // 1 day
        consensus.nPowTargetSpacing = 2 * 60; // 2 min

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;

        consensus.nRuleChangeActivationThreshold = 684; // 95% of 720
        consensus.nMinerConfirmationWindow = 720; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1514764800; // January 1, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1546300799; // December 31, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1514764800; // January 1, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1546300799; // December 31, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000000000a142e89b"); // 0.001537902015823812

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000016c46bcce2c99731f6395a92b1458246df5c230232fb66d935bbf647779"); // #2160

        pchMessageStart[0] = 0x4d;
        pchMessageStart[1] = 0x41;
        pchMessageStart[2] = 0x52;
        pchMessageStart[3] = 0x47;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);  // G
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,98);  // g
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,166); // ?
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};          // xpub
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};          // xprv

        nDefaultPort = 7567;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1514764800, 285477, 0x1e0fffff, 1, 220 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x000000ec74514a010f69552d45a6880dcc13635f687339525517e77521bb1cbe"));
        assert(genesis.hashMerkleRoot == uint256S("0xffa1771723ffaea20ba1339e9ae0d5c0552dc3e74a83f8c5035f34b5f445d795"));

        //vFixedSeeds.clear();
        //vSeeds.clear();

        vSeeds.emplace_back("net.gramcore.org");

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {    0, uint256S("0x000000ec74514a010f69552d45a6880dcc13635f687339525517e77521bb1cbe")}, // genesis
                {  720, uint256S("0x000004422d8cf26940e5bbe8a74a8e494f1feede40efc49427034de52c6c4d6d")}, // retarget
                { 1440, uint256S("0x000008bcfb827a2576d0cb7ede8fc7a46565311975937d946774378146976f40")}, // segwit locked
                { 2160, uint256S("0x0000016c46bcce2c99731f6395a92b1458246df5c230232fb66d935bbf647779")}, // segwit activated
            }
        };

        chainTxData = ChainTxData {

            // Data as of block 0x0000016c46bcce2c99731f6395a92b1458246df5c230232fb66d935bbf647779 (height 2160)

            1519749983,                 // * UNIX timestamp of last known number of transactions
            2238,                        // * total number of transactions between genesis and that timestamp
            0.01584680477105052        // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */

class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
/**

Genesis nTime = 1514764801 
Genesis nNonce = 326299 
Genesis nBits: 1e0fffff
Genesis Hash = 00000502f9aad434b6d0dc7920e61d5667deb2d5daaf95d6755268fd7ea87bb3
Genesis Hash Merkle Root = ffa1771723ffaea20ba1339e9ae0d5c0552dc3e74a83f8c5035f34b5f445d795

**/
        strNetworkID = "test";

        consensus.nSubsidyHalvingInterval = 144000;

        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00000502f9aad434b6d0dc7920e61d5667deb2d5daaf95d6755268fd7ea87bb3");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;

        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nPowTargetTimespan = 24 * 60 * 60; // 1 day
        consensus.nPowTargetSpacing = 2 * 60; // 2 min

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;

        consensus.nRuleChangeActivationThreshold = 540; // 75% for testchains
        consensus.nMinerConfirmationWindow = 720; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1514764800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1546300799; // December 31, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1514764800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1546300799; // December 31, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000010f0033a0"); 

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000002d2b3693597c7ef517b8babe9ea6028c08d66cb02a7ea5357cc861b845"); // #2160

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);  // G
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,98);  // g
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,166); // ?
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};          // xpub
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};          // xprv

        nDefaultPort = 17567;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1514764801, 326299, 0x1e0fffff, 1, 220 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00000502f9aad434b6d0dc7920e61d5667deb2d5daaf95d6755268fd7ea87bb3"));
        assert(genesis.hashMerkleRoot == uint256S("0xffa1771723ffaea20ba1339e9ae0d5c0552dc3e74a83f8c5035f34b5f445d795"));

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.emplace_back("testnet.gramcore.org", true);

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {    0, uint256S("00000502f9aad434b6d0dc7920e61d5667deb2d5daaf95d6755268fd7ea87bb3")}, // genesis
                {  720, uint256S("000004c5cf9e34391251d710b6346d902e8e1de9a977cdbe20659906766d1ecd")}, // retarget
                { 1440, uint256S("000002be8e305a2c9f0a8e18b4f1f8ddedd28ecd9bf377a39010afd2fea27cee")}, // segwit locked
                { 2160, uint256S("0000002d2b3693597c7ef517b8babe9ea6028c08d66cb02a7ea5357cc861b845")}, // segwit activated
            }
        };

        chainTxData = ChainTxData {

            // Data as of block 0000002d2b3693597c7ef517b8babe9ea6028c08d66cb02a7ea5357cc861b845 (height 2160)

            1519758155,                 // * UNIX timestamp of last known number of transactions
            2161,                       // * total number of transactions between genesis and that timestamp
            0.3646343523053538          // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Regression test
 */

class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
/**

Genesis nTime = 1514764802 
Genesis nNonce = 0 
Genesis nBits: 207fffff
Genesis Hash = 4d621bb1fffb81e103d9b94fb5698b5aab16e98d72368edfcc6f62e5ca495ea3
Genesis Hash Merkle Root = ffa1771723ffaea20ba1339e9ae0d5c0552dc3e74a83f8c5035f34b5f445d795

**/
        strNetworkID = "regtest";

        consensus.nSubsidyHalvingInterval = 144;

        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)

        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nPowTargetTimespan = 24 * 60 * 60; // 1 day
        consensus.nPowTargetSpacing = 2 * 60; // 2 min

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;

        consensus.nRuleChangeActivationThreshold = 540; // 75% for testchains
        consensus.nMinerConfirmationWindow = 720; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);  // G
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,98);  // g
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,166); // ?
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};          // xpub
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};          // xprv

        nDefaultPort = 17568;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1514764802, 0, 0x207fffff, 1, 220 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x4d621bb1fffb81e103d9b94fb5698b5aab16e98d72368edfcc6f62e5ca495ea3"));
        assert(genesis.hashMerkleRoot == uint256S("0xffa1771723ffaea20ba1339e9ae0d5c0552dc3e74a83f8c5035f34b5f445d795"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
                {   0, uint256S("4d621bb1fffb81e103d9b94fb5698b5aab16e98d72368edfcc6f62e5ca495ea3")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
