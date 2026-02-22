# NEXO Financial Application for Terminals (FAT) implemented in C++

Here it is assumed that reader has prior knowledge of terminology and use-cases.
A great place to get introduced is at [Nexo](https://www.nexo-standards.org/).
If HAP or SCAP doesn't mean anything to you then please read the specification
first.

## Build instructions

Currently only Linux is supported, some work was done to be able to compile on
Windows, but it isn't yet stable, [Cygwin](https://cygwin.com/) or
[MinGW](https://www.mingw-w64.org/) builds should work though, but due to lack
of time they aren't frequently tested.

### Preconditions

Install [drakon-editor](https://drakon-editor.sourceforge.net/), and make sure
that `drakon-gen` location is in your `$PATH` environment variable.

### On Linux

```sh
make all
```

That command should suffice, it would produce `nexoid-cpp` executable that you
can immediately run, but it wouldn't do much because it just implements FAST, for
a full payment application you need SCAP and there are currently 2 implementations:

  * Web based [nexui](https://github.com/hurufu/nexoid-nexui-flask) which is
    limited to CNP transactions and mostly serves as a demo and debugging tool.
  * Implemented in C for embedded use in some hardware terminals (list of
    supported brands and models is undisclosed now) with an ambition to be real
    SCAP for real payment terminals. Open Sourcing is quite tricky, because of
    NDA. It shall be done in such a way that doesn't upset IP holders.

Also you will need HAP and TMAP for a complete suite, but neither is currently
implemented and you will have to write bindings to your vendors L2 EMV kernels
too (for SCAP).

#### Cross-compilation

If you have a working SDK, just define a `$TRIPLET` for your target platform
(like `arm-linux-gnueabihf`) and the rest should just work the same:

```sh
make all CC=$TRIPLET-cc CXX=$TRIPLET-c++
```

### Integration

`nexoid-cpp` implements a network-oriented trusted layer around `nexoid-ed`
business logic. What it does is it just calls `Main()` from `nexoid-ed` and then
redirects all external calls to either RPC to SCAP, HAP, TMAP, etc or calls
custom code, while checking if data is consistent and nothing suspicious isn't
happening. That's basically all!

Most of the time integration should be quite trivial (the hardest part is SCAP,
because it has to use a lot of vendor specific code for EMV, PIN entry,
cryptography etc.) In many cases you can even run it locally on your laptop and
connect to SCAP on the real terminal (this is actually how do I develop).

You will have to implement protocol from [protocol-collection](https://github.com/hurufu/nexoid-protocol-collection)
and connect to `nexoid-cpp` via [NNG](https://nng.nanomsg.org/) PUB/SUB mechanism.
