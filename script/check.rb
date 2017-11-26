#!/usr/bin/ruby -w

require 'fileutils'

if Process.uid != 0
  STDERR.puts "Error you must be root"
  exit (false)
end

array = ["/etc/pam.d/common-auth", "/etc/pam.d/common-session", "/etc/pam.d/common-password"]
bool = true

array.each { | file |
  if File.open(file).each_line.any? { |line| line.include?('pam_test.so') }
    puts "file #{file} : OK"
  else
    puts "Error in file #{file}"
    bool = false
  end
}

if File.file?("/lib/i386-linux-gnu/security/pam_pamela.so")
  puts "library emplacement : OK"
else
  puts "Error library emplacement must be /lib/i386-linux-gnu/security"
  bool = false
end

if bool == false
  STDERR.puts "checking : KO"
  exit(false)
end

puts "checking : OK"
