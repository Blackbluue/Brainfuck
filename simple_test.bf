,                 read input and place in Cell #0
>,                and Cell #1
<                 move back to cell #0
[                 block to add value of Cell #0 to Cell #2
    >>+           move to Cell #2 and add 1
    <<-           move back to Cell #0 and subtract 1
]                 keep going until Cell #0 is empty
>                 move to Cell #1
[                 block to add value of Cell #1 to Cell #2
    >+            move to Cell #3 and add 1
    <-            move back to Cell #1 and subtract 1
]                 keep going until Cell #1 is empty
>.                print value of cell #2
