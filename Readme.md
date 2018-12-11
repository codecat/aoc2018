# Advent of Code 2018
This is my Advent of Code repository for 2018. This is the first time I'm doing this and quite frankly, I don't really know what I'm doing, but I'm having fun doing it.

It's written in C/C++. I don't use STL, I use [scratch2](https://github.com/codecat/scratch2).

## Improvements
There's some improvements that could've been made. I don't really want to go back and change my solution after looking at other solutions, because that feels like I'm not being true to myself. Either way, here's some of my observations:

* Day 1 part 2 is really slow. There's tricks on making it faster, but I didn't really bother optimizing that part too much after submitting my answer.
* Day 4 is a mess. A cool and relatively organized mess, but still a mess. This could've been done with a lot less code, but I don't dislike my end result either way.
* Day 6 could've been sped up with a quad tree. I'm sure there are also other tricks that could've optimized it, but the execution time is acceptable either way. Part 2 is probably also not valid on every input given, considering I'm filling in the points in the region based on its left coordinate.
* Day 11 could've been optimized better. At the time of writing I wasn't sure how to do it any faster, so I just decided to go with the "dumb" approach. It still runs in 7 seconds on my machine which is acceptable for this kind of puzzle.
