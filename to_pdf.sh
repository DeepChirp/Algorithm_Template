#!/bin/bash
python3 merge_code.py && 
pandoc --from markdown --to latex output.md -o output.tex --template ./evisvogel.tex &&
xelatex output.tex &&
xelatex output.tex