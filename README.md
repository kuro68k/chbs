# chbs - XKCD 936 password generator

A simple app that takes a dictionary as input, and randomly selects words from it. By default 4 words are selected. Based on the concept shown in [XKCD 936](https://xkcd.com/936/)

The name is an acronym - Correct Horse Battery Staple.

Dictionaries, or wordlists, are simple ASCII text files. The EFF's sample wordlists are supplied in the correct format. Please remove any blank lines if you create your own.

[EFF Wordlists](https://www.eff.org/deeplinks/2016/07/new-wordlists-random-passphrases)

Thanks to Michael Thomas Greer for the [duthomhas::CSPRNG library](https://github.com/Duthomhas/CSPRNG), which this project makes use of to generate cryptographically secure random numbers.

### Usage

```
chbs [options] wordlist_file
Options:
  -c count <num_words>          Number of random words to select
  -d duplicates                 Allow words to be used more than once
  -v verbose                    Print extra information
  -r repetitions <repetitions>  Number of passwords to generate
```

### Building

Compiles with LLVM, eventually I'll get around to providing a makefile and test with GCC.