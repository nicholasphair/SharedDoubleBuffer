### Shared Double Buffer

A thread-safe implementation of a queue that can be used in a producer-consumer
model.

The SharedDobuleBuffer is composed of 2 underlying queues, 1 for reading and 1
for writing. These are abstracted away as an implementation detail.
If the consumer calls pop and the read queue is empty a swap is made so that the
read queue becomes the write queue and visa-versa. Popping is non-blocking.
If the producer pushes onto a full queue, it blocks until room is made by a
consumers pop call.


TODO: Make templated.
