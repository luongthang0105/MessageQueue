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

## Functional Requirements
Functional requirements are inspired by an overview of message queue [video](https://www.youtube.com/watch?v=1ISRd0bS714):
- [ ] APIs
  - [ ] Consumers APIs
    - When started developing, link this to the API section
  - [ ] Producers APIs
    - When started developing, link this to the API section
  - [ ] Communicating via a TCP connection..?
- [ ] System Architecture
  - [ ] Topics
  - [ ] Partitions
- [ ] Message sending mechanisms (or, Delivery Guarantees):
  - [ ] At-Least-Once
  - [ ] At-Most-Once
  - [ ] Exactly-Once
- [ ] Rejection mechanism to avoid overloading
- [ ] Dead-lettered-queue
- [ ] Replica (fault tolerance)
- [ ] Message Replay mechanism

## Non-functional Requirements

## APIs
Besides the Kafka server implementation, I'm also going to write a Kafka client library so producers/consumers can easily use it.
### Producer APIs
- connect to a Kafka server (broker)
- set the topic
- produce it
### Consumer APIs
- connect to a Kafka server
- set the topic
- choose partition
- consumes it