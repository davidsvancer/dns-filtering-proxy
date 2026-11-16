# DNS Filtering Proxy

A lightweight DNS filtering proxy written in C. The application receives DNS queries from clients, checks requested domains against a configurable blocklist, forwards permitted queries to an upstream DNS server, and relays the response back to the client.

The project was developed as a university networking assignment and implements DNS packet handling directly using POSIX sockets rather than relying on a DNS library.

## Features

- **Domain filtering** using a configurable text file of blocked hostnames.
- **Subdomain filtering** — a blocked domain also matches its subdomains (for example, blocking `example.com` also blocks `ads.example.com`).
- **DNS packet parsing and construction**, including DNS headers and question sections.
- **DNS name decompression**, including compression pointers.
- **A and AAAA query support.**
- **IPv4 and IPv6 client communication** using a dual-stack UDP listener.
- **Upstream DNS forwarding** to a configurable server.
- **Concurrent request handling** using POSIX threads (`pthread`).
- **Verbose logging** for inspecting request processing.
- **Integration tests** using `dig`, with an optional `dnsperf` stress test.

## How It Works

```text
                    DNS query (UDP)
       ┌───────────────┐
       │  DNS client   │
       └───────┬───────┘
               │
               ▼
       ┌──────────────────────┐
       │  DNS filtering proxy│
       │  - parse packet     │
       │  - validate query   │
       │  - check blocklist  │
       └──────────┬───────────┘
                  │
          ┌───────┴────────┐
          │               │
     Blocked/invalid   Allowed query
          │               │
          ▼               ▼
   Refusal/error   ┌─────────────────┐
                   │ Upstream DNS    │
                   │ server (UDP)   │
                   └────────┬────────┘
                            │ response
                            ▼
                     DNS filtering proxy
                            │
                            ▼
                        DNS client
```

At a high level, the application:

1. Receives a DNS request over UDP.
2. Parses the DNS header and question.
3. Checks whether the query type is supported and whether the domain is blocked.
4. Creates a response for requests that should not be forwarded, or sends an allowed request to the configured upstream DNS server.
5. Receives the upstream response and forwards it to the original client.

## Domain Blocklist

The blocklist is a text file containing one hostname per line. The file is supplied with the `-f` option.

Example:

```text
360yield.com
ads.example.com
tracking.example.org
```

Domain matching is case-insensitive. A blocked hostname also matches its subdomains, while a hostname that merely contains the blocked string does not match unless it is separated by a domain label boundary.

## Build

### Requirements

- GCC or a compatible C compiler
- GNU Make
- POSIX-compatible environment with sockets and pthreads
- `dig` for integration tests
- Optional: `dnsperf` for the stress test

### Compile

Build the executable from the project root:

```bash
make
```

The resulting executable is named `dns`.

The Makefile uses C11 and enables strict compiler warnings (`-Wall -Wextra -Werror -pedantic`).

### Clean build artifacts

```bash
make clean
```

## Run

The command-line syntax is:

```text
dns -s server [-p port] -f filter_file [-v]
```

| Option | Description |
|---|---|
| `-s server` | Upstream DNS server IP address or hostname (required). |
| `-p port` | Local listening port. Defaults to `53`. |
| `-f filter_file` | Path to the blocklist file (required). |
| `-v` | Enable verbose logging. |
| `-h`, `-help` | Print usage information. |

Example:

```bash
./dns -f tests/domeny.txt -s 8.8.8.8 -p 53530 -v
```

The proxy listens on the selected local port. Use a non-privileged port such as `53530` for local development; binding to port `53` may require elevated privileges depending on the operating system.

## Testing

The project includes a shell-based integration test suite in `tests/test_complex.sh`.

Start the proxy in one terminal:

```bash
./dns -f tests/domeny.txt -s 8.8.8.8 -p 53530
```

Then, from the project root, run the tests in another terminal:

```bash
make test
```

The test script uses `dig` and checks examples including:

- allowed and blocked domains,
- blocked subdomains,
- A and AAAA queries,
- unsupported query types,
- invalid and nonexistent domains,
- IPv4 and IPv6 client transport,
- selected edge cases.

If `dnsperf` is installed, the script also runs an optional short stress test. The integration tests require a running proxy and access to the configured upstream DNS server.

## Project Structure

```text
.
├── include/                 # Header files
├── src/                     # C implementation
│   ├── arg_parse.c          # Command-line argument parsing
│   ├── dns.c                # Main application and socket setup
│   ├── dns_packet_parse.c   # DNS packet parsing
│   ├── dns_packet_create.c  # DNS packet construction
│   ├── thread.c              # Worker request handling
│   ├── utils.c               # Shared DNS and utility functions
│   ├── linked_list.c          # Linked-list implementation
│   ├── dynamic_array.c        # Dynamic-array/string utilities
│   └── log.c                  # Logging
├── tests/
│   ├── test_complex.sh        # Integration tests
│   └── domeny.txt             # Example blocklist
├── Makefile
└── manual.pdf                 # Project specification/documentation
```

## Learning Outcomes

The project provided practical experience with:

- C programming and manual memory management.
- Binary protocol parsing and serialization.
- DNS headers, questions and compressed domain names.
- IPv4/IPv6 addressing and POSIX UDP sockets.
- Multithreaded request processing with pthreads.
- Data structures such as linked lists and dynamic arrays.
- Integration testing of network services.

---

*Educational project developed for a university networking course.*
