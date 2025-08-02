# DPKVS

DPKVS stands for Distributed Persistent Key-Value Store. It is non-relational (NoSQL) database for storing key-value pairs.

Built with C++, so DPKVS is ⚡ blazingly fast ⚡ and 🔥 memory-unsafe 🔥

## Features

* Persistence using Append-Only Log and binary serialization
* Distributed (WIP)
* Master-Slave Replication (WIP)
* Data Partitioning using Consistent Hashing (WIP)
* Eventual Consistency (WIP)

### Persistence

As for now, data is stored in in-memory hash map. But every modification operation (`PUT` or `REMOVE`) is being written to Append-Only Log.

Every operation log consists of:

| Field          | Size              |
|----------------|-------------------|
| operation type | 1 byte            |
| key size       | 4 bytes           |
| key itself     | 1 byte * key_size |

Put operations also contain:

| Field                        | Size                |
|------------------------------|---------------------|
| value size                   | 4 bytes             |
| value itself                 | 1 byte * value_size |
| "has expiry" flag            | 1 byte              |
| expiry timestamp             | 8 bytes             |
| additional client-side flags | 4 bytes             |

Therefore, operation's size is:
* `5 + key_size` bytes for `REMOVE` operations
* `22 + key_size + value_size` bytes for `PUT` operations

For example, size of `put("hello", "world")` operation will be just 32 bytes.

It will be serialized to:

```shell
root:~$ xxd -b append-only-log.txt
00000000: 00000001 00000101 00000000 00000000 00000000 01101000  .....h
00000006: 01100101 01101100 01101100 01101111 00000101 00000000  ello..
0000000c: 00000000 00000000 01110111 01101111 01110010 01101100  ..worl
00000012: 01100100 00000000 00000000 00000000 00000000 00000001  d.....
00000018: 11010010 11011111 01001000 11110110 01101010 00111011  ..H.j;
0000001e: 00000110 00000000
```

## Testing

Code is being covered with unit and integration test using Google Test framework.

## Deploy

To be done
