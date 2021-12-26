#include <iostream>
#include <map>
#include <math.h>
#include <algorithm>
#include "predictor.h"

using namespace std;

// Handy Global for use in output routines
const char* bpName[4] = { "Static", "Gshare",
    "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits; // Number of bits used for PC index
int bpType; // Branch Prediction Type
int verbose;

class GsharePredictor {
public:
    uint32_t GHR; // global history record
    std::map<uint32_t, int8_t> BHT; // branch history table
};
GsharePredictor Gshare;

class TournamentPredictor {
public:
    uint32_t GHR; // global history record
    std::map<uint32_t, uint32_t> local_PHT; // pattern history table
    std::map<uint32_t, int8_t> local_BHT; // branch history table
    std::map<uint32_t, int8_t> global_BHT;
    std::map<uint32_t, int8_t> Chooser;
};
TournamentPredictor Tournament;

class CustomPredictor {
public:
    uint32_t GHR; // global history record
    std::map<uint32_t, uint32_t> local_PHT; // pattern history table
    std::map<uint32_t, int8_t> local_BHT; // branch history table
    std::map<uint32_t, int8_t> global_BHT;
    std::map<uint32_t, int8_t> Chooser;
};
CustomPredictor Custom;

// Initialize the predictor
void init_predictor()
{
    if (bpType == GSHARE) {
        Gshare.GHR = 0;
    }
    else if (bpType == TOURNAMENT) {
        Tournament.GHR = 0;
    }
    else if (bpType == CUSTOM) {
        Custom.GHR = 0;
    }
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t make_prediction(uint32_t pc)
{
    // Make a prediction based on the bpType
    switch (bpType) {
    case STATIC:
        return TAKEN;
    case GSHARE: {
        uint32_t pc_mask = uint32_t(pow(2, ghistoryBits) - 1);
        uint32_t BHT_index = pc_mask & pc ^ Gshare.GHR;
        int8_t prediction = Gshare.BHT[BHT_index];
        return prediction >= WT ? TAKEN : NOTTAKEN;
    }
    case TOURNAMENT: {
        uint32_t global_mask = uint32_t(pow(2, ghistoryBits) - 1);
        uint32_t local_mask = uint32_t(pow(2, lhistoryBits) - 1);
        uint32_t pc_mask = uint32_t(pow(2, pcIndexBits) - 1);
        uint32_t local_PHT_index = pc_mask & pc;
        uint32_t local_BHT_index = Tournament.local_PHT[local_PHT_index] & local_mask;
        int8_t local_prediction = Tournament.local_BHT[local_BHT_index];
        int8_t global_prediction = Tournament.global_BHT[Tournament.GHR & global_mask];
        int8_t choice = Tournament.Chooser[Tournament.GHR & global_mask];
        int8_t prediction = (choice >= WEAKLY_LOCAL) ? local_prediction : global_prediction;
        return prediction >= WT ? TAKEN : NOTTAKEN;
    }
    case CUSTOM: {
        uint32_t global_mask = uint32_t(pow(2, ghistoryBits) - 1);
        uint32_t local_mask = uint32_t(pow(2, lhistoryBits) - 1);
        uint32_t pc_mask = uint32_t(pow(2, pcIndexBits) - 1);
        uint32_t local_PHT_index = pc_mask & pc;
        uint32_t local_BHT_index = Custom.local_PHT[local_PHT_index] & local_mask;
        int8_t local_prediction = Custom.local_BHT[local_BHT_index];
        int8_t global_prediction = Custom.global_BHT[(Custom.GHR & global_mask) ^ (pc_mask & pc)];
        int8_t choice = Custom.Chooser[Tournament.GHR & global_mask];
        int8_t prediction = (choice >= WEAKLY_LOCAL) ? local_prediction : global_prediction;
        return prediction >= WT ? TAKEN : NOTTAKEN;
    }
    default:
        break;
    }

    // If there is not a compatable bpType then return NOTTAKEN
    return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//

void train_Gshare_predictor(uint32_t pc, uint8_t outcome)
{
    uint32_t pc_mask = uint32_t(pow(2, ghistoryBits) - 1);
    uint32_t BHT_index = pc_mask & pc ^ Gshare.GHR;
    uint32_t prediction = Gshare.BHT[BHT_index];

    //update BHT
    if (outcome == TAKEN)
        Gshare.BHT[BHT_index] = min(int8_t(3), ++Gshare.BHT[BHT_index]);
    else
        Gshare.BHT[BHT_index] = max(int8_t(0), --Gshare.BHT[BHT_index]);

    //update GHR
    // GHR[1:]+outcome
    Gshare.GHR = (Gshare.GHR << 1) | outcome;
}

void train_Tournament_predictor(uint32_t pc, uint8_t outcome)
{
    uint32_t global_mask = uint32_t(pow(2, ghistoryBits) - 1);
    uint32_t local_mask = uint32_t(pow(2, lhistoryBits) - 1);
    uint32_t pc_mask = uint32_t(pow(2, pcIndexBits) - 1);

    //update global predictor
    uint32_t global_BHT_index = Tournament.GHR & global_mask;
    int8_t global_prediction = Tournament.global_BHT[global_BHT_index];
    if (outcome == TAKEN)
        Tournament.global_BHT[global_BHT_index] = min(int8_t(3), ++Tournament.global_BHT[global_BHT_index]);
    else
        Tournament.global_BHT[global_BHT_index] = max(int8_t(0), --Tournament.global_BHT[global_BHT_index]);

    uint32_t ori_GHR = Tournament.GHR;
    Tournament.GHR = (Tournament.GHR << 1) | outcome;

    //update local predictor
    uint32_t local_PHT_index = pc_mask & pc;
    uint32_t local_BHT_index = Tournament.local_PHT[local_PHT_index] & local_mask;
    int8_t local_prediction = Tournament.local_BHT[local_BHT_index];
    if (outcome == TAKEN)
        Tournament.local_BHT[local_BHT_index] = min(int8_t(3), ++Tournament.local_BHT[local_BHT_index]);
    else
        Tournament.local_BHT[local_BHT_index] = max(int8_t(0), --Tournament.local_BHT[local_BHT_index]);

    Tournament.local_PHT[local_PHT_index] = (Tournament.local_PHT[local_PHT_index] << 1) | outcome;

    //update Chooser
    if (local_prediction != global_prediction) { // if the two predictors get the same result, there is no need to train the chooser
        if (global_prediction == outcome)
            Tournament.Chooser[ori_GHR & global_mask] = max(int8_t(STRONGLY_GLOBAL), --Tournament.Chooser[ori_GHR & global_mask]);
        else
            Tournament.Chooser[ori_GHR & global_mask] = min(int8_t(STRONGLY_LOCAL), ++Tournament.Chooser[ori_GHR & global_mask]);
    }
}

void train_Custom_predictor(uint32_t pc, uint8_t outcome)
{
    uint32_t global_mask = uint32_t(pow(2, ghistoryBits) - 1);
    uint32_t local_mask = uint32_t(pow(2, lhistoryBits) - 1);
    uint32_t pc_mask = uint32_t(pow(2, pcIndexBits) - 1);

    //update global predictor
    uint32_t global_BHT_index = (Custom.GHR & global_mask) ^ (pc_mask & pc);
    int8_t global_prediction = Custom.global_BHT[global_BHT_index];
    if (outcome == TAKEN)
        Custom.global_BHT[global_BHT_index] = min(int8_t(3), ++Custom.global_BHT[global_BHT_index]);
    else
        Custom.global_BHT[global_BHT_index] = max(int8_t(0), --Custom.global_BHT[global_BHT_index]);

    uint32_t ori_GHR = Custom.GHR;
    Custom.GHR = (Custom.GHR << 1) | outcome;

    //update local predictor
    uint32_t local_PHT_index = pc_mask & pc;
    uint32_t local_BHT_index = Custom.local_PHT[local_PHT_index] & local_mask;
    int8_t local_prediction = Custom.local_BHT[local_BHT_index];
    if (outcome == TAKEN)
        Custom.local_BHT[local_BHT_index] = min(int8_t(3), ++Custom.local_BHT[local_BHT_index]);
    else
        Custom.local_BHT[local_BHT_index] = max(int8_t(0), --Custom.local_BHT[local_BHT_index]);

    Custom.local_PHT[local_PHT_index] = (Custom.local_PHT[local_PHT_index] << 1) | outcome;

    //update Chooser
    if (local_prediction != global_prediction) { // if the two predictors get the same result, there is no need to train the chooser
        if (global_prediction == outcome)
            Custom.Chooser[ori_GHR & global_mask] = max(int8_t(STRONGLY_GLOBAL), --Custom.Chooser[ori_GHR & global_mask]);
        else
            Custom.Chooser[ori_GHR & global_mask] = min(int8_t(STRONGLY_LOCAL), ++Custom.Chooser[ori_GHR & global_mask]);
    }
}

void train_predictor(uint32_t pc, uint8_t outcome)
{
    if (bpType == GSHARE) {
        train_Gshare_predictor(pc, outcome);
    }
    else if (bpType == TOURNAMENT) {
        train_Tournament_predictor(pc, outcome);
    }
    else if (bpType == CUSTOM) {
        train_Custom_predictor(pc, outcome);
    }
}
