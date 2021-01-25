# Networking - sockets programming

## Types of sockets

* Raw sockets - TODO
* Stream sockets - SOCK_STREAM
  - Two-way connected communication stream - uses TCP (Transmission Control Protocol)
  - Used by telnet, http
* Datagram sockets - SOCK_DGRAM
  - Connectionless sockets - UDP (User Datagram Protocol)
  - Usually applications using UDP implement their own protocol
    on top of UDP to guarantee that all packets sent are received,
    i.e. they require some kind of ACK on the receiving and and will keep
    recending the packet until they get back the acknowledgement.


## Structures

socket descriptor - int

`struct addrinfo` - used to prep the socket structures for subsequent use, host name lookup, service name lookup
- loaded up with `getaddrinfo()`
```C++
struct addrinfo
{
  int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
  int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
  int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
  int              ai_protocol;  // use 0 for "any"
  size_t           ai_addrlen;   // size of ai_addr in bytes
  struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
  char            *ai_canonname; // full canonical hostname

  struct addrinfo *ai_next;      // linked list, next node
};
```

`sockaddr` - holds socket address information
```C++
struct sockaddr {
  unsigned short    sa_family;    // address family, AF_xxx
  char              sa_data[14];  // 14 bytes of protocol address
};
```

`sockaddr_in` - used to avoid packing the `sa_data` in `sockaddr` manually
```C++
struct sockaddr_in {
  short int          sin_family;  // Address family, AF_INET
  unsigned short int sin_port;    // Port number
  struct in_addr     sin_addr;    // Internet address
  unsigned char      sin_zero[8]; // Same size as struct sockaddr - padding, memset() to all zeros
};
```

```C++
struct in_addr {
  uint32_t s_addr; // that's a 32-bit int (4 bytes) in network byte order
};
```

Same, but for IPv6

```C++
struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port number, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};

struct in6_addr {
    unsigned char   s6_addr[16];   // IPv6 address
};
```

`sockaddr_storage` - large enough to hold IPv4 and IPv6, can be cast to either at runtime.
```C++
struct sockaddr_storage {
  sa_family_t  ss_family;     // address family

  // all this is padding, implementation specific, ignore it:
  char      __ss_pad1[_SS_PAD1SIZE];
  int64_t   __ss_align;
  char      __ss_pad2[_SS_PAD2SIZE];
};
```
