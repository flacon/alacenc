alacenc
=========
alacenc - encode audio into the Apple Lossless Audio Codec (ALAC) format

Usage:
------
```
Usage:
  alacenc [options] [--] <INPUT_FILE> <OUTPUT_FILE>

Arguments:
  INPUT_FILE        Input file name,
                    when INPUT_FILE is -, read standard input
  OUTPUT_FILE       Output ALAC file name,
                    when OUTPUT_FILE is -, write to standard output

Options:
  -q --quiet               Produce no output to stderr
  -h --help                Print this help text and exit
  -V --version             Print the version number
  -f --fast                Fast mode. Encode a channel pair without
                           the search loop for maximum possible speed
  --artist=<value>         Set artist name
  --album=<value>          Set album/performer name
  --albumArtist=<value>    Set album artist name
  --title=<value>          Set title/track name
  --comment=<value>        Set comment
  --genre=<value>          Set genre
  --year=<value>           Set year
  --songWriter=<value>     Set song writer name
  --group=<value>          Set group name
  --lyrics=<value>         Set lyrics
  --compilation            Set track as part of a compilation
  --track=<number/total>   Set track number
  --disc=<number/total>    Set disc number
  --cover=<file>           Set disc cover from file. The program supports
                           covers in JPEG, PNG and BMP formats.
```
