//
// Created by peter on 18/07/19.
//

#include "Block.h"


Block::Block(const std::string &miner_name, double reward) {
    this->miner_name = miner_name;
    this->reward_transaction.fee = reward;

    this->update_timestamp();
}

void Block::update_timestamp() {
    this->timestamp = std::time(nullptr);
    this->iso_timestamp = "2019-07-20T10:29:00.893446+00:00";

    char buf[sizeof this->iso_timestamp + 1];
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%S.893446+00:00", gmtime(&this->timestamp));

    this->iso_timestamp = std::string(buf);
}

const std::string Block::create_block_before_nonce(bool force_update) {
    if (force_update || this->block_cache_before_nonce.empty()) {
        this->block_cache_before_nonce = "--- !Block\nTimestamp: " + this->iso_timestamp + "\nDifficulty: " + std::to_string(this->difficulty) + "\nNonce: ";
    }

    return this->block_cache_before_nonce;
}

const std::string Block::create_block_after_nonce(bool force_update) {
    if (force_update || this->block_cache_after_nonce.empty()) {
        this->block_cache_after_nonce = "\nMiner: " + this->miner_name + "\nTransactions:\n" + this->reward_transaction.create_transaction();

        for (auto &transaction : this->transactions) {
            this->block_cache_after_nonce += transaction.create_transaction();
        }
    }

    return this->block_cache_after_nonce;
}

const std::string Block::create_block() {
    // Only nonce is changing in the block forever iteration, so everything before and after is cached in two strings,
    // for better perfromance
    return this->create_block_before_nonce(false) +
           std::to_string(this->nonce) +
           this->create_block_after_nonce(false);
}