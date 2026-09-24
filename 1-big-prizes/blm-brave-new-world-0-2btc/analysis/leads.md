# Open leads, ranked

## 1. Log the 3 already-launched btcrecover runs (minutes, free)

Runs 1 to 3 (499,000,000 raw candidates; 274,000,000 close-word and typo variants of the 4
refuted anchors; 880,000 candidates crossed with 50 thematic passphrases) were prepared and
launched against a pipeline whose own known-answer test passes, but the result of each run was
never written down. This is the fastest possible action: rerun each and record whether the
tool printed a match. Confirms: any match ends the puzzle. Kills: a clean run on all 3
confirms these particular candidate sets carry no signal, without spending new compute.

## 2. Re-derive the word inventory from the image itself, not from community claims (hours)

Every large campaign to date, including the 3.75-million-seed anchor-free sweep, still forces
a fixed pool of "obviously visible" words rather than a pool built by a fresh, systematic pass
over the full image. The high-resolution crops needed for this already exist; what has not
been done is a first-principles relisting of every candidate word on the collage, independent
of the word lists used in past runs (several of which trace back to the 2 unconfirmed Reddit
accounts). Confirms: a word not present in any prior candidate pool turns up and, combined with
the others, derives the target. Kills: a systematic relisting reproduces the same pool already
tested, closing this as a source of new candidates.

## 3. Cross the rune transcription against the Russian-prose cipher key (minutes, free)

The 85-glyph positioned transcription of the geometric rune script (3 locations on the image)
was completed on 2026-08-02. The hypothesis that this script decodes as Russian-language prose
was developed earlier and independently. The two have never been directly cross-checked against
each other, because the positioned transcription did not exist until the later pass. Either the
decoded prose reads coherently start to finish, which would settle the rune channel as
non-seed-bearing with certainty rather than high confidence, or a mismatch would reopen the
cipher as a candidate word source. The image's pedestal and bottom band remain untranscribed, so
even a coherent result would not close the glyph inventory completely.

## 3, done 2026-09-24: the rune script is read in full

The 3 inscriptions decode to Russian prose under one key (`analysis/runes.md`,
`data/rune-key.json`): a joke about donations to the escrow, the statement "here bitcoins
are encrypted for a black day, number 1", and, under the dial, "the sum of two numbers".
The rune channel is closed as a direct word source and reopened as the source of a reading
rule, lead 5 below.

## 4. Settle BIP39 versus old-Electrum from a source, not from more derivation (needs new
information)

The format fork is the single choice that would cut the remaining search space roughly in half.
The puzzle author posted once, in 2020, and has not been heard from since; no known-answer
message signature exists. The only path I have not exhausted is a further pass over 2025 to
2026 Reddit and BitcoinTalk activity for any post that quotes or references a hint from the
author directly, as opposed to a poster's own guess.

## 5. The dial reading rule: the sum of two numbers, and a phrase longer than 12 words (insight, then bounded compute)

The line under the dial reads "сумма двух чисел", the sum of two numbers. Each hand of the
dial points between two numerals, not at one: TOWER between 1 and 2, MOON between 12 and 1,
the short unlabeled hand between 10 and 11 (measured 2026-09-24 on the published image,
consistent with the fractional positions 1.48 and 0.54 recorded in `analysis/tested.md`).
Summing the two numerals gives TOWER 3, MOON 13, unlabeled hand 21. "Tower at position 3,
moon at position 13" are the two anchors the community used from 2020 and that this folder
retracted as untraceable; they follow from the author's own rule and are reinstated as
author-derived, with a third value, 21, that nobody has used.

A position 13 or 21 does not exist in a 12-word phrase. Every sweep in `analysis/tested.md`
assumed 12 words. What would confirm this lead: a reading of the collage that assigns
positions to more than 12 words with the same rule (other pairs of numbers on the image:
"1865-202", "05.25.20", "11.03.20", "Section 1", the dial), and a derivation of that phrase
that matches the escrow. What would kill it: a demonstration that the phrase is fixed at 12
words by the author, which no published statement gives. Cost: the reading is hours; a
24-word phrase built from the collage's own vocabulary with fixed positions is seconds to
test through `tools/oracle.py --stdin` once it exists, and the oracle needs a 24-word mode
first (it accepts 12 words today).
