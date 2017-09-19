#! /usr/bin/ruby


num_threads = [1]
#num_threads = [1, 8]
sizes = ["S"]
#sizes = ["S", "M", "L", "XL"]

sizes.each do |size|
  num_threads.each do |nt|
    'export OMP_NUM_THREADS=#{num_threads}'
    `Exana -- ./bmt #{size} > log.#{size}.#{nt}`
  end
end
