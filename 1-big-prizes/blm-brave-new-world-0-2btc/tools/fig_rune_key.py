#!/usr/bin/env python3
"""
fig_rune_key.py -- generates images/03-key-rune-script.png

Purpose:
    Render the key of the collage's geometric rune script: one exemplar glyph per
    letter, cropped from the published image at the pixel boxes recorded in
    data/rune-key.json, with the Cyrillic letter it stands for under each glyph.
    Column glyphs are rotated 90 degrees clockwise, which is the orientation the
    right-edge column is read in (bottom to top). Nothing here is drawn by hand.

Usage:
    python3 tools/fig_rune_key.py

Input:
    clues/welcome-to-the-brave-new-world.png (the published puzzle image)
    data/rune-key.json (exemplar boxes and letters)

Output:
    images/03-key-rune-script.png
"""

import json
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont, ImageOps

ROOT = Path(__file__).resolve().parent.parent
KEY_FILE = ROOT / "data" / "rune-key.json"
OUT_FILE = ROOT / "images" / "03-key-rune-script.png"

TILE_W, TILE_H, GLYPH_H, PER_ROW = 96, 122, 64, 10


def main() -> None:
    with open(KEY_FILE, encoding="utf-8") as f:
        spec = json.load(f)
    art = Image.open(ROOT / spec["image"]).convert("L")
    try:
        font = ImageFont.truetype("DejaVuSans-Bold.ttf", 26)
    except OSError:
        font = ImageFont.load_default()

    exemplars = spec["exemplars"]
    rows = (len(exemplars) + PER_ROW - 1) // PER_ROW
    canvas = Image.new("L", (TILE_W * PER_ROW, TILE_H * rows), 255)
    draw = ImageDraw.Draw(canvas)
    for n, ex in enumerate(exemplars):
        x0, y0, x1, y1 = ex["box"]
        pad = 2
        glyph = art.crop((x0 - pad, y0 - pad, x1 + pad, y1 + pad))
        if ex["rotate_clockwise"]:
            glyph = glyph.rotate(-90, expand=True)
        glyph = ImageOps.autocontrast(glyph, cutoff=2)
        scale = GLYPH_H / max(glyph.height, 1)
        glyph = glyph.resize((max(8, int(glyph.width * scale)), GLYPH_H), Image.LANCZOS)
        cx = (n % PER_ROW) * TILE_W
        cy = (n // PER_ROW) * TILE_H
        canvas.paste(glyph, (cx + (TILE_W - glyph.width) // 2, cy + 8))
        tw = draw.textlength(ex["letter"], font=font)
        draw.text((cx + (TILE_W - tw) / 2, cy + 82), ex["letter"], fill=0, font=font)

    OUT_FILE.parent.mkdir(exist_ok=True)
    canvas.save(OUT_FILE, format="PNG", optimize=True)
    print(f"wrote {OUT_FILE} ({OUT_FILE.stat().st_size} bytes, {canvas.size})")


if __name__ == "__main__":
    main()
