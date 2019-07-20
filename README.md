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

## Performance

According to performance measurements, time in application is divided (see images/flame_chart.png):

```
Hashing of block content (using openssl/sha.h): 86%
Miner: 11%
Requests: 3%
```

In my opinion, even though miner logic could still be improved, it would make only little difference until new hashing method is implemented.

![Performance example](https://github.com/kongzii/coingame-miner/blob/master/images/performance.png "Performance example")