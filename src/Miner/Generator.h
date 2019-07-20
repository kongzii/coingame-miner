//
// Created by peter on 18/07/19.
//

#ifndef TESTDAYMINER_GENERATOR_H
#define TESTDAYMINER_GENERATOR_H

#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <pthread.h>

#include <openssl/sha.h>
#include "BlockChain.h"
#include "Block.h"
#include "Transaction.h"
#include "State.h"

#define forever while(1)

class Generator {
public:
    Generator(std::string &miner_name, std::string &url, double reward, int n_workers, int transactions_per_worker, int nonce_limit);

    void run();

    std::string miner_name;
    std::string url;
    double reward;

    int n_workers = 1;
    time_t last_found_time = 0;
    time_t last_update_time = 0;
    int transactions_per_worker = 4;
    int nonce_limit = 1000000;

    State state;
    BlockChain blockchain;

    std::string previous_block;
    Transactions transactions;

    std::string create_body_content(Block &block);

    void update_state();
    void update_transactions();

    void update_blockchain();
    void update_block(Block &block, int worker_id);

    static std::string create_previous_block(const std::string &digest);
    static int calculate_difficulty(const std::string &content);
};


#endif //TESTDAYMINER_GENERATOR_H
