//
// Created by peter on 18/07/19.
//

#include "Transaction.h"


Transaction::Transaction(const std::string &id, double fee, const std::string &data, time_t valid_to) {
    this->id = id;
    this->fee = fee;
    this->data = data;
    this->valid_to = valid_to;
}

const std::string Transaction::create_transaction() {
    if (!this->cache.empty()) {
        return this->cache;
    }

    std::string t = "  - !Transaction\n";

    if (!this->id.empty()) {
        t += "    Id: " + this->id + "\n";
    }

    if (this->fee > 0) {
        t += "    Fee: " + std::to_string(this->fee) + "\n";
    }

    if (!this->data.empty()) {
        t += "    Data: " + this->data + "\n";
    }

    this->cache = t;
    return t;
}