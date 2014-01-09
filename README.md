`pibuf` purpose is to record in memory the last (mega)bytes of an H.264 raw
stream produced by `raspivid`.

It is very similar to `tail -c` (which keep the last bytes of any stream),
except:
 * it keeps the H.264 headers
 * it ignores garbage until the next NAL

~~~
input:   hhp11111p22222p33p444444p555p6666p77
tail -c:                     XXXXXXXXXXXXXXXX
pibuf:   XX                  ....XXXXXXXXXXXX
~~~

with:
 * `h`: headers
 * `p`: NAL start_prefix (0x00000001)
 * `1`-`7`: NALs
 * `.`: garbage ignored
 * `X`: included in output

### Usage

    pibuf <size>

`k` and `m` size suffixes are accepted (respectively for Kb and Mb).

For instance:

    raspivid … | pibuf 10m > record.h264

`record.h264` can then be read by `omxplayer` (or `vlc`).
