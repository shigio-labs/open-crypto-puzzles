# Mechanism, in full

## The site

The puzzle lives on an approximately 70-page Wix site, almost all of it password-protected
page by page. The entry page (`/treasure-hunt`) carries an image whose EXIF metadata encodes
GPS coordinates; entering the latitude and then the longitude as consecutive page passwords
leads to a `who-is-she` page, and answering that opens a compass page, which displays four
branch passwords in clear text: `north64`, `south64`, `east64`, and `west64`. From there the
site forks into four branches.

The coordinate passwords are 8-digit strings, not the decimal or symbol-stripped forms the
EXIF viewer prints: latitude `27756932` and longitude `73511573`, i.e. the degrees, minutes
and seconds concatenated, the seconds carried to three decimals and rounded, no sign and no
separators (27 deg 7' 56.932" N, 73 deg 5' 11.573" W). This is the single detail that gates
the whole hunt, and it is why a correctly read coordinate still fails until the eighth digit
is appended. The `who-is-she` page answers to `amphitrite` (Poseidon's wife; the ship on the
entry page is named Poseidon), lowercase.

Everything up to and including the branch fork is solved, and the South branch is now solved
to its island fork (see below). I hold every password from the entry page through the compass
page and through every South page down to the three island-exit gates.

## The four branches

North is a decorative dead end: its chain of pages (ending in a "coming soon" page) carries
none of the 12 words. I confirmed this by reading the full content of every page on the
branch; no word-bearing artifact of any kind is present.

West and East each still end in one password-gated page I have not opened. South, which was
the third such lock, is now open and runs well past it. Each of these gates takes a short,
unenumerable string answering the riddle text quoted in
[clues/author-posts.md](../clues/author-posts.md), not anything hidden in an image or audio
file: no steganographic payload was found on any page along the way (see analysis/tested.md).

The South branch, in full. After `south64` the storm page `/south` shows two passwords in
clear text: "But I'm Strong and Woke Up the Next Day" = `electricfeel64` (the live path) and
"I Never Woke Up" = `777` (a heaven/hell dead branch, with `666` and `vampire` beyond it).
`electricfeel64` opens `/awake` ("There are now 6 others aboard your ship"), which begins a
chain of pages naming the seven castaways in sequence, each password a single Title Case name:
`Gilligan`, then `Jonas` (the Skipper, Jonas Grumby), `Thurston`, `Lovey`, `Ginger`, then the
"Name 6" gate `b3vye`, and finally `Mary Ann` on `/havingfunwiththeurl-ilovedthisshowasakid`.

The "Name 6" gate is the one the whole community was stuck on. Its answer is **`Dr. Roy`**
(with the period and the space): the Professor's real name is Roy Hinkley, and the page
rewards a different title ("Dr.") rather than "Professor", which is why every spelling of
"Professor" failed. This is the key correction: earlier I assumed `b3vye` was a single Title
Case token with no spaces and no punctuation, and that its password would also open everything
downstream. Both were wrong. The answer carries a period and a space, and every page after it
is separately gated (`Mary Ann`, then the island, then each of the three island exits).

`Mary Ann` opens the island page `/hereonnnnngiligansissssssland`: the castaways wash ashore,
survive on coconuts ("after a few years, they make you crazy"), and are offered three exits,
each its own locked gate:

- `/raft-escape` -- "Escape via raft, made from palm tree wood"
- `/smoke-signals` -- "Light a Fire to Alert a Passing Ship"
- `/death` -- "I think I'll just make a life for myself here" (the title implies a dead end)

`/raft-escape` and `/smoke-signals` are the live frontier. Both are unopened, and a public
solver reports firing roughly 250,000 guesses at the island exits with no hit, so these are
insight gates, not enumerable ones.

## Case sensitivity and format, established by direct test

Passwords on this site are case sensitive. I confirmed this on two already-open gates on the
South chain: the page password `Gilligan` succeeds where `gilligan` and `GILLIGAN` both fail,
and `Ginger` succeeds where `ginger` and `GINGER` both fail. Combined with the passwords
already known for every other open gate, the format of the three remaining locks is:

- South (`b3vye`): solved, `Dr. Roy` -- Title Case but with a period and a space, which breaks
  the "single token, no punctuation" assumption I had held for the three locks. The island
  exits after it (`/raft-escape`, `/smoke-signals`) have no confirmed format yet.
- West (`wt1jy`) and East (`c2ozw`): lowercase, a single token with no spaces (matches every
  other open West/East page password, e.g. `albatross`, `semaphore`, `20000leagues`).

No password anywhere on the site I have opened carries a numeric suffix that is not directly
derivable from the page's own content (a number visible in an image on that same page); no
guess should add an arbitrary digit string.

## The 12-word carrier channels

The site's predecessor, "Born to Be Wild" (2021, Apollo/moon theme), was solved and swept by
an unidentified third party; I used its known solution purely as a template for how this
author hides seed words, not as part of the live puzzle. Its seed was
`fortune all man kind one giant step into digital tomorrow virtual moon` with passphrase
`supernova`, assembled from 7 carrier channels. Mapping the same 7 channels onto this hunt:

| # | words | Hunt #1 channel | Hunt #2 equivalent | status here |
|---|---|---|---|---|
| 1 | word 1 | bytes appended after the cover image's EOF marker | Glimmer cover art | refuted: every Hunt #2 cover image I found (Bandcamp, Apple Music, Deezer, ToneDen, and the site's own PNG) is clean; a collage image that looked like a second Hunt #2 cover turned out to be a Hunt #1 teaser predating this hunt |
| 2-4 | words 2-4 | museum gold frames pictured in the book/site | not located outside a gated page | not found on any open page |
| 5-7 | words 5-7 | a forced cultural quotation naming a BIP39 word | pop-culture reference on the East branch | behind the locked `c2ozw` gate |
| 8-9 | words 8-9 | Morse code in an alternate audio mix | West branch "computer" theme or a hypothetical alternate mix | refuted for the public master: the only public 48kHz/24-bit Glimmer audio matches its own official master with no Morse signal; no alternate mix has been found distributed anywhere |
| 10 | word 10 | binary encoding | not located | the only binary string found anywhere on the site is an EXIF comment reading "nope lol", which does not decode to anything |
| 11-12 | words 11-12 | a "future song" ticket prop | inherited endgame page, already open | present, but this is Hunt #1's own endgame carried over, already read, and not new information |
| passphrase | "in the song" | the closing track's title | the Glimmer track title or lyric | not testable without the 12 words |

The practical conclusion: every carrier channel that is reachable without solving one of the
three insight locks has been checked and is either empty (refuted) or inherited scaffolding
from Hunt #1 that carries no new word. All 12 words are behind West, East, and South.
