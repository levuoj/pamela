#!/usr/bin/ruby -w

require 'fileutils'

if Process.uid != 0
  STDERR.puts "Error you must be root"
  exit (false)
end

array = ["auth", "session", "password"]

array.each { | file |
  File.open('tmp', 'w+') do |out|
    File.foreach("/etc/pam.d/common-#{file}") do |line|
      out.puts line unless line.include?("pam_pamela.so")
    end
  end
  FileUtils.mv("tmp", "/etc/pam.d/common-#{file}")
}

FileUtils.rm("/lib/i386-linux-gnu/security/pam_pamela.so")

puts "OK"
