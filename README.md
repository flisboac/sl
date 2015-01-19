
C Streams with per-stream locale and encoding.

Greatly inspired by C++ IOStreams, but geared towards C programmers' needs.

Abstracts various stream types into a common denominator, while allowing for
specializations to be discovered and used.

Favors reentrancy and avoids global states (but of course, they may be needed,
promises of pure reentrancy and/or thread safety are made per stream type).

That's it... The library itself is still going over a major brainstorming and
overhaul. Check `include/sl.h` for the public (user) API, and `include/sldefs.h`
for the internal (implementors') API.
