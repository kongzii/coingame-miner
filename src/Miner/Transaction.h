//
// Created by peter on 18/07/19.
//

#ifndef TESTDAYMINER_TRANSACTION_H
#define TESTDAYMINER_TRANSACTION_H

#include <string>
#include <ctime>
#include <vector>


class Transaction {
public:
    std::string id = "";
    double fee = 0;
    std::string data = "";
    time_t valid_to = 0;
    std::string valid_to_str = "";
    std::string cache;

    Transaction() = default;
    Transaction(const std::string &id, double fee, const std::string &data, time_t valid_to);

    const std::string create_transaction();
};

typedef std::vector<Transaction> Transactions;


#endif //TESTDAYMINER_TRANSACTION_H
