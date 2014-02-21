# ring-buffer-ipc

## DESCRIPTION

ring_buffer.h - a simple implementation of a ring buffer based on boost shared_memory_object.

producer.cpp - a producer implementation endlessly writing random numbers to the ring buffer.
    Blocks after detecting that the buffer is full. Current implementation has support
    for only 1 producer at a time.

consumer.cpp - consumer implementation. It consumes 1 value from the ring buffer and exits.
    May be re-launched multiple times. Also concurrent consumer executions are supported.

## USAGE

1. Start the producer.
2. Start any number of consumer processes.
3. Observe :)
