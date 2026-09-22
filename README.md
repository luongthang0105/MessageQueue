# Message Queue
## System Requirements
- C++23 Standard
- At least `gcc-14/g++-14`

To compile and run, in root directory, run:
```bash
mkdir build
cmake -S . -B build -DCMAKE_CXX_COMPILER=g++-14
cd build
make && ./MessageQueue
```
Running `make` will also create a Doxygen documentation of this project at the `docs/` folder of the project root directory.

To contribute, make sure you run [lint.sh](/lint.sh) before commiting.

## Architectural Design
[MessageQueue_V1](https://drive.google.com/file/d/19TWJBxTqIwX-nk3NQcdMmgxa18oyShT9/view?usp=drive_link).

## CLI usage

Currently, we support these commands:

| Command                                               | Functionality                                            |
| ----------------------------------------------------- | -------------------------------------------------------- |
| `topic create <topic_name>`                           | Create a topic with a given name                         |
| `topic populate <topic_name> <partition_key> <item>`  | Populate an item into partition of a topic               |
| `topic consume <topic_name> <partition_key> <offset>` | Consume an item at an offset from a partition of a topic |

To run the CLI application, first you have to run the server itself (see [System Requirements](#system-requirements)). Then run the CLI client:

```bash
cd example_cli_clients
cmake --preset normal
cd build && make
./MQClient localhost 10001
```

You can now type in the commands above to interact with the MQ server.

## Functionalities checklist
These are the functionalities I have had in mind, inspired by [this video](https://www.youtube.com/watch?v=1ISRd0bS714):

- MessageQueue as TCP server
  - [x] Accepting TCP connections
  - [ ] Accepting *concurrent* TCP connections (more than 1 at a time)
- CLI applications
  - [x] Minimalistic CLI client
    - Forwards whatever command you type into it. I'd prefer using an SDK though, this is too tedious!
- APIs
  - Topic APIs
    - [x] Create a topic
  - Consumers APIs
    - [ ] Consumer subscribing to a topic
    - [x] Consuming item at given offset from a partition of a topic
  - Producers APIs
    - [x] Producing item to a partition within a topic
- SDK - the idea is that it can be used like the code example in [this](https://www.hellointerview.com/learn/system-design/deep-dives/kafka)
  - [ ] Consumers SDK
  - [ ] Producers SDK
- [ ] Persist data into a log file
- [ ] Message sending mechanisms (or, Delivery Guarantees):
  - [ ] At-Least-Once
  - [ ] At-Most-Once
  - [ ] Exactly-Once
- [ ] Rejection mechanism to avoid overloading
- [ ] Dead-lettered-queue
- [ ] Replica (fault tolerance)
- [ ] Message Replay mechanism
