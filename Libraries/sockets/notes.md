# Networking - sockets programming

## Types of sockets

* Raw sockets - TODO
* Stream sockets - `SOCK_STREAM`
  - Two-way connected communication stream - uses TCP (Transmission Control Protocol)
  - Used by telnet, http
* Datagram sockets - `SOCK_DGRAM`
  - Connectionless sockets - UDP (User Datagram Protocol)
  - Usually applications using UDP implement their own protocol
    on top of UDP to guarantee that all packets sent are received,
    i.e. they require some kind of ACK on the receiving and and will keep
    resending the packet until they get back the acknowledgement.


## Structures

socket descriptor - `int`

`struct addrinfo` - used to prep the socket structures for subsequent use, host name lookup, service name lookup
- loaded up with `getaddrinfo()`
```C++
struct addrinfo
{
  int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
  int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC, set AF_UNSPEC to support both ipv4 and ipv6
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
  char              sa_data[14];  // 14 bytes of protocol address; i.e. address in string format
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
Check ss_family for either `AF_INET` or `AF_INET6`, then cast to `sockaddr_in` or `sockaddr_in6`
```C++
struct sockaddr_storage {
  sa_family_t  ss_family;     // address family

  // all this is padding, implementation specific, ignore it:
  char      __ss_pad1[_SS_PAD1SIZE];
  int64_t   __ss_align;
  char      __ss_pad2[_SS_PAD2SIZE];
};
```

## IP addresses and getting them into/out of structures

Given an IP in string format - "10.12.110.57", or "2001:db8:63b3:1::3490", to store it into
`struct in_addr` or `struct in_addr6` can be done with `inet_pton()`.
`pton` stands for "presentation to network"

```C++
struct sockaddr_in sa;
struct sockaddr_in6 sa6;

// missing error-handling, returns -1 on error, 0 if the address is invalid, and >0 for success
inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr));
inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));
```

To convert from binary reprasentation into string format use `inet_ntop()`.
`ntop` stands for "network to presentation"

```C++
char ip4[INET_ADDRSTRLEN];    // space to hold a IPV4 string
struct sockaddr_in sa;        // assume this is loaded with data

inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
print("The IPV4 address is: %s\n", ip4);

// IPV6
char ip6[INET6_ADDRSTRLEN];   // space to hold IPV6 string
struct sockaddr_in6 sa6;      // assume this is loaded with data

inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

printf("The address is: %s\n", ip6);
```


## NAT - Network Address Translation
Done when a local network is hidden behind a firewall for purposes of protection. Translates an "internal" IP to "external" IP, that's recognisable in the world.


## getaddrinfo()

```C++
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
                const char *service,  // name of the service e.g. "http", or the port number
                const struct addrinfo *hints, // addrinfo filled with data
                struct addrinfo **res);
```

Setup for a server that listens on localhost port 3490
```C++
int status;
struct addrinfo hints;
struct addrinfo *servinfo;        // will point to the results

memset(&hints, 0, sizeof hints);  // make sure the struct is empty
hints.ai_family = AF_UNSPEC;      // don't care IPv4 or IPv6
hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
hints.ai_flags = AI_PASSIVE;      // fill in my IP for me

if ((status = getaddrinfo(nullptr, "3490", &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
}

// servinfo now points to a linked list of 1 or more struct addrinfos

// ... do everything until you don't need servinfo anymore ....

freeaddrinfo(servinfo); // free the linked-list
```

Setup for a client that wants to connecto to a particular server on a port
```C++
int status;
struct addrinfo hints;
struct addrinfo *servinfo;        // will point to the results

memset(&hints, 0, sizeof hints);  // make sure the struct is empty
hints.ai_family = AF_UNSPEC;      // don't care IPv4 or IPv6
hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets

// get ready to connect
status = getaddrinfo("www.example.net", "3490", &hints, &servinfo);

// servinfo now points to a linked list of 1 or more struct addrinfos

// etc.
```


## Sockets

```C++
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol); 

// domain - PF_INET, or PF_INET6
// type - SOCK_STREAM, or SOCK_DGRAM
// protocol - `0` to choose the proper protocol for the given type, or getprotobyname() to lookup the protocol
//            you want explicitly - "tcp" or "udp"
```

This data should be filled in from the call to getaddrinfo().
`socket` returns a socket descriptor or -1 on error;
```C++
int s;
struct addrinfo hints, *res;

// do the lookup
// [pretend we already filled out the "hints" struct]
getaddrinfo("www.example.com", "http", &hints, &res);

// again, you should do error-checking on getaddrinfo(), and walk
// the "res" linked list looking for valid entries instead of just
// assuming the first one is good (like many of these examples do).
// See the section on client/server for real examples.

s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
```

`bind()` system call
- associates socket wiht a port,
- commonly done for servers that will `listen()` for incomming connections on a port

```C++
#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
```

Creating a socket and binding it to a port
```C++
struct addrinfo hints, *res;

// first, load up address structs with getaddrinfo():

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

// nullptr is used in combination with AI_PASSIVE to automatically infer the local address
// Alternatively the address could be passed in here directly
getaddrinfo(nullptr, "3490", &hints, &res);

// make a socket:

int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// bind it to the port we passed in to getaddrinfo():

bind(sockfd, res->ai_addr, res->ai_addrlen);
```

If `bind()` fails with a "Address already in use" it's likely due to a socket left open in the kernel.
As long as there's no connection open it will clear up in ~minute.
It might be better to add the code to allow your program to reuse the socket

```C++
int yes=1;
//char yes='1'; // Solaris people use this

// lose the pesky "Address already in use" error message
if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    perror("setsockopt");
    exit(1);
} 
```


`connect()` is used to connecto to an address on a port via a socket.
```C++
#include <sys/types.h>
#include <sys/socket.h>

int connect(int sockfd, struct sockaddr *serv_addr, int addrlen); 
```

Example of connecting to an address on port 3490
```C++
struct addrinfo hints, *res;

// first, load up address structs with getaddrinfo():

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;

getaddrinfo("www.example.com", "3490", &hints, &res);

// make a socket:

int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// connect!

connect(sockfd, res->ai_addr, res->ai_addrlen);
```


`listen()` - wait for incomming connections
```C++
int listen(int sockfd, int backlog);
```
`backlog` is a number of incomming connections allowed to wait until they're `accept()`'ed. This is likely ~20 by default, and should be possible to set lower to 5-10.


`accept()` - processes a pending connection, returning a new socket descriptor that can be `send()` and `recv()` on
```C++
#include <sys/types.h>
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); 
```

Complete `listen()` - `accept()` example (no error handling though)
```C++
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

int main(void)
{
struct sockaddr_storage their_addr;
socklen_t addr_size;
struct addrinfo hints, *res;

// !! don't forget your error checking for these calls !!

// first, load up address structs with getaddrinfo():

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

getaddrinfo(NULL, MYPORT, &hints, &res);

// make a socket, bind it, and listen on it:

int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
bind(sockfd, res->ai_addr, res->ai_addrlen);
listen(sockfd, BACKLOG);

// now accept an incoming connection:

addr_size = sizeof their_addr;
int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

// ready to communicate on socket descriptor new_fd!
```

## `send()` and `recv()`
Communication over stream sockets or connected datagram sockets.

```C++
int send(int sockfd, const void *msg, int len, int flags);
const char *msg = "Hello, Sockets!";
int len = strlen(msg);
int bytes_sent = send(sockfd, msg, len, 0);
```
- send data `msg` of length `len` to the socket descriptor `sockfd`
- returns the number of bytes actually sent - this may be less than requested. In such a case the remainder needs to
  be handled and sent later.


```C++
int recv(int sockfd, void *buf, int len, int flags);
```
- reads data from socket `sockfd` into buffer `buf`, at most length `len`
- returns the number of bytes received
  - -1 on error
  - 0 if the other end has closed the connection
