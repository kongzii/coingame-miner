#include <iostream>
#include <cstdlib>

#include "Config/Config.h"
#include "Miner/Generator.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./TestDayMiner path_to_config_file" << std::endl;
        exit(-1);
    }

    auto config = Config(argv[1]);

    auto NAME = config.get_str("NAME");
    auto URL = config.get_str("URL");
    auto N_WORKERS = config.get_int("N_WORKERS");
    auto TRANSACTIONS_PER_WORKER = config.get_int("TRANSACTIONS_PER_WORKER");
    auto REWARD = atof(config.get_str("REWARD").c_str());
    auto NONCE_LIMIT = config.get_int("NONCE_LIMIT");

    auto generator = Generator(NAME, URL, REWARD, N_WORKERS, TRANSACTIONS_PER_WORKER, NONCE_LIMIT);

    std::cout << "Starting generator" << std::endl << std::endl;

    generator.run();
}