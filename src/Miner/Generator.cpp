//
// Created by peter on 18/07/19.
//

#include "Generator.h"

Generator::Generator(std::string &miner_name, std::string &url, double reward, int n_workers, int transactions_per_worker, int nonce_limit) {
    this->miner_name = miner_name;
    this->url = url;
    this->n_workers = n_workers;
    this->last_found_time = std::time(nullptr);
    this->reward = reward;
    this->transactions_per_worker = transactions_per_worker;
    this->nonce_limit = nonce_limit;

    this->blockchain = BlockChain(this->url);
}

std::string Generator::create_previous_block(const std::string &digest) {
    return "--- !Hash\nDigest: '" + digest + "'\n";
}

std::string Generator::create_body_content(Block &block) {
    return this->previous_block + block.create_block();
}

void Generator::update_state() {
    this->state = this->blockchain.get_state();
    this->previous_block = this->create_previous_block(this->state.digest);
}

void Generator::update_transactions() {
    this->transactions = this->blockchain.get_transactions();
}

void Generator::update_blockchain() {
    this->update_state();
    this->update_transactions();
    this->last_update_time = std::time(nullptr);
}

void Generator::update_block(Block &block, int worker_id) {
    // Update block values

    if (block.loop * block.nonce + this->nonce_limit >= std::numeric_limits<unsigned int>::max()) {
        // I want to reset nonce back to 0 only in case that it would overflow
        // Otherwise it could happen that worker gets same transactions and just computing everything from begin...
        block.nonce = 0;
        block.loop = 0;
    }

    // TODO: With enough available threads, different workers could start getting same set of transactions and therefore
    // TODO: they would compute same hash values, so get each worker his own nonce range

    block.update_timestamp();
    block.difficulty = this->state.difficulty;

    // Calculate transactions for this worker

    int n = this->transactions.size();
    int offset = (worker_id * this->transactions_per_worker) % n;
    int end = (offset + this->transactions_per_worker) % n;

    block.transactions = Transactions (this->transactions.begin() + offset,
                                       this->transactions.begin() + end);

    // Create new caches for block texts

    block.create_block_before_nonce(true);
    block.create_block_after_nonce(true);

    // New loop started

    block.loop += 1;
}

int Generator::calculate_difficulty(const std::string &content) {
    // Hash need to be computed on twice the content (game rules...), so copy two times in row
    char content_char[content.length() * 2 + 1];

    strcpy(&content_char[0], content.c_str());
    strcpy(&content_char[content.length()], content.c_str());

    unsigned char digest[SHA384_DIGEST_LENGTH];
    SHA384((unsigned char*) &content_char, strlen(content_char), (unsigned char*) &digest);

    int difficulty = 0;

    // Based on Heurekas Python hints
    // Could be breaked after we now that difficulty is big enough, but I am curious about max achieved diff.

    for (int i = 0; i < SHA384_DIGEST_LENGTH; ++i) {
        if ((digest[i] & 0x80) == 0x80) {
            return difficulty;
        }
        difficulty += 1;
        if ((digest[i] & 0x40) == 0x40) {
            return difficulty;
        }
        difficulty += 1;
        if ((digest[i] & 0x20) == 0x20) {
            return difficulty;
        }
        difficulty += 1;
        if ((digest[i] & 0x10) == 0x10) {
            return difficulty;
        }
        difficulty += 1;
        if ((digest[i] & 0x08) == 0x08) {
            return difficulty;
        }
        difficulty += 1;
        if ((digest[i] & 0x04) == 0x04) {
            return difficulty;
        }
        difficulty += 1;
        if ((digest[i] & 0x02) == 0x02) {
            return difficulty;
        }
        difficulty += 1;
        if ((digest[i] & 0x01) == 0x01) {
            return difficulty;
        }
        difficulty += 1;
    }

    return difficulty;
}

