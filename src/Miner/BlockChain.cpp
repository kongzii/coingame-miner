//
// Created by peter on 18/07/19.
//

#include "BlockChain.h"


BlockChain::BlockChain(const std::string &url) {
    this->url = url;
}

std::string BlockChain::get(const std::string &endpoint, Accept accept) const {
    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    request.setOpt(curlpp::options::Url(this->url + endpoint));

    std::list<std::string> header;

    if (accept == JSON) {
        header.emplace_back("Accept: text/json");
    }

    else if (accept == TEXT) {
        header.emplace_back("Accept: text");
    }

    request.setOpt(curlpp::options::HttpHeader(header));

    std::ostringstream os;
    request.setOpt(curlpp::options::WriteStream(&os));

    request.perform();

    return os.str();
}

void BlockChain::put(const std::string &content) const {
    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    std::list<std::string> header;

    request.setOpt(curlpp::options::Url(this->url));
    request.setOpt(curlpp::options::CustomRequest{"PUT"});
    request.setOpt(curlpp::options::HttpHeader(header));
    request.setOpt(curlpp::options::PostFields(content));

    std::ostringstream os;
    request.setOpt(curlpp::options::WriteStream(&os));

    request.perform();

    std::cout << os.str() << std::endl;
}

State BlockChain::get_state() const {
    const auto response = this->get("/state", JSON);

    rapidjson::Document doc;
    doc.Parse(response.c_str());

    return State{doc["Fee"].GetDouble(), doc["Digest"].GetString(), doc["Difficulty"].GetInt()};
}

Transactions BlockChain::get_transactions() const {
    return BlockChain::parse_transactions(this->get("/txpool", TEXT));
}

Transactions BlockChain::parse_transactions(const std::string &input) {
    Transaction transaction;
    Transactions transactions;

    std::string line;
    std::istringstream f(input);

    while (std::getline(f, line)) {
        if (BlockChain::starts_with(line, "---")) {
            if (!transaction.id.empty()) {
                transactions.emplace_back(transaction);
            }

            transaction = Transaction();
        }

        else if (BlockChain::starts_with(line, "Fee")) {
            transaction.fee = atof(line.substr(5, line.size()).c_str());
        }

        else if (BlockChain::starts_with(line, "Id")) {
            transaction.id = line.substr(4, line.size());
        }

        else if (BlockChain::starts_with(line, "Data")) {
            transaction.data = line.substr(6, line.size()) + "\n";
        }

        else if (BlockChain::starts_with(line, "ValidTo")) {
            tm valid_to;
            strptime(line.c_str(), "%Y-%m-%d %H:%M:%S", &valid_to);

            transaction.valid_to = timegm(&valid_to);
            transaction.valid_to_str = line;
        }

        else {
            if (!transaction.data.empty()) {
                transaction.data += "   " + line;
            }
        }
    }

    return transactions;
}

bool BlockChain::starts_with(const std::string &input, const std::string &prefix) {
    return input.compare(0, prefix.size(), prefix) == 0;
}