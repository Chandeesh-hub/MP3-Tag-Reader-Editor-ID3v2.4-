 # MP3 Tag Reader & Editor (ID3v2.4)

A C command-line utility to **view** and **edit** ID3v2.4 metadata tags embedded in MP3 files — Title, Artist, Album, Year, Composer, and Comment — without touching the audio content.

## Overview

MP3 files store song metadata (Title, Artist, Album, Year, Genre/Composer, Comments, etc.) inside ID3v2 tag frames near the start of the file. This tool lets you:

- **View** all supported tags in a given MP3 file.
- **Edit** a single tag (e.g., replace the Title or Artist) while preserving all other tag data and the audio stream untouched.
- See a **help menu** when invalid or missing arguments are provided.

## Project Structure

```
.
├── main.c        # Entry point, argument routing (view/edit/help), usage menu
├── tagreader.c    # Core logic: validation, reading, displaying, and editing tags
├── header.h       # Struct/enum definitions and function prototypes
└── a.out          # Compiled binary (optional, can be rebuilt)
```

## Supported Tags

| Flag | Tag Frame | Description |
|------|-----------|-------------|
| `-t` | `TIT2`    | Title        |
| `-T` | `TPE1`    | Artist       |
| `-A` | `TALB`    | Album        |
| `-Y` | `TYER`    | Year         |
| `-c` | `TCON`    | Composer     |
| `-C` | `COMM`    | Comment      |

## Building

Compile using `gcc`:

```bash
gcc main.c tagreader.c -o a.out
```

This produces an executable named `a.out`.

## Usage

### View mode

Displays all tag details (Title, Artist, Album, Year, Composer, Comment) from an MP3 file:

```bash
./a.out -v <file.mp3>
```

**Example:**
```bash
./a.out -v sample.mp3
```

### Edit mode

Replaces a single tag's value with new data:

```bash
./a.out -e <tag_option> <new_data> <file.mp3>
```

**Example:** Change the Title tag:
```bash
./a.out -e -t "New Song Title" sample.mp3
```

### Help

```bash
./a.out -help
```

Or simply run the program with no arguments to see the usage menu.

## How It Works

1. **Argument validation** — `main.c` determines whether the operation is `view`, `edit`, or invalid, based on the first argument (`-v`, `-e`, or `-help`).
2. **File validation** — `tagreader.c` checks that the given file has a valid `.mp3` extension and isn't hidden/empty before opening it.
3. **View flow**
   - Skips the 10-byte ID3v2 header.
   - Iterates through each 10-byte frame header (4-byte tag name + 4-byte size + 2-byte flags).
   - Converts the big-endian size field to little-endian and reads/prints the tag's text content.
4. **Edit flow**
   - Opens the source file and creates a `temp.mp3` file.
   - Copies the 10-byte ID3v2 header as-is.
   - Walks through each tag frame: if it matches the tag the user wants to edit, writes the new data (with an updated size field); otherwise copies the existing frame unchanged.
   - Copies any remaining audio data byte-by-byte.
   - Deletes the original file and renames `temp.mp3` to the original filename.

## Notes / Limitations

- Only the six tags listed above are recognized; other ID3v2.4 frames are not parsed.
- The tool assumes a well-formed ID3v2.4 header and frame layout; malformed or missing tags may produce unexpected output.
- Editing rewrites the file in place via a temporary file — ensure you have a backup of important MP3 files before editing.

## Author

Chandeesh K M
