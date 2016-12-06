#! /usr/bin/ruby

require "~/bin/lock.rb"

#num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
#num_threads = [1, 4, 8, 12, 16]
num_threads = [1, 8]
#sizes = ["XS", "S", "M", "L", "XL"]
#sizes = ["S", "XL"]
sizes = ["S", "M", "L", "XL"]
#perf_groups = ["DATA", "MEM", "L3", "L2CACHE", "L2"]
perf_groups = ["MEM"]

#lock_file

if !File.exist?("bmt") then
  `make`
end

sizes.each do |size|
  num_threads.each do |nt|
    perf_groups.each do | pg |
      if nt == 1 then
        `likwid-perfctr -C S0:0 -g #{pg} ./bmt #{size} > #{size}_#{nt}_#{pg}.log`
      elsif nt == 8 then
        `likwid-perfctr -C S0:0-7 -g #{pg} ./bmt #{size} > #{size}_#{nt}_#{pg}.log`
      else
        count = nt / 2 - 1
        `likwid-perfctr -C S0:0-#{count}@S1:0-#{count} -g #{pg} ./bmt #{size} > #{size}_#{nt}_#{pg}.log`
      end
    end
  end
end

#unlock_file
