//
// Created by peter on 20/07/19.
//

#include "Generator.h"

pthread_mutex_t mutex;
std::string block_body_content;
pthread_cond_t block_found;
pthread_cond_t block_sent;

typedef struct {
    int id;
    Generator *generator;
} worker_args_t;

void *worker(void *args_) {
    const auto args = (worker_args_t *) args_;

    auto my_block = Block(args->generator->miner_name, args->generator->reward);

    args->generator->update_block(my_block, args->id);

    forever {
        if (!block_body_content.empty()) {
            pthread_cond_wait(&block_sent, &mutex);
            pthread_mutex_unlock(&mutex);

            args->generator->update_block(my_block, args->id);
        }

        ++my_block.nonce;

        const auto body_content = args->generator->create_body_content(my_block);
        const auto difficulty = Generator::calculate_difficulty(body_content);

        if (difficulty >= args->generator->state.difficulty) {
            pthread_mutex_lock(&mutex);

            std::cout << "Thread " << args->id
                      << " found block difficulty " << difficulty
                      << " after " << (std::time(nullptr) - args->generator->last_found_time) << " seconds: ";

            block_body_content = body_content;

            pthread_cond_broadcast(&block_found);
            pthread_mutex_unlock(&mutex);
            continue;
        }

        if (my_block.nonce > my_block.loop * args->generator->nonce_limit && block_body_content.empty()) {
            // Checks size of integer instead of exact time passed, because getting current time is costly operation
            // Modify value to not be too often (getting new transactions from server is very slow)
            // but also to not be too few (transactions become outdated easily)

            const auto last_update = args->generator->last_update_time;

            pthread_mutex_lock(&mutex);

            if (last_update < args->generator->last_update_time || !block_body_content.empty()) {
                // Check if wasnt requested by another thread in mean time
                pthread_mutex_unlock(&mutex);
                continue;
            }

            std::cout << "Thread " << args->id
                      << " lot of time passed by, requiring update... " << std::endl;

            // For simplicity of signals, use block_found and block_body_content, master will check what to do
            block_body_content = "update";

            pthread_cond_broadcast(&block_found);
            pthread_mutex_unlock(&mutex);
            continue;
        }
    }
}

void Generator::run() {
    // Init blockchain

    this->update_blockchain();

    // Init communication variables

    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&block_found, nullptr);
    pthread_cond_init(&block_sent, nullptr);

    // Create workers

    pthread_t workers_t[this->n_workers];
    worker_args_t workers_args[this->n_workers];

    for (int i = 0; i < this->n_workers; ++i) {
        workers_args[i] = {i, this};

        if (pthread_create(&workers_t[i], nullptr, worker, (void *) &workers_args[i]) != 0) {
            std::cerr << "Error while creating worker " << i << std::endl;
            exit(-1);
        } else {
            std::cout << "Created worker thread " << i << std::endl;
        }
    }

    std::cout << std::endl;

    forever {
        pthread_cond_wait(&block_found, &mutex);

        if (block_body_content != "update") {
            // Worker found valid block
            this->blockchain.put(block_body_content);
            this->last_found_time = std::time(nullptr);
        }

        block_body_content = "";
        this->update_blockchain();

        pthread_cond_broadcast(&block_sent);
        pthread_mutex_unlock(&mutex);
    }
}
