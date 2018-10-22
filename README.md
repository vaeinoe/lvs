# LVS — a video game single in 10:00

Match hexagonal tiles to play a little song. Keep focused to finish on time. Rewind and play again to beat your personal high score.

LVS is a small game / audiovisual toy developed during the Christmas holiday season 2013-2014. Download at https://vae.fi/lvs/

# Technical

The game was originally developed on Mac OS (Xcode) and should compile and run on various Windows versions (VC++). The project may also compile and work on iOS targets, but with audio and runtime resource allocation issues that were left unsolved during the original development period.

The audio resource files are not included in the repository. They can be found from the distribution packages if required.

## Libraries / dependencies

 - Game engine: *Cinder (libcinder)*
 - Audio engine: *Pure Data (libpd)*
 - Audio output: *Cinder / Portaudio (depending on build)*
 - Dynamics: *Chunkware simple dynamics limiter*
 - OGG decoding: *stb-vorbis*

# MIT License

Copyright (c) 2012-2013 Väinö Ala-Härkönen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
