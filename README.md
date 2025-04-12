# SwiftNet++

**SwiftNet++** is a lightweight, C-based networking library that provides both client-server and peer-to-peer (P2P) communication over a custom protocol. It is designed to be efficient, modular, and easily embeddable in other C projects. SwiftNetPP supports connection handling, peer communication, and optional AI-driven protection mechanisms.

---

## Features

- 🚀 Minimal Client-Server Networking
- 🔁 Peer-to-Peer (P2P) Communication
- 🧠 AI-based Traffic Protection
- ⚙️ Custom Protocol Architecture (SNPP)
- 🧩 Modular Design with Header/Source Separation
- 🔄 Cross-platform Build (Makefile & CMake support)

---

## Getting Started

---

## 🧠 Example Usage

### 🖥️ Server Example

```c
#include "snpp/net/Host.h"

int main() {
    SNPP_HostConfig config = {
        .address = {.ip = "127.0.0.1", .port = 8080},
        .maxConnections = 10,
        .enableAIProtection = true
    };

    SNPP_Host* host = snpp_host_create(&config);
    if (!host) {
        printf("Failed to start host.\n");
        return 1;
    }

    printf("Server running on 127.0.0.1:8080...\n");

    // Add event loop / peer management here

    snpp_host_destroy(host);
    return 0;
}
```

---

### 💻 Client Example

```c
#include "snpp/net/Client.h"

int main() {
    SNPP_Client* client = snpp_client_create();
    if (!client) {
        printf("Failed to create client.\n");
        return 1;
    }

    printf("Client initialized.\n");

    // Connect to host, send/receive packets

    snpp_client_destroy(client);
    return 0;
}
```

---

### 🔄 Peer-to-Peer Example

```c
#include "snpp/net/Peer.h"

// Assume peer object is obtained from host connection handling

void on_peer_connect(SNPP_Peer* peer) {
    if (peer) {
        printf("Connected to peer: %d\n", snpp_peer_get_id(peer));

        // Send or receive data
        const char* message = "Hello Peer!";
        snpp_peer_send(peer, message, strlen(message));
    }
}
```

---

## 📁 Project Structure

```
SwiftNetPP/
├── include/snpp/
│   ├── net/              # Networking API (Client, Host, Peer, Endpoint)
│   ├── ai/               # AI protection headers
│   └── utils/            # Utility headers
├── src/
│   ├── net/              # Networking implementation
│   ├── ai/               # AI logic
│   └── utils/            # Allocators, helpers
├── Makefile              # Makefile support
├── CMakeLists.txt        # CMake build
```

---

## 📚 Core API Overview

Here are some of the key functions provided by SwiftNet++:

### 🖥️ Host (Server)

- `SNPP_Host* snpp_host_create(const SNPP_HostConfig* config);`  
  Initializes a host/server with specified IP, port, max connections, and optional AI protection.

- `void snpp_host_destroy(SNPP_Host* host);`  
  Cleans up the host and all its connections.

- `SNPP_Peer* snpp_host_accept(SNPP_Host* host);`  
  Accepts incoming connections and returns a `SNPP_Peer` instance.

---

### 💻 Client

- `SNPP_Client* snpp_client_create();`  
  Allocates and initializes a client instance.

- `void snpp_client_destroy(SNPP_Client* client);`  
  Cleans up client resources.

- `int snpp_client_connect(SNPP_Client* client, const char* ip, uint16_t port);`  
  Connects the client to a remote host.

---

### 🔄 Peer (P2P)

- `int snpp_peer_send(SNPP_Peer* peer, const void* data, size_t size);`  
  Sends raw data to a peer.

- `int snpp_peer_receive(SNPP_Peer* peer, void* buffer, size_t maxSize);`  
  Receives data from a peer (non-blocking or event-driven expected).

- `int snpp_peer_get_id(SNPP_Peer* peer);`  
  Returns a unique identifier for the connected peer.

---

## 🚧 Project Status

> ⚠️ **This project is currently under development.**  
> Some features are experimental, and the implementation may contain bugs, incomplete logic, or unstable behavior.  
> Please use it carefully in production environments and consider contributing to improve stability.

---

## 📬 Feedback and Contributions

If you encounter issues or have suggestions, feel free to open an issue or submit a pull request.
