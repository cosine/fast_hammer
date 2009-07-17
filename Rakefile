
require 'fileutils'

file 'Makefile' => 'extconf.rb' do |t|
  sh 'ruby', t.prerequisites[0]
end

file 'fast_hammer.o' => ['fast_hammer.c', 'Makefile'] do |t|
  sh 'make'
end

task :clean do |t|
  rm_rf Dir['Makefile', '*.o', '*.so', '*.bundle']
end

task :default => 'fast_hammer.o'

