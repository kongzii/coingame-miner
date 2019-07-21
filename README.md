# TeskaLabs coingame

Client for https://github.com/TeskaLabs/coingame

## Dependencies

Tested on Linux Mint Tessa 64-bit

```
sudo apt-get install gcc g++ cmake pkg-config libcurlpp-dev libcurl4-openssl-dev libssl-dev
```

## Building

```
mkdir build
cd build
cmake ..
make
```

## Config

Example in `config.cfg`, options:

```
NAME: Name of miner
REWARD: Fee value of reward transaction
N_WORKERS: Number of threads to start
TRANSACTIONS_PER_WORKER: Number of transactions each thread will use in block (transactions will be splitted from pool)
NONCE_LIMIT: Limit of maximum nonce value, after reached, blockchain will update from server and start from 0
URL: Address of API endpoint
```

## Run

```
./miner ../config.cfg
```

## Master - Workers architecture

Application uses standart master-worker(s) architecture, and works like this:

1. Master fetches blockchain information from server (difficulty, last block, etc.)
2. Master spawns N workers
3. All workers uses blockchain information from master memory, except for transactions, which are divided uniquely to them
4. When some worker finds block that satisfies difficulty, he sends string representation to the master
5. All threads get paused and waits
6. Master sends stringified block to the server, fetches new data and resumes workers
7. Go to 3.

There is also timeout fuse, where master fetch new data from server if block was not found in a long time, because transactions get invalidated in time.

## Performance

According to performance measurements, time in application is divided (see images/flame_chart.png):

```
Hashing of block content (using openssl/sha.h): 86%
Miner: 11%
Requests: 3%
```

In my opinion, even though miner logic could still be improved, it would make only little difference until new hashing method is implemented.

![Performance example](https://github.com/kongzii/coingame-miner/blob/master/images/performance.png "Performance example")