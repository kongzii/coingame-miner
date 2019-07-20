//
// Created by peter on 18/07/19.
//

#ifndef TESTDAYMINER_BLOCKCHAIN_H
#define TESTDAYMINER_BLOCKCHAIN_H

#include <string>
#include <vector>
#include <cstdlib>
#include <cerrno>
#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "State.h"
#include "Transaction.h"

enum Accept { JSON, TEXT };

class BlockChain {
public:
    BlockChain() = default;
    explicit BlockChain(const std::string &url);

    void put(const std::string &content) const;
    std::string get(const std::string &endpoint, Accept accept) const;

    State get_state() const;
    Transactions get_transactions() const;
private:
    std::string url;

    static Transactions parse_transactions(const std::string &input);
    static bool starts_with(const std::string &input, const std::string &prefix);
};


#endif //TESTDAYMINER_BLOCKCHAIN_H
