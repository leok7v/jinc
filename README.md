jinc Java-in-C

This is old code. I miss immutable string with length and hashcode in C.
From time to time HashMap<String, String> or HashMap<String, Long> seems to be a bit of a help.
  
This is very simple (almost naive) implementation of immutable strings and even simple hash maps.
Dangerous as hell if you do *not* know what you are doing.
Uses: `alloca()` which will blow up your stack in the first `for(;;)` loop cycle.

It is not by any means high performance implemention of reference counted string.
It is good for 

    str_t filepath = str_stack_concat(filename, extension);

and something like:

    s2i_put("width", 1920);
    s2i_put("height", 1080);

pronounced [jiNGks]

*verb*
3rd person present: **jinks**
    change direction suddenly and nimbly, as when dodging a pursuer.

    "she was too quick for him and jinked away every time"

*noun*
plural noun: **jinks**
    a sudden quick change of direction.

Origin late 17th century (originally Scots as *high jinks*, denoting antics at
drinking parties): probably symbolic of nimble motion. Current senses date from
the 18th century.

"​JINKS - is what you say, when you and at least one other person,
simultaneously, say the same word(s). If you are first to say jinks, you are in
effect the jinks master, and the person who uttered the same word(s) and who is
now jinksed, cannot speak, until someone says their name in full. Uttering words
while jinksed however, will result in double jinks, triple jinks and so on and
so forth, for each additional word spoken."​
