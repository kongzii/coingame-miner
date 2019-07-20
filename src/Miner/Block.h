//
// Created by peter on 18/07/19.
//

#ifndef TESTDAYMINER_BLOCK_H
#define TESTDAYMINER_BLOCK_H

#include <ctime>
#include <vector>
#include <string>
#include "Transaction.h"
#include <iostream>


class Block {
public:
    time_t timestamp = 0;
    std::string iso_timestamp = "";
    int difficulty = 0;
    unsigned int nonce = 0;

    std::string miner_name;

    Transactions transactions;
    Transaction reward_transaction;

    void update_timestamp();

    Block() = default;
    explicit Block(const std::string &miner_name, double reward);

    std::string block_cache_before_nonce = "";
    std::string block_cache_after_nonce = "";

    const std::string create_block();
    const std::string create_block_before_nonce(bool force_update);
    const std::string create_block_after_nonce(bool force_update);
};


#endif //TESTDAYMINER_BLOCK_H
