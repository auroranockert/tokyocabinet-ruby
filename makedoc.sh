#! /bin/sh

LANG=C
LC_ALL=C
PATH="$PATH:/usr/local/bin:$HOME/bin:.:.."
export LANG LC_ALL PATH

rm -rf doc

rdoc --title "Tokyo Cabinet" -o doc tokyocabinet-doc.rb
