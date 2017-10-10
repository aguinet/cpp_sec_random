CPP secure random
=================

This is a simple library that implements the random number generator interface
to provide a portable C++ secure random generator.

Compatibility
-------------

This is compatible with the following OSes:

* Debian-based linux systems
* Windows (starting with XP)
* OSX

Other Unix flavors might work but hasn't been tested.

Usage
-----

You can use this as any other library or simply copy/paste sec_random.{cpp,h}
into your project.

Motivations
-----------

``std::random_device`` is supposed to be a number generator that produces
non-deterministic random numbers. But the standard says that this can be
implemented as a pseudo-random engine if no hardware/kernel support is
provided. This is fine, but there is no way to check this at runtime.

This class simply throw an exception if such thing isn't available.

Implementation
--------------

This class is basically a wrapper around these APIs:

* under Unix and OSX, it reads random bytes from ``/dev/urandom``
* under Windows, it uses ``CryptGenRandom``

Improvements/idea
-----------------

Some cryptographic libraries use more than one source to generate cryptographic
secure random bytes. The OpenSSL ``sslrand`` man page is a good example
(https://linux.die.net/man/3/sslrand).
