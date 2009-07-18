
require 'fileutils'

file 'Makefile' => 'extconf.rb' do |t|
  sh 'ruby', t.prerequisites[0]
end

OBJ_FILES = %w[ fast_hammer.o sha1.o ]

OBJ_FILES.each do |obj_file|
  file obj_file => [obj_file.sub(/\.o$/, ".c"), 'Makefile'] do |t|
    sh 'make'
  end
end

task :clean do |t|
  rm_rf Dir['Makefile', '*.o', '*.so', '*.bundle']
end

task :default => 'fast_hammer.o'

