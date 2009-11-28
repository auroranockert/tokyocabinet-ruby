require 'rubygems'

spec = Gem::Specification.new do |s|
  s.name = "tokyocabinet"
  s.version = "1.29"
  s.author "Mikio Hirabayashi"
  s.email = "mikio@users.sourceforge.net"
  s.homepage = "http://tokyocabinet.sourceforge.net/"
  s.summary = "Tokyo Cabinet: a modern implementation of DBM (This is a MacRuby only version for personal, please don't send bugs reports to Mikio if you use this)."
  s.description = "Tokyo Cabinet is a library of routines for managing a database.  The database is a simple data file containing records, each is a pair of a key and a value.  Every key and value is serial bytes with variable length.  Both binary data and character string can be used as a key and a value.  There is neither concept of data tables nor data types.  Records are organized in hash table, B+ tree, or fixed-length array."
  s.files = [ "tokyocabinet.c", "extconf.rb" ]
  s.require_path = "."
  s.extensions = [ "extconf.rb" ]
end

if $0 == __FILE__
  Gem::manage_gems
  Gem::Builder.new(spec).build
end
