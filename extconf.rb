require "mkmf"

dir_config('tokyocabinet')

home = ENV["HOME"]
ENV["PATH"] = ENV["PATH"] + ":/usr/local/bin:$home/bin:.:..:../.."
tccflags = `tcucodec conf -i`.chomp
tcldflags = `tcucodec conf -l`.chomp
tcldflags = tcldflags.gsub(/-l[\S]+/, "").strip
tclibs = `tcucodec conf -l`.chomp
tclibs = tclibs.gsub(/-L[\S]+/, "").strip

$CFLAGS = "-I. -I.. -I../.. #{tccflags} -Wall #{$CFLAGS} -O2"
$LDFLAGS = "#{$LDFLAGS} -L. -L.. -L../.. #{tcldflags}"
$libs = "#{$libs} #{tclibs}"

if have_header('tcutil.h')
  create_makefile('tokyocabinet')
end
